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

		# This needs to be abolished
		self.primary = Monome(primaryMonome, leftClients, rightClients)
		self.secondary = Monome(secondaryMonome, leftClients, rightClients)

		# Transport
		self.tempo = 120
		
		# Register server methods
		for (rPfx, _),(lPfx, _)  in zip(rightClients, leftClients):
			self.add_method('/' + rPfx + '/hide', '', self.client_hide_responder)
			self.add_method('/' + rPfx + '/grid/led/row', 'iiii', self.led_row)
			self.add_method('/' + rPfx + '/grid/led/row', 'iii', self.led_row)
			#self.add_method('/' + rPfx + '/grid/led/col', 'iiii', self.led_col)
			#self.add_method('/' + rPfx + '/grid/led/col', 'iii', self.led_col)
			self.add_method('/' + rPfx + '/grid/led/map', None, self.led_map)
			self.add_method('/' + rPfx + '/grid/led/set', 'iii', self.led_set)
			self.add_method('/' + rPfx + '/grid/led/all', '', self.led_all)

			self.add_method('/' + lPfx + '/hide', '', self.client_hide_responder)
			self.add_method('/' + lPfx + '/grid/led/row', 'iiii', self.led_row)
			self.add_method('/' + lPfx + '/grid/led/row', 'iii', self.led_row)
			#self.add_method('/' + lPfx + '/grid/led/col', 'iiii', self.led_col)
			#self.add_method('/' + lPfx + '/grid/led/col', 'iii', self.led_col)
			self.add_method('/' + lPfx + '/grid/led/map', None, self.led_map)
			self.add_method('/' + lPfx + '/grid/led/set', 'iii', self.led_set)
			self.add_method('/' + lPfx + '/grid/led/all', '', self.led_all)
		self.add_method(None, None, self.oscrespond)

		#self.report("Key commands:")
		#self.report("q: quit")
		#self.report("<>: intensity")
		
		# Light up monome and display some info
		self.primary.monome_send('/sys/prefix', 'bridge')
		#liblo.send(self.primary.monome_port, '/sys/info', self.get_port())
	
		self.primary.switch_to_bridge()
		self.secondary.switch_to_bridge()
		liblo.send(12002, '/serialosc/notify', 'localhost', 8000)
		liblo.send(12002, '/serialosc/list', 'localhost', 8000)


	def run(self):
		while self.on:
			self.recv(30)
			self.keyboard_press_respond()

		# Do This before exiting to restore terminal to normal settings
		os.system('reset')


	#
	# Curses Functions
	#

	def keyboard_press_respond(self):
		key = self.stdscr.getch()
		if key == curses.KEY_BACKSPACE:
			self.primary.switch_to_bridge()
			# Child hide

			for prefix, port in self.primarqleftClients:
				if prefix != '':
					liblo.send(port, '/'+prefix+'/hide')

		elif key == ord('q'):
			self.on = False
			liblo.send(57120, '/sc/transport/stop')
			self.primary.light_clear()
			self.secondary.light_clear()
			# send quit messages to all leftClients
			for (lPre, lPort), (rPre, rPort) in self.leftClients:
				liblo.send(lPort, '/'+lPre+'/quit')
				liblo.send(rPort, '/'+rPre+'/quit')
			self.free()

		# Monome Button Brightness Adjustment
		elif key == ord('<'):
			self.primary.dec_intensity()
			self.secondary.dec_intensity()

		elif key == ord('>'):
			self.primary.inc_intensity()
			self.secondary.inc_intensity()

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



	#
	# Led and Key Press Forwarding
	#

	@liblo.make_method('/bridge/grid/key', None)
	def press_handler(self, path, args, types, src ):
		x, y, z = (args[0], args[1], args[2])

		monome = None
		if src.get_port() == self.primary.monome_port:
			monome = self.primary
		elif src.get_port() == self.secondary.monome_port:
			monome = self.secondary
				
		if monome != None:
			monome.key_press(x,y,z)
		#if monome != None:
			#if monome.is_at_bridge():
				#self.bridge_press(x,y,z)
			#else:
				#monome.forward_press(x,y,z)




	def bridge_press(self, x,y,z):
		if x <= 5:
			self.trans_button(x,y,z)

		# the shift key
		#elif x in [11, 12]:
		elif z == 1:
			if x in [8,9] and y%2 == 0:
				if self.l_clients != '':
					prefix, port = self.l_clients[y/2]
					self.prefix = prefix
					self.client_port = port
					self.light_clear()
					self.client_send('/show')

			if x in [14,15] and y%2 == 0:
				if self.r_clients != '':
					prefix, port = self.r_clients[y/2]
					self.prefix = prefix
					self.client_port = port
					self.light_clear()
					self.client_send('/show')



	def trans_button(self, x, y, z):
		if z == 1:
			if y  < 4: # Toggle on off
				self.trans_press_count = self.trans_press_count + 1
				if self.trans_press_count == 1:
					if y == 0:
						liblo.send(57120, '/sc/transport/toggle')

					else: #tap and clear
						if x <= 2:	
							liblo.send(57120, '/sc/transport/clear_tap')
						else:
							liblo.send(57120, '/sc/transport/tap')
	
			# Nudge Tempo Down
			elif y in [4,5]:	
				if x == 0:
					liblo.send(57120, '/sc/transport/tempo_sub_bpm', float(3))
				elif x == 1:
					liblo.send(57120, '/sc/transport/tempo_sub_bpm', float(2))
				elif x == 4:
					liblo.send(57120, '/sc/transport/tempo_add_bpm', float(2))
				elif x == 5:
					liblo.send(57120, '/sc/transport/tempo_add_bpm', float(3))
	
	
			# Time Factor
			elif y in [6,7]:	
				if x > 2:
					liblo.send(57120, '/sc/transport/factor_power', x - 3)
				else:
					liblo.send(57120, '/sc/transport/factor_power', x - 2)

		elif z == 0:
			if x <= 5 and y <= 3:
				self.trans_press_count = self.trans_press_count - 1;
				if self.trans_press_count < 0:
					self.trans_press_count = 0


	def led_all(self, path, args, types, src):
		if path.startswith('/'+self.primary.prefix+'/'):
			self.primary.monome_send('/grid/led/all', args)
		elif path.startswith('/'+self.secondary.prefix+'/'):
			self.secondary.monome_send('/grid/led/all', args)


	def led_set(self, path, args, types, src):
		y = args[1]
		if y < 8:
			if path.startswith('/'+self.primary.prefix+'/'):
				self.primary.light_set(*args) 
			elif path.startswith('/'+self.secondary.prefix+'/'):
				self.secondary.light_set(*args) 

		else:
			if path.startswith('/'+self.secondary.prefix+'/'):
				self.secondary.light_set(args[0], args[1] - 8, args[2])


	def led_map(self, path, args, types, src):
		y = args[1]
		if y == 0:
			if path.startswith('/'+self.primary.prefix+'/'):
				self.primary.light_map(args[0], 0, args[2:])
			elif path.startswith('/'+self.secondary.prefix+'/'):
				self.secondary.light_map(args[0], 0, args[2:])

		elif y == 8:
			if path.startswith('/'+self.secondary.prefix+'/'):
				self.secondary.light_map(args[0], 0, args[2:])


	def led_row(self, path, args, types, src):
		y = args[1]
		if y < 8:
			if path.startswith('/'+self.primary.prefix+'/'):
				self.primary.light_row(*args)
			elif path.startswith('/'+self.secondary.prefix+'/'):
				self.secondary.light_row(*args)

		else:
			if path.startswith('/'+self.secondary.prefix+'/'):
				self.secondary.light_row(args[0], args[1]-8, *args[2:])


	#def led_col(self, path, args, types, src):
	#	y = args[1]
	#	if y == 0:
	#		if path.startswith('/'+self.primary.prefix+'/'):
	#			self.primary.monome_send( '/bridge/grid/led/row', args)
	#		elif path.startswith('/'+self.secondary.prefix+'/'):
	#			self.secondary.monome_send('/bridge/grid/led/row', args)

	#	elif y == 8:
	#		if path.startswith('/'+self.secondary.prefix+'/'):
	#			self.secondary.monome_send('/bridge/grid/led/row', 
	#					[args[0], 0, args[2], args[3]])


	#
	# Hide Responder
	#

	def client_hide_responder(self, path, args, types, src):
		if path.startswith('/'+self.primary.prefix+'/'):
			self.primary.switch_to_bridge()
		if path.startswith('/'+self.secondary.prefix+'/'):
			self.secondary.switch_to_bridge()


	#
	# Transport Responders
	#

	@liblo.make_method('/bridge/tempo', 'f')
	def tempo_responder(self, path, args, types, src ):
		self.tempo = args[0]
		self.update_tempo_line()

	@liblo.make_method('/tick', 'i')
	def tick_responder(self, path, args, types, src ):
		tick = args[0]

		if tick%48 == 0:
			if self.primary.is_at_bridge():
				self.primary.trans_up()
			if self.secondary.is_at_bridge():
				self.secondary.trans_up()

		elif tick%48 == 24:
			if self.primary.is_at_bridge():
				self.primary.trans_down()
			if self.secondary.is_at_bridge():
				self.secondary.trans_down()

	@liblo.make_method('/bridge/factor_power', 'i')
	def factor_responder(self, path, args, types, src ):
		self.primary.update_factor_mask(args[0])
		self.secondary.update_factor_mask(args[0])

	@liblo.make_method('/bridge/stop', None)
	def stop_responder(self, path, args, types, src ):
		self.primary.trans_stop()
		self.secondary.trans_stop()


	#
	# Serialosc Interaction
	#

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
