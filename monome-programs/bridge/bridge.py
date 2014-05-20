import liblo
import pynotify
import os
import curses
from collections import deque

from Monome import *

# What to do if I want to use just one monome? How does the program consider
# that case so that it does not attempt to send stuff to the secondary that is
# not there

class Bridge(liblo.Server):
	def __init__(self, 
			listen_port, primaryMonome, secondaryMonome, leftClients, rightClients):

		liblo.Server.__init__(self, listen_port)

		# Curses init
		self.stdscr = curses.initscr()
		curses.def_shell_mode()
		curses.start_color()
		curses.noecho() #keys are not echoed to screen
		curses.cbreak() # uncomment to allow instant reaction to keys, not buffd
		self.stdscr.keypad(1) # return keypad strokes
		self.stdscr.nodelay(1)
		curses.curs_set(0)

		# Color stuff
		curses.init_pair(1, curses.COLOR_CYAN, curses.COLOR_BLACK )
		curses.init_pair(2, curses.COLOR_BLACK, curses.COLOR_CYAN )
		curses.init_pair(3, curses.COLOR_WHITE, curses.COLOR_RED )
		curses.init_pair(4, curses.COLOR_MAGENTA, curses.COLOR_RED )

		# notification init
		pynotify.init("The Bridge")

		self.on = True # used to kill the main run loop

		# Console interface variables
		self.print_list = deque()

		self.primary = Monome(primaryMonome, 8000, 'bridge')
		self.secondary = Monome(secondaryMonome, 8000, 'bridge')

		# Transport
		#self.tick = 0
		self.tempo = 120
		self.transport = False
		self.trans_time_factor = 2
			# This will store the numbers 0, 1, 2, 3, 4 representing .25 .5 1 2 4
			# respectively. This is for led lighting convienience.
		
		# Clients
		self.leftClients = leftClients
		self.rightClients = rightClients

		# Register server methods
		for (rPfx, _),(lPfx, _)  in zip(rightClients, leftClients):
			self.add_method('/' + rPfx + '/hide', '', self.client_hide_responder)
			self.add_method('/' + rPfx + '/grid/led/row', 'iiii', self.led_row)
			self.add_method('/' + rPfx + '/grid/led/row', 'iii', self.led_row)
			self.add_method('/' + rPfx + '/grid/led/col', 'iiii', self.led_col)
			self.add_method('/' + rPfx + '/grid/led/col', 'iii', self.led_col)
			self.add_method('/' + rPfx + '/grid/led/map', None, self.led_map)
			self.add_method('/' + rPfx + '/grid/led/set', 'iii', self.led_set)
			self.add_method('/' + rPfx + '/grid/led/all', '', self.led_all)

			self.add_method('/' + lPfx + '/hide', '', self.client_hide_responder)
			self.add_method('/' + lPfx + '/grid/led/row', 'iiii', self.led_row)
			self.add_method('/' + lPfx + '/grid/led/row', 'iii', self.led_row)
			self.add_method('/' + lPfx + '/grid/led/col', 'iiii', self.led_col)
			self.add_method('/' + lPfx + '/grid/led/col', 'iii', self.led_col)
			self.add_method('/' + lPfx + '/grid/led/map', None, self.led_map)
			self.add_method('/' + lPfx + '/grid/led/set', 'iii', self.led_set)
			self.add_method('/' + lPfx + '/grid/led/all', '', self.led_all)
		self.add_method(None, None, self.oscrespond)

		#self.report("Key commands:")
		#self.report("q: quit")
		#self.report("<>: intensity")
		
		# Light up monome and display some info
		liblo.send(self.primary.monomePort, '/sys/prefix', 'bridge')
		#liblo.send(self.primary.monomePort, '/sys/info', self.get_port())
		self.light_bridge()
		liblo.send(12002, '/serialosc/notify', 'localhost', 8000)
		liblo.send(12002, '/serialosc/list', 'localhost', 8000)


	def run(self):
		while self.on:
			self.recv(30)
			self.keyboard_press_respond()

		# Do This before exiting to restore terminal to normal settings
		os.system('reset')

	def keyboard_press_respond(self):
		key = self.stdscr.getch()
		if key == curses.KEY_BACKSPACE:
			# show responder
			self.primary.show = True
			liblo.send(self.primary.monomePort, '/sys/prefix', 'bridge')
			self.light_bridge()
			# Child hide
			for prefix, port in self.leftClients:
				if prefix != '':
					liblo.send(port, '/'+prefix+'/hide')

		elif key == ord('q'):
			self.on = False
			liblo.send(57120, '/sc/transport/stop')
			self.primary.clear_leds()
			self.secondary.clear_leds()
			# send quit messages to all leftClients
			#for prefix, port in self.leftClients:
				#liblo.send(port, '/'+prefix+'/quit')
			self.free()

		# Monome Button Brightness Adjustment
		# show needs to be true because bridge cant doanything while something
		# else is up
		elif key == ord('<'):
			self.primary.dec_intensity()

		elif key == ord('>'):
			self.primary.inc_intensity()

		elif key == ord('t'):
			# draw a new window
			height, width = self.stdscr.getmaxyx()
			subwin = self.stdscr.subwin(4, 20, height/4, width/4)
			subwin.attrset( curses.color_pair(3) )
			subwin.box()
			subwin.addstr(1,1, "Enter Tempo (BPM):", 
					curses.color_pair(3) | curses.A_BOLD)
			subwin.chgat(2,1,18, curses.color_pair(3) )
			curses.echo() 
			subwin.attrset( curses.color_pair(2) )
			subwin.refresh()
			typed = subwin.getstr() # at this point the whole program is blocked!
			typed = ''.join(c for c in typed if c.isdigit())
			if typed != '':
				self.tempo = float(typed)
				self.update_tempo_line()
				if self.tempo < 20:
					self.tempo = 20
					self.report("tempo too low.")
				if self.tempo > 350:
					self.tempo = 350
					self.report("tempo too hi.")

				liblo.send(57120, '/sc/transport/set_bpm', self.tempo)


			# put everything back like you found it
			curses.noecho() 
			subwin.clear()
			del subwin
			
		self.print_screen()

	def print_screen(self):
		self.update_tempo_line()
		for i in range(len(self.print_list)):
			self.stdscr.addstr(i+1,0, self.print_list[i], curses.color_pair(2) )
		self.stdscr.refresh()

	def update_tempo_line(self):
		height, width = self.stdscr.getmaxyx()
		left = "Tempo: " + str(self.tempo) + " bpm"
		right = self.primary.prefix
		center = ' '*(width - (len(left) + len(right)))
		self.stdscr.addstr(0,0, left + center + right, 
				curses.color_pair(4) | curses.A_BOLD)

	def report(self, str):
		self.print_list.append(str)
		height, width = self.stdscr.getmaxyx()

		while len(self.print_list) >= height:
			self.print_list.popleft()


	def light_bridge(self):
		if self.primary.is_at_bridge():
			# Transport Side
			mask = [0,0,0,0,0,0,0,0]

			# Tempo Nudging buttons 
			if self.transport:
				mask[0] = 63

			mask[4] = 51
			mask[5] = 51

			# Factor
			if self.trans_time_factor in [2,3]:
				mask[6] = 12
				mask[7] = 12
			else:
				mask[6] = 2**(self.trans_time_factor)
				mask[7] = 2**(self.trans_time_factor)

			liblo.send(self.primary.monomePort, '/bridge/grid/led/map',0,0, *mask)


			# Other side
			mask = [0,0,0,24,24,0,0,0]

			# leftClient buttons on the other half
			for y, (prefix, _) in zip(range(4), self.leftClients):
				if prefix != '':
					mask[2*y] = mask[2*y] + 3

			# Right Client Buttons
			for y, (prefix, _) in zip(range(4), self.rightClients):
				if prefix != '':
					mask[2*y] = mask[2*y] + 2**6 + 2**7

			liblo.send(self.primary.monomePort, '/bridge/grid/led/map',8,0, *mask)


	@liblo.make_method('/bridge/grid/key', None)
	def press_handler(self, path, args, types, src ):
		x, y, z = (args[0], args[1], args[2])

		monome = None
		if src.get_port() == self.primary.monomePort:
			monome = self.primary
		elif src.get_port() == self.secondary.monomePort:
			monome = self.secondary
				
		if monome != None:
			if monome.is_at_bridge():
				self.bridge_press(monome, x, y, z)
			else:
				monome.forward_press(x,y,z)


	def bridge_press(self, monome, x, y, z):
		if x <= 5:
			monome.transport_button(x, y, z)
		elif z == 1:
			if x in [8,9] and y%2 == 0:
				if self.leftClients[y/2][0] != '':
					prefix, port = self.leftClients[y/2]
					monome.switch_to_client(prefix, port)

			elif x in [14,15] and y%2 == 0:
				if self.rightClients[y/2][0] != '':
					prefix, port = self.rightClients[y/2]
					monome.switch_to_client(prefix, port)

				# the shift key
				#elif x in [11,12]:



	def client_hide_responder(self, path, args, types, src):
		if path.startswith('/'+self.primary.prefix+'/'):
			self.primary.clientPort = 8000
			self.primary.prefix = 'bridge'
			self.primary.clear_leds()
		if path.startswith('/'+self.secondary.prefix+'/'):
			self.secondary.clientPort = 8000
			self.secondary.prefix = 'bridge'
			self.secondary.clear_leds()

		self.light_bridge()


	#
	# L E D   F O R W A R D I N G
	#

