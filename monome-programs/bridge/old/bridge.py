
from subprocess import Popen, PIPE
from collections import deque
import threading
import liblo
import time

import urwid.curses_display
import urwid
import sys
import os
from signal import  *

import monome

# To DO
# - impliment checking on serialosc
# - make everything look fancy
# - move the thread stuff to another file
# - classify qseq, and get ready to do so for others
# - front end via monome.
#


class OscRouter(threading.Thread):
	def __init__(self, port, send_ports):
		threading.Thread.__init__(self)
		self.server = liblo.Server(port)
		self.on = True
		self.targets = [ ]
		self.server.add_method(None, None, self.message_handler)

	#def initialize_server(self):
		# Get addresses for all the port to talk to
		for port in send_ports:
			self.targets.append(liblo.Address(port))
		
	def message_handler(self, path, args, types, src):
		# Uncomment this to see all traffic
		#liblo.send(liblo.Address(8888), path,  *(args + [src.port]) )
		for target in self.targets:
			liblo.send(target, path, *args)

	def stop(self):
		self.on = False

	def run(self):
		while self.on:
			self.server.recv(50)

	

# Should the bridge just extend the server class like above?
class Bridge(liblo.ServerThread):
	def __init__(self, port):
		liblo.ServerThread.__init__(self, 8888)

		#List box
		#self.items = [ ]
		self.items = deque()
		self.listbox = urwid.ListBox( self.items )

		# Footer
		self.monome_router_text = urwid.AttrMap( 
				urwid.Text("| monome router |", align='center', ), 'thread off' )
		#self.serialoscd_text = urwid.AttrMap( urwid.Text("| serialosc |"), 'thread off' )
		self.qseq_text = urwid.AttrMap( urwid.Text("| qseq |", 'center'), 'thread off')

		columns = urwid.Columns([self.monome_router_text, self.qseq_text])
		divider = urwid.AttrMap(urwid.Divider(), 'header')
		footer = urwid.Pile([divider, columns, divider])

		#instruct = urwid.Text("Press F8 to exit.")
		#header = urwid.AttrWrap( instruct, 'header' )
		self.top = urwid.Frame(self.listbox, footer=footer)

		self.ui = urwid.curses_display.Screen()
		self.ui.set_input_timeouts(max_wait=0) 
		self.ui.register_palette([
			# name, 					foreground, background, mono
			('I say', 				'black', 		'dark cyan', 	'standout'),# This is a cool color!!!
			('header', 				'default', 	'default', 		'bold'),
			('body bg', 			'default', 	'dark cyan', 	'bold'),
			('bridge report', 'default', 	'dark blue', 	'bold'),
			('bridge error', 	'default', 	'dark red', 	'bold'),
			('thread off', 		'black', 		'light gray', 'bold'),
			('thread error', 	'black', 		'dark red', 	'bold'),
			('thread on', 		'black',		'dark cyan', 	'bold'),])
		
		# Threads
		self.qseq = None

		# Monome
		self.monome = monome.Monome(13090)


	def initialize_server(self):
		# Monome Router
		self.monome_router = OscRouter(8000, [ 8001, 8888, 7720])
		try:
			self.monome_router.start()
			self.monome_router_text.set_attr_map({None:'thread on'}) 
			self.add_item('monome router started.', 'bridge report')
		except liblo.AddressError, err:
			self.add_item("liblo error, failed to start monome router", 'bridge error')
			self.add_item(str(err))
			self.monome_router_text.set_attr_map({None:'thread error'}) 
		
		#self.serialoscd = Popen("serialoscd", close_fds=True)


	def run(self):
		size = self.ui.get_cols_rows()
		self.start()
	
		while True:
			self.draw_screen( size )

			keys = self.ui.get_input()
			if "f8" in keys:
				self.quit()
				break
			for k in keys:
				if k == "window resize":
					size = self.ui.get_cols_rows()
					continue
				self.keypress(size, k )

			# Check that everything is running
			if not self.monome_router.isAlive():
				self.monome_router_text.set_attr_map({None:'thread error'})
				self.add_item("monome router thread appears to have crashed", 'bridge error')

			if self.qseq != None:
				self.qseq.poll()
				if self.qseq.returncode != None:
					self.add_item("qseq thread is not running", 'bridge error')
					self.qseq_text.set_attr_map({None:'thread error'})
					self.qseq = None

			time.sleep(0.05)


	#This is an overriden function
	def keypress(self, size, k):
		if k == 'i':		# Get monome information
			try:
				self.send(liblo.Address(13090), "/sys/info", "localhost", 8888)
			except:
				self.add_item("Failed to retrieve monome info...")

		elif k == 'h':	 # show some helpful info
			self.add_item("Bridge Keys:")
			self.add_item("		i  - get monome info from serialoscd")
			self.add_item("		f8 - close the bridge")
			self.add_item("		q  - attempt to start qseq")
			self.add_item("		Q  - attempt to kill qseq")
			self.add_item("		")

		elif k == 'q':
			self.run_qseq()
		elif k == 'Q':
			self.kill_qseq()

		#else:
			#self.add_item(k)

	def quit(self):
		self.monome_router.stop()
		if self.qseq != None:
			self.qseq.kill()
			self.qseq.wait()
		self.free()
		liblo.send(57120, '/transport/stop')

	def run_qseq(self):
		if self.qseq != None: # then it is running
			self.qseq.poll()
			self.qseq_text.set_attr_map({None:'thread on'})
			if self.qseq.returncode == None:
				self.add_item("qseq is aready engaged", 'bridge report')
			else:
				self.add_item("qseq appears to have exited", 'bridge report')
				self.add_item(str(self.qseq.returncode))

		else:
			try:
				self.qseq = Popen('/home/dylan/Code/monome/qseq/qseq', close_fds=True)
				self.add_item("qseq engaged: "+ str(self.qseq.pid),
						'bridge report')
				self.qseq_text.set_attr_map({None:'thread on'})
			except OSError, err:
				self.add_item("qseq oserrr" + str(err), 'bridge error')
			except ValueError, err:
				self.add_item("qseq valueerror" + str(err), 'bridge error')

		self.monome.light_on((15,7))
		#self.monome.refresh_all()

	def kill_qseq(self):
		if self.qseq != None: #if it is running
			self.qseq.kill()
			self.qseq.wait()
			self.add_item("qseq disengage", 'bridge report')
			self.add_item("return code: " + str(self.qseq.returncode),'bridge report')
			self.qseq = None
			self.qseq_text.set_attr_map({None:'thread off'})
		else:
			self.add_item("qseq is not running", 'bridge report')

		try:
			server = liblo.Server(8001)
			server.free()
			self.add_item("qseq port 8801 is free", 'bridge report')
		except liblo.ServerError, err:
			self.add_item("qseq port 8801 is in use", 'bridge report')
		try:
			server = liblo.Server(7720)
			server.free()
			self.add_item("qseq port 7720 is free", 'bridge report')
		except liblo.ServerError, err:
			self.add_item("qseq port 7720 is in use", 'bridge report')

		self.monome.light_off((15,7))
		#self.monome.refresh_all()

	def draw_screen(self, size):
		canvas = self.top.render(size,focus=True)
		self.ui.draw_screen( size, canvas )

	def add_item(self, text, attr=None):
		if attr == None:
			attr = 'I say'
		elif attr == 'bridge report':
			text = "Bridge:    " + text
		elif attr == 'bridge error':
			text = "ERROR:     " + text

		self.items.append( 
				urwid.AttrMap( urwid.Padding(urwid.Text((attr,text)) ), attr))
		self.listbox.set_focus( len(self.items), coming_from='above' )
		if( len(self.items) >= 200 ):
			self.items.popleft


	@liblo.make_method('/bridge/grid/key', None)
	def press_responder(self, path, args, types, src ):
		if args[2] == 1:
			#self.add_item('press responder' + str(args))
			self.monome.press(args[0], args[1])

	@liblo.make_method('/tick', 'i')
	def tick_responder(self, path, args, types, src ):
		if( args[0] == 0):
			self.monome.blink_lights_on()
		if( args[0] == 16):
			self.monome.blink_lights_off()

	@liblo.make_method('/bridge/show', None)
	def show_responder(self, path, args, types, src ):
		self.monome.refresh_all()

	@liblo.make_method(None, None)
	def oscrespond(self, path, args, types, src ):
		self.add_item(path + '    ' + str(args).strip('[]'))



# -This thread shall be dedicated purely toward ui stuff.
#  That includes sending commands to other entities from here and recieving
#  messages from other entities
# -There will be one other threads as well. The OSC router.
#	-We need to ensure that only one instance will occur


# Threads overseen by the bridge:
# -serialoscd
# -osc router
# -qseq 


#Start and run the bridge
#Try and catch here to make sure bridge is created nicely
bridge = Bridge(8888)
bridge.initialize_server()
bridge.ui.run_wrapper( bridge.run )

bridge.free()




