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
			server_port, monome_ports, left_client, top_clients, bot_clients):

		liblo.Server.__init__(self, server_port)

		self.clients = [x for x in 
				[left_client] + top_clients + bot_clients if x != None]

		self.monomes = [Monome(port, left_client, top_clients, bot_clients) 
				for port in monome_ports]

		self.led_mask = self.gen_led_mask( left_client, top_clients, bot_clients)

		# Curses init
		#self.stdscr = curses.initscr()
		#curses.def_shell_mode()
		#curses.start_color()
		#curses.noecho() #keys are not echoed to screen
		#curses.cbreak() # uncomment to allow instant reaction to keys, not buffd
		#self.stdscr.keypad(1) # return keypad strokes
		#self.stdscr.nodelay(1)
		#curses.curs_set(0)

		# Color stuff
		#curses.init_pair(1, curses.COLOR_CYAN, curses.COLOR_BLACK )
		#curses.init_pair(2, curses.COLOR_BLACK, curses.COLOR_CYAN )
		#curses.init_pair(3, curses.COLOR_WHITE, curses.COLOR_RED )
		#curses.init_pair(4, curses.COLOR_MAGENTA, curses.COLOR_RED )

		# notification init
		pynotify.init("The Bridge")

		self.on = True # used to kill the main run loop

		# Console interface variables
		self.print_list = deque()


		for _, prefix in self.clients: 
			self.add_method(prefix + '/hide', '', self.client_hide_responder)
			self.add_method(prefix + '/grid/led/row', 'iiii', self.led_row)
			#self.add_method(prefix + '/grid/led/col', 'iiii', self.led_col)
			self.add_method(prefix + '/grid/led/set', 'iii', self.led_set)
			self.add_method(prefix + '/grid/led/all', 'i', self.led_all)
			self.add_method(prefix + '/grid/led/map', 'iiiiiiiiii', self.led_map)
			#self.add_method('/' + prefix + '/grid/led/row', 'iii', self.led_row)
			#self.add_method('/' + prefix + '/grid/led/col', 'iii', self.led_col)

		# The generic responder
		self.add_method(None, None, self.oscrespond)

		#self.report("Key commands:")
		#self.report("q: quit")
		#self.report("<>: intensity")
		#self.print_screen()
		
		# Light up monome and display some info
		for monome in self.monomes:
			#self.primary.monome_send('/sys/prefix', 'bridge')
			liblo.send(monome.monome_port, '/sys/prefix', 'bridge')
			liblo.send(monome.monome_port, '/sys/port', 8000)
			monome.mask = self.led_mask
			monome.switch_to_bridge()

		liblo.send(12002, '/serialosc/notify', 'localhost', 8000)
		liblo.send(12002, '/serialosc/list', 'localhost', 8000)

	def gen_led_mask(self, left_client, top_clients, bot_clients):
		mask = ([0,0,0,0,0,0,0,0], [0,0,0,0,0,0,0,0])

		if left_client != None:
			mask[0][3] = 3
			mask[0][4] = 3

		for i, client in zip(range(len(top_clients)), top_clients):
			if client != None:
				if i < 2:
					mask[0][0] = mask[0][0] + 2**((i*4)+1) + 2**((i*4)+2)
					mask[0][1] = mask[0][1] + 2**((i*4)+1) + 2**((i*4)+2)
				else:
					mask[1][0] = mask[1][0] + 2**(((i-2)*4)+1) + 2**(((i-2)*4)+2)
					mask[1][1] = mask[1][0] + 2**(((i-2)*4)+1) + 2**(((i-2)*4)+2)

		for i, client in zip(range(len(bot_clients)), bot_clients):
			if client != None:
				if i < 2:
					mask[0][6] = mask[0][6] + 2**((i*4)+1) + 2**((i*4)+2)
					mask[0][7] = mask[0][7] + 2**((i*4)+1) + 2**((i*4)+2)
				else:
					mask[1][6] = mask[1][6] + 2**(((i-2)*4)+1) + 2**(((i-2)*4)+2)
					mask[1][7] = mask[1][7] + 2**(((i-2)*4)+1) + 2**(((i-2)*4)+2)
		print mask
		return mask


	def run(self):
		while self.on:
			self.recv(10)
			#self.keyboard_press_respond()
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
			for monome in self.monomes:
				monome.switch_to_bridge()

			for prefix, port in self.clients:
				if prefix != '':
					liblo.send(port, prefix + '/hide')

		elif key == ord('q'):
			self.on = False
			liblo.send(57120, '/sc/transport/stop')
			for monome in self.monomes:
				monome.light_clear()
			# send quit messages to all clients
			#for pre, port in self.all_clients:
			#	if pre != '':
			#		liblo.send(port, '/'+pre+'/quit')
			self.free()

		# Monome Button Brightness Adjustment
		elif key == ord('<'):
			for monome in self.monomes:
				monome.dec_intensity()

		elif key == ord('>'):
			for monome in self.monomes:
				monome.inc_intensity()


	def print_screen(self):
		for i in range(len(self.print_list)):
			self.stdscr.addstr(i+1,0, self.print_list[i], curses.color_pair(2) )
		self.stdscr.refresh()


	#def report(self, string):
	#	self.print_list.append(string)
	#	height, width = self.stdscr.getmaxyx()

	#	while len(self.print_list) >= height:
	#		self.print_list.popleft()
	#	self.print_screen()



	#
	# Serialosc Message Handlers
	#


	@liblo.make_method('/bridge/grid/key', None)
	def press_handler(self, path, args, types, src ):
		x, y, z = (args[0], args[1], args[2])

		source_monome = None
		other_monome = None
		for monome in self.monomes:
			if src.get_port() == monome.monome_port:
				source_monome = monome
				break

		if source_monome != None:
			if source_monome.is_at_bridge():
				if x > 5 and z == 1:
					if x in [8,9] and y%2 == 0:
						if source_monome.l_clients != '':
							source_monome.client = source_monome.l_clients[y/2]
							source_monome.light_clear()
							if source_monome.client == other_monome.client:
								other_monome.light_clear()
							source_monome.client_send('/show')

					if x in [14,15] and y%2 == 0:
						if source_monome.r_clients != '':
							source_monome.client = source_monome.r_clients[y/2]
							source_monome.light_clear()
							if source_monome.client == other_monome.client:
								other_monome.light_clear()
							source_monome.client_send('/show')

			else:
				source_monome.forward_press(x,y,z)


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
		#self.report(str(src.get_port()) + ': ' + path + ' '+ str(args))
		print (str(src.get_port()) + ': ' + path + ' '+ str(args))



	#
	# Client Message Handlers
	#

	def client_hide_responder(self, path, args, types, src):
		for monome in self.monomes:
			if path.startswith( monome.selected_client[1] ):
				monome.switch_to_bridge()
				break

	 
	# Led Message Forwarding
	# (For sending messages from clients back to serialosc and to the monome)

	def led_all(self, path, args, types, src):
		if not path.startswith('/bridge/'):
			for monome in self.monomes:
				if path.startswith( monome.selected_client[1] ):
					monome.monome_send('/grid/led/all', args)
					break
			#if path.startswith('/'+self.secondary.client[0]+'/'):


	def led_set(self, path, args, types, src):
		x, y, z = args[0], args[1], args[2]

		if not path.startswith('/bridge/'):
			#if self.primary.client == self.secondary.client:

			for monome in self.monomes:
			#for monome in zip(range(len(slef.monomes)), self.monomes):
				if path.startswith( monome.selected_client[1] ):
					monome.light_set(x, y, z)
					break

	def led_map(self, path, args, types, src):
		x_off, y_off = args[0], args[1]

		if not path.startswith('/bridge/'):
			#if self.primary.client == self.secondary.client:

			for monome in self.monomes:
				if path.startswith( monome.selected_client[1] ):
					monome.light_map(x_off, y_off, args[2:])
					break


	def led_row(self, path, args, types, src):
		x_off, y, mask1, mask2 = args[0], args[1], args[2], args[3]

		if not path.startswith('/bridge/'):

			for monome in self.monomes:
				if path.startswith( monome.selected_client[1] ):
					monome.light_row(*args)
					break


	#def led_col(self, path, args, types, src):
	#	x, y_off, mask1, mask2 = args[0], args[1], args[2], args[3]

	#	if not path.startswith('/bridge/'):
	#		for monome in self.monomes:
	#			if path.startswith( monome.selected_client[1] ):
	#				monome.light_row(*args)
	#				#monome.light_row(*args)
	#				break