#/grid/led/all s
	def led_all(self, path, args, types, src):
		if path.startswith('/'+self.primary.prefix+'/'):
			self.primary.monome_send('/grid/led/all', args)
		elif path.startswith('/'+self.secondary.prefix+'/'):
			self.secondary.monome_send('/grid/led/all', args)

#/grid/led/set x y s
	def led_set(self, path, args, types, src):
		y = args[1]
		if y < 8:
			if path.startswith('/'+self.primary.prefix+'/'):
				self.primary.monome_send('/grid/led/set', args)
			elif path.startswith('/'+self.secondary.prefix+'/'):
				self.secondary.monome_send('/grid/led/set', args)

		else:
			if path.startswith('/'+self.secondary.prefix+'/'):
				liblo.send(self.secondary.monomePort, '/bridge/grid/led/map', 
						args[0], args[1] - 8, args[2])

#/grid/led/map x_offset y_offset s[8]
	def led_map(self, path, args, types, src):
		y = args[1]
		if y == 0:
			if path.startswith('/'+self.primary.prefix+'/'):
				liblo.send(self.primary.monomePort, '/bridge/grid/led/map', *args)
			elif path.startswith('/'+self.secondary.prefix+'/'):
				liblo.send(self.secondary.monomePort, '/bridge/grid/led/map', *args)

		elif y == 8:
			if path.startswith('/'+self.secondary.prefix+'/'):
				liblo.send(self.secondary.monomePort, '/bridge/grid/led/map', 
						0, 0, *args[2:])


	def led_row(self, path, args, types, src):
		y = args[1]
		if y < 8:
			if path.startswith('/'+self.primary.prefix+'/'):
				liblo.send(self.primary.monomePort, '/bridge/grid/led/row', *args)
			elif path.startswith('/'+self.secondary.prefix+'/'):
				liblo.send(self.secondary.monomePort, '/bridge/grid/led/row', *args)

		else:
			if path.startswith('/'+self.secondary.prefix+'/'):
				liblo.send(self.secondary.monomePort, '/bridge/grid/led/row', 
						args[0], args[1] - 8, args[2], args[3])


