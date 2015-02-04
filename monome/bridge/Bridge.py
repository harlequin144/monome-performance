import liblo
import os
import sys
import curses

from collections import deque
from Monome import *
from Client import *


class Bridge(liblo.Server):
	def __init__(self, 
			server_port, monome_ports, top_clients, mid_clients, bot_clients):

		liblo.Server.__init__(self, server_port)

		top_clients = (top_clients + ([None] * (4 - len(top_clients))))[:4]
		mid_clients = (mid_clients + ([None] * (3 - len(mid_clients))))[:3]
		bot_clients = (bot_clients + ([None] * (4 - len(bot_clients))))[:4]

		self.clients = {}
		for i, top, bot in zip(range(4), top_clients, bot_clients):
			if top != None:
				top = Client(top[0], top[1])

			if bot != None:
				bot = Client(bot[0], bot[1])

			self.clients[((i*4) + 1 , 0)] = top
			self.clients[((i*4) + 2 , 0)] = top
			self.clients[((i*4) + 1 , 1)] = top
			self.clients[((i*4) + 2 , 1)] = top

			self.clients[((i*4) + 1, 6)] = bot
			self.clients[((i*4) + 2, 6)] = bot
			self.clients[((i*4) + 1, 7)] = bot
			self.clients[((i*4) + 2, 7)] = bot

		for i, client in zip(range(3), mid_clients):
			if client != None:
				client = Client(client[0], client[1])
			self.clients[((i*7) + 1, 3)] = client
			self.clients[((i*7) + 0, 3)] = client
			self.clients[((i*7) + 1, 4)] = client
			self.clients[((i*7) + 0, 4)] = client


		self.monomes = {} #{port: Monome(port) for port in monome_ports} 

		self.led_mask = self.gen_led_mask()

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

		self.on = True

		# Console interface variables
		self.print_list = deque()


		for prefix in [client.prefix for client in self.get_clients()]: 
			self.add_method(prefix + '/hide', '', self.client_hide_responder)
			self.add_method(prefix + '/grid/led/row', 'iiii', self.led_row)
			#self.add_method(prefix + '/grid/led/col', 'iiii', self.led_col)
			self.add_method(prefix + '/grid/led/set', 'iii', self.led_set)
			self.add_method(prefix + '/grid/led/all', 'i', self.led_all)
			self.add_method(prefix + '/grid/led/map', 'iiiiiiiiii', self.led_map)

		# The generic responder - this is here because it needs to be added last
		self.add_method(None, None, self.oscrespond)

		self.report("Key commands:")
		self.report("q: quit")
		self.report("<>: intensity")
		self.print_screen()
		
		for monome in self.monomes.itervalues():
			liblo.send(monome.monome_port, '/sys/prefix', 'bridge')
			liblo.send(monome.monome_port, '/sys/port', 8000)
			monome.switch_to_bridge()
			monome.light_map(0, 0, self.led_mask[0])
			monome.light_map(8, 0, self.led_mask[1])

		liblo.send(12002, '/serialosc/notify', 'localhost', 8000)
		liblo.send(12002, '/serialosc/list', 'localhost', 8000)


	def gen_led_mask(self):
		mask = ([0,0,0,0,0,0,0,0], [0,0,0,0,0,0,0,0])

		for (x,y) in iter(self.clients):
			if self.clients[(x,y)] != None:
				if x < 8:
					mask[0][y] += 2**x
				else:
					mask[1][y] += 2**(x - 8)

		return mask


	def run(self):
		while self.on:
			self.recv(10)
			self.keyboard_press_respond()

		# Do This before exiting to restore terminal to normal settings
		os.system('reset')


	def get_clients(self):
		return [c for c in self.clients.itervalues() if c != None]


	#
	# Curses Functions
	#

	def keyboard_press_respond(self):
		key = self.stdscr.getch()
		if key == curses.KEY_BACKSPACE:
			for monome in self.monomes.itervalues():
				monome.switch_to_bridge()
				monome.light_map(0, 0, self.led_mask[0])
				monome.light_map(8, 0, self.led_mask[1])


		elif key == ord('q'):
			self.on = False

			for monome in self.monomes.itervalues():
				monome.light_clear()

			for client in self.clients.itervalues():
				if client != None:
					liblo.send(client.port, client.prefix + '/quit')


		# Monome Button Brightness Adjustment
		elif key == ord('<'):
			for monome in self.monomes.itervalues():
				monome.dec_intensity()

		elif key == ord('>'):
			for monome in self.monomes.itervalues():
				monome.inc_intensity()


	def print_screen(self):
		for i in range(len(self.print_list)):
			self.stdscr.addstr(i,0, self.print_list[i], curses.color_pair(2) )
		self.stdscr.refresh()


	def report(self, string):
		height, width = self.stdscr.getmaxyx()

		self.print_list.append( string + (" " * (width - len(string))) )

		while len(self.print_list) >= height:
			self.print_list.popleft()
		self.print_screen()



	#
	# Serialosc Message Handlers
	#


	@liblo.make_method('/bridge/grid/key', None)
	def press_handler(self, path, args, types, src ):
		x, y, z = (args[0], args[1], args[2])

		source_monome = self.monomes[src.get_port()]

		if source_monome != None:
			if source_monome.is_at_bridge():
				if x < 8:
					if self.led_mask[0][y] & 2**x:
						source_monome.switch_to_client( self.clients[(x,y)] )

				else:
					if self.led_mask[1][y] & 2**(x-8):
						source_monome.switch_to_client( self.clients[(x,y)] )
								

			else:
				source_monome.forward_press(x,y,z)


	@liblo.make_method('/serialosc/device', None)
	def serialosc_device(self, path, args, types, src ):
		self.report( "Serialosc: device present" )
		self.report( "Adding " + str(args[0]) + ": " + str(args[2]) )
		self.monomes[ args[2] ] = Monome( args[2])
		self.monomes[ args[2] ].switch_to_bridge()
		self.monomes[ args[2] ].light_map(0, 0, self.led_mask[0])
		self.monomes[ args[2] ].light_map(8, 0, self.led_mask[1])


	@liblo.make_method('/serialosc/add', None)
	def serialosc_add(self, path, args, types, src ):
		self.report( "Serialosc: device added" )
		self.report( "Adding " + str(args[0]) + ": " + str(args[2]) )
		self.monomes[ args[2] ] = Monome( args[2])
		self.monomes[ args[2] ].switch_to_bridge()
		self.monomes[ args[2] ].light_map(0, 0, self.led_mask[0])
		self.monomes[ args[2] ].light_map(8, 0, self.led_mask[1])


	@liblo.make_method('/serialosc/remove', None)
	def serialosc_remove(self, path, args, types, src ):
		self.report( "Serialosc: device removed" )
		self.report( "Removing " + str(args[0]) + ": " + str(args[2]) )
		#self.monomes[ args[2] ].free()
		if args[2] in self.monomes.keys():
			del self.monomes[ args[2] ]


	@liblo.make_method('/sys/connect', None)
	def sys_connect(self, path, args, types, src ):
		self.report( "Sys: device connected" )
		liblo.send(12002, '/serialosc/notify', 'localhost', 8000)
		liblo.send(12002, '/serialosc/list', 'localhost', 8000)


	#@liblo.make_method('/sys/disconnect', None)
	#def disconnectrespond(self, path, args, types, src ):


	# Generic responder (Make sure this is registered last!)
	def oscrespond(self, path, args, types, src ):
		self.report(str(src.get_port()) + ': ' + path + ' '+ str(args))



	#
	# Client Message Handlers
	#

	def client_hide_responder(self, path, args, types, src):
		for monome in self.monomes.itervalues():
			if not monome.is_at_bridge():
				if path.startswith( monome.selected_client.prefix ):
					monome.switch_to_bridge()
					monome.light_map(0, 0, self.led_mask[0])
					monome.light_map(8, 0, self.led_mask[1])

	 
	# Led Message Forwarding
	# (For sending messages from clients back to serialosc and to the monome)

	def led_all(self, path, args, types, src):
		for monome in self.monomes.itervalues():
			if not monome.is_at_bridge():
				if path.startswith( monome.selected_client.prefix):
					monome.monome_send('/grid/led/all', args)


	def led_set(self, path, args, types, src):
		x, y, z = args[0], args[1], args[2]

		for monome in self.monomes.itervalues():
			if not monome.is_at_bridge():
				if path.startswith( monome.selected_client.prefix ):
					monome.light_set(x, y, z)


	def led_map(self, path, args, types, src):
		x_off, y_off = args[0], args[1]

		for monome in self.monomes.itervalues():
			if not monome.is_at_bridge():
				if path.startswith( monome.selected_client.prefix ):
					monome.light_map(x_off, y_off, args[2:])


	def led_row(self, path, args, types, src):
		x_off, y, mask1, mask2 = args[0], args[1], args[2], args[3]

		for monome in self.monomes.itervalues():
			if not monome.is_at_bridge():
				if path.startswith( monome.selected_client.prefix ):
					monome.light_row(*args)


	#def led_col(self, path, args, types, src):
	#	x, y_off, mask1, mask2 = args[0], args[1], args[2], args[3]

	#	if not path.startswith('/bridge/'):
	#		for monome in self.monomes:
	#			if path.startswith( monome.selected_client.prefix ):
	#				monome.light_row(*args)
	#				#monome.light_row(*args)
	#				break
