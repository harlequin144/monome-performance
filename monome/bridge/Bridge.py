import liblo
import pynotify
import os
import sys
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

		self.primary = Monome(primaryMonome, leftClients, rightClients)
		self.secondary = Monome(secondaryMonome, leftClients, rightClients)
		self.all_clients = leftClients + rightClients

		# Register server methods
		for (rPfx, _),(lPfx, _)  in zip(rightClients, leftClients):
			self.add_method('/' + rPfx + '/hide', '', self.client_hide_responder)
			self.add_method('/' + rPfx + '/grid/led/row', 'iiii', self.led_row)
			#self.add_method('/' + rPfx + '/grid/led/row', 'iii', self.led_row)
			self.add_method('/' + rPfx + '/grid/led/col', 'iiii', self.led_col)
			#self.add_method('/' + rPfx + '/grid/led/col', 'iii', self.led_col)
			self.add_method('/' + rPfx + '/grid/led/map', 'iiiiiiiiii', self.led_map)
			self.add_method('/' + rPfx + '/grid/led/set', 'iii', self.led_set)
			self.add_method('/' + rPfx + '/grid/led/all', 'i', self.led_all)

			self.add_method('/' + lPfx + '/hide', '', self.client_hide_responder)
			self.add_method('/' + lPfx + '/grid/led/row', 'iiii', self.led_row)
			#self.add_method('/' + lPfx + '/grid/led/row', 'iii', self.led_row)
			self.add_method('/' + lPfx + '/grid/led/col', 'iiii', self.led_col)
			#self.add_method('/' + lPfx + '/grid/led/col', 'iii', self.led_col)
			self.add_method('/' + lPfx + '/grid/led/map', 'iiiiiiiiii', self.led_map)
			self.add_method('/' + lPfx + '/grid/led/set', 'iii', self.led_set)
			self.add_method('/' + lPfx + '/grid/led/all', 'i', self.led_all)

		# The generic responder
		self.add_method(None, None, self.oscrespond)

		self.report("Key commands:")
		self.report("q: quit")
		self.report("<>: intensity")
		self.print_screen()
		
		# Light up monome and display some info
		#self.primary.monome_send('/sys/prefix', 'bridge')
		#self.secondary.monome_send('/sys/prefix', 'bridge')
		liblo.send(self.primary.monome_port, '/sys/prefix', 'bridge')
		liblo.send(self.secondary.monome_port, '/sys/prefix', 'bridge')
		liblo.send(self.primary.monome_port, '/sys/port', 8000)
		liblo.send(self.secondary.monome_port, '/sys/port', 8000)
	
		self.primary.switch_to_bridge()
		self.secondary.switch_to_bridge()
		liblo.send(12002, '/serialosc/notify', 'localhost', 8000)
		liblo.send(12002, '/serialosc/list', 'localhost', 8000)


	def run(self):
		while self.on:
			self.recv(10)
			self.keyboard_press_respond()
			#if self.primary.shift_selected != None:
				# double switch
			#if self.primary.shift_selected != None:
				# send the trigger
				# I think that this could work because after any single press, the
				# loop will make the check for a selected press, so there will never
				# be any issue with mulitple fast presss

		# Do This before exiting to restore terminal to normal settings
		os.system('reset')


	#
	# Curses Functions
	#

	def keyboard_press_respond(self):
		key = self.stdscr.getch()
		if key == curses.KEY_BACKSPACE:
			self.primary.switch_to_bridge()
			self.secondary.switch_to_bridge()
			# Child hide

			for prefix, port in self.all_clients:
				if prefix != '':
					liblo.send(port, '/'+prefix+'/hide')

		elif key == ord('q'):
			self.on = False
			liblo.send(57120, '/sc/transport/stop')
			self.primary.light_clear()
			self.secondary.light_clear()
			# send quit messages to all clients
			for pre, port in self.all_clients:
				if pre != '':
					liblo.send(port, '/'+pre+'/quit')
			self.free()

		# Monome Button Brightness Adjustment
		elif key == ord('<'):
			self.primary.dec_intensity()
			self.secondary.dec_intensity()

		elif key == ord('>'):
			self.primary.inc_intensity()
			self.secondary.inc_intensity()


	def print_screen(self):
		for i in range(len(self.print_list)):
			self.stdscr.addstr(i+1,0, self.print_list[i], curses.color_pair(2) )
		self.stdscr.refresh()


	def report(self, string):
		self.print_list.append(string)
		height, width = self.stdscr.getmaxyx()

		while len(self.print_list) >= height:
			self.print_list.popleft()
		self.print_screen()



	#
	# Serialosc Message Handlers
	#


	@liblo.make_method('/bridge/grid/key', None)
	def press_handler(self, path, args, types, src ):
		x, y, z = (args[0], args[1], args[2])

		monome = None
		other_monome = None
		if src.get_port() == self.primary.monome_port:
			monome = self.primary
			other_monome = self.secondary
		elif src.get_port() == self.secondary.monome_port:
			monome = self.secondary
			other_monome = self.primary
				
		if monome != None:
			if not monome.is_at_bridge():
				monome.forward_press(x,y,z)

			else:
				if x > 5 and z == 1:
					if x in [8,9] and y%2 == 0:
						if monome.l_clients != '':
							monome.client = monome.l_clients[y/2]
							monome.light_clear()
							if monome.client == other_monome.client:
								other_monome.light_clear()
							monome.client_send('/show')

					if x in [14,15] and y%2 == 0:
						if monome.r_clients != '':
							monome.client = monome.r_clients[y/2]
							monome.light_clear()
							if monome.client == other_monome.client:
								other_monome.light_clear()
							monome.client_send('/show')


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


	# Generic responder 
	# (Make sure this is registered last!)
	def oscrespond(self, path, args, types, src ):
		self.report(str(src.get_port()) + ': ' + path + ' '+ str(args))



	#
	# Client Message Handlers
	#

	def client_hide_responder(self, path, args, types, src):
		if path.startswith('/'+self.primary.client[0]+'/'):
			self.primary.switch_to_bridge()
		if path.startswith('/'+self.secondary.client[0]+'/'):
			self.secondary.switch_to_bridge()

	 
	# Led Message Forwarding
	# (For sending messages from clients back to serialosc and to the monome)

	def led_all(self, path, args, types, src):
		if path.startswith('/'+self.primary.client[0]+'/'):
			self.primary.monome_send('/grid/led/all', args)
		if path.startswith('/'+self.secondary.client[0]+'/'):
			self.secondary.monome_send('/grid/led/all', args)


	def led_set(self, path, args, types, src):
		x, y, z = args[0], args[1], args[2]

		if not path.startswith('/bridge/'):
			if self.primary.client == self.secondary.client:
				if path.startswith('/'+self.primary.client[0]+'/'):
					if y < 8:
						self.primary.light_set(*args) 
					else:
						self.secondary.light_set(x, y-8, z) 

			else:
				if path.startswith('/'+self.primary.client[0]+'/'):
					self.primary.light_set(x, y, z)
				elif path.startswith('/'+self.secondary.client[0]+'/'):
					self.secondary.light_set(x, y, z)


	def led_map(self, path, args, types, src):
		x_off, y_off = args[0], args[1]

		if not path.startswith('/bridge/'):
			if self.primary.client == self.secondary.client:
				if path.startswith('/'+self.primary.client[0]+'/'):
					if y_off == 0:
						self.primary.light_map(x_off, y_off, args[2:])
					elif y_off == 8:
						self.secondary.light_map(x_off, 0, args[2:])

			else:
				if path.startswith('/'+self.primary.client[0]+'/'):
					self.primary.light_map(x_off, y_off, args[2:])
				elif path.startswith('/'+self.secondary.client[0]+'/'):
					self.secondary.light_map(x_off, y_off, args[2:])


	def led_row(self, path, args, types, src):
		x_off, y, mask1, mask2 = args[0], args[1], args[2], args[3]

		if not path.startswith('/bridge/'):
			if self.primary.client == self.secondary.client:
				if path.startswith('/'+self.primary.client[0]+'/'):
					if y < 8:
						self.primary.light_row(*args) 
					else:
						self.secondary.light_row(x_off, y-8, mask1, mask2) 

			else:
				if path.startswith('/'+self.primary.client[0]+'/'):
					self.primary.light_row(*args)
				elif path.startswith('/'+self.secondary.client[0]+'/'):
					self.secondary.light_row(*args)


	def led_col(self, path, args, types, src):
		x, y_off, mask1, mask2 = args[0], args[1], args[2], args[3]

		if not path.startswith('/bridge/'):
			if self.primary.client == self.secondary.client:
				if path.startswith('/'+self.primary.client[0]+'/'):
					if y == 0:
						self.primary.light_col(x, y_off, mask1) 
						self.secondary.light_col(x, y_off, mask2) 
					elif y == 8:
						self.secondary.light_col(x, y_off, mask1) 

			else:
				if path.startswith('/'+self.primary.client[0]+'/'):
					self.primary.light_row(*args)
				elif path.startswith('/'+self.secondary.client[0]+'/'):
					self.secondary.light_row(*args)