#/grid/led/col x y_offset s[..]
	def led_col(self, path, args, types, src):
		y = args[1]
		if y == 0:
			if path.startswith('/'+self.primary.prefix+'/'):
				self.primary.monome_send( '/bridge/grid/led/row', args)
			elif path.startswith('/'+self.secondary.prefix+'/'):
				self.secondary.monome_send('/bridge/grid/led/row', args)

		elif y == 8:
			if path.startswith('/'+self.secondary.prefix+'/'):
				self.secondary.monome_send('/bridge/grid/led/row', 
						[args[0], 0, args[2], args[3]])


	#
	# T R A N S P O R T   R E S P O N D E R S
	#

	@liblo.make_method('/bridge/tempo', 'f')
	def tempo_responder(self, path, args, types, src ):
		self.tempo = args[0]
		self.update_tempo_line()

	@liblo.make_method('/tick', 'i')
	def tick_responder(self, path, args, types, src ):
		tick = args[0]

		if self.transport != True:
			self.transport = True
		if self.primary.is_at_bridge():
			clientLights = 0
			lprefix, _ = self.leftClients[0]
			rprefix, _ = self.rightClients[0]
			if lprefix != '':
				clientLights = 3
			if rprefix != '':
				clientLights = clientLights + 2**6 + 2**7
			liblo.send(self.primary.monomePort, '/bridge/grid/led/row', 
					0, 0, 63, clientLights)

		if self.primary.is_at_bridge():
			if tick%48 == 0:
				if self.trans_time_factor in [2,3]:
					factor = 12
				else:
					factor = 2**self.trans_time_factor
				mask = [63,56,42,56,51,51,factor,factor]
				liblo.send(self.primary.monomePort, '/bridge/grid/led/map',0,0, *mask)

			elif tick%48 == 24:
				if self.trans_time_factor in [2,3]:
					factor = 12
				else:
					factor = 2**self.trans_time_factor
				mask = [63,7,5+16,7,51,51,factor,factor]
				liblo.send(self.primary.monomePort, '/bridge/grid/led/map',0,0, *mask)



	@liblo.make_method('/bridge/stop', None)
	def stop_responder(self, path, args, types, src ):
		if self.transport == True:
			self.transport = False

		if self.primary.is_at_bridge():
			if self.trans_time_factor in [2,3]:
				factor = 12
			else:
				factor = 2**self.trans_time_factor
			args = [0,0, 0,0,0,0,51,51,factor,factor]
			self.primary.monome_send('/grid/led/map', args)



	# Responders to serialosc notifications
	@liblo.make_method('/sys/connect', None)
	def connectrespond(self, path, args, types, src ):
		note = pynotify.Notification("Serialosc", "A monome has been connected",
		"dialog-information")
		note.set_category("bridge")
		note.show()

	@liblo.make_method('/sys/disconnect', None)
	def disconnectrespond(self, path, args, types, src ):
		note = pynotify.Notification(
				"Serialosc", 
				"Monome Disconnected.",
				"dialog-information")
		note.show ()

	#@liblo.make_method('/sys/prefix', None)
	#def prefixrespond(self, path, args, types, src ):
		#self.serialosc_prefix = args[0]
		#note = pynotify.Notification(
				#"Serialosc", "Prefix: " + str(self.serialosc_prefix), "critical")
		#note.show ()

	# Generic responder. Make sure this is registered last!
	def oscrespond(self, path, args, types, src ):
		self.report(str(src.get_port()) + ': ' + path + ' '+ str(args))





if __name__ == "__main__":
	#parser = optparse.OptionParser()
	#parser.add_option('-l', '--listen-port', help='port to listen on')


	#(opts, args) = parser.parse_args()

	# Here is a good place to parse a config file to be used for starting the
	# bridge


	#Start and run the bridge
	#bridge = Bridge(8000, [(0, 'test', 12345),(7, 'qseq', 7720)])
	bridge = Bridge(8000, primaryMonome=13090, secondaryMonome=22222,
			leftClients=[
				('cutter', 57120),
				('sc/jenniferhale', 57120),
				('dseq', 8001),
				('', 0)],
			rightClients=[
				('sc/micronome', 57120),
				('', 0),
				('', 0),
				('', 0)]
			)

	bridge.run()

	bridge.free()
