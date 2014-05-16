import liblo
import pynotify
import os
import curses
from collections import deque


# TO DO
# - option parsing ??? necessary


class Bridge(liblo.Server):
	def __init__(self, listen_port, childs):
		liblo.Server.__init__(self, listen_port)

		# Curses
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

		# State variables
		self.on = True
		self.show = True
		self.tick = 0
		self.tempo = 120
		self.print_list = deque()
		self.monome_intensity = 0;
		self.serialosc_prefix = '';

		# Transport
		self.transport = False
		self.trans_press_count = 0
		self.trans_time_factor = 2
			# This will store the numbers 0, 1, 2, 3, 4 representing .25 .5 1 2 4
			# respectively. This is for led lighting convienience.
		
		# Mixer
		self.mix = [0,0,0,0,0,0,0,0]
		
		# Message Forwarding
		self.children = childs

		# Register server methods
		for y, prefix, port in childs:
			#print prefix
			self.add_method('/' + prefix + '/grid/key', None,
					self.forward_to_child, port)

		self.add_method(None, None, self.oscrespond)

		self.report("Key commands:")
		self.report("q: quit")
		self.report("<>: intensity")
		
		# Light up monome and display some info
		# When the bridge starts, it takes command
		liblo.send(13090, '/sys/prefix', 'bridge')
		liblo.send(13090, '/bridge/grid/led/intensity', self.monome_intensity)
		self.light_monome()
		liblo.send(13090, '/sys/info', self.get_port())


	def run(self):
		while self.on:
			self.recv(30)

			key = self.stdscr.getch()
			if key == curses.KEY_BACKSPACE:
				# show responder
				self.show = True
				liblo.send(13090, '/sys/prefix', 'bridge')
				self.light_monome()
				# Child hide
				for y, prefix, port in self.children:
					liblo.send(port, '/'+prefix+'/hide')

			elif key == ord('q'):
				curses.reset_shell_mode
				curses.endwin()
				self.on = False
				liblo.send(57120, '/sc/transport/stop')
				liblo.send(13090, '/bridge/grid/led/all', 0)
				# send quit messages to all children
				for y, prefix, port in self.children:
					liblo.send(port, '/'+prefix+'/quit')
				self.free()

			# Monome Button Brightness Adjustment
			# show needs to be true because bridge cant doanything while something
			# else is up
			elif key == ord('<') and self.show == True:
				self.monome_intensity = self.monome_intensity - 1
				if self.monome_intensity < 0:
					self.monome_intensity = 0
				else:
					liblo.send(13090, '/bridge/grid/led/intensity',self.monome_intensity)

			elif key == ord('>') and self.show == True:
				self.monome_intensity = self.monome_intensity + 1
				if self.monome_intensity > 15:
					self.monome_intensity = 16
				else:
					liblo.send(13090, '/bridge/grid/led/intensity',self.monome_intensity)

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

		# The last thing we do before exiting
		# this sidesteps the curses problem
		os.system('reset')


	def print_screen(self):
		self.update_tempo_line()
		for i in range(len(self.print_list)):
			self.stdscr.addstr(i+1,0, self.print_list[i], curses.color_pair(2) )
		self.stdscr.refresh()

	def update_tempo_line(self):
		height, width = self.stdscr.getmaxyx()
		left = "Tempo: " + str(self.tempo) + " bpm"
		right = self.serialosc_prefix
		center = ' '*(width - (len(left) + len(right)))
		self.stdscr.addstr(0,0, left + center + right, 
				curses.color_pair(4) | curses.A_BOLD)

	def report(self, str):
		self.print_list.append(str)
		height, width = self.stdscr.getmaxyx()

		while len(self.print_list) >= height:
			self.print_list.popleft()


	def light_monome(self):
		if self.show:
			# Mixer
			mask = [0,0,0,0,0,0,0,0]
			for x in range(8):
				for y in range(8):
					if self.mix[y] >= (7-x):  mask[x] = mask[x] + 2**(y)

			liblo.send(13090, '/bridge/grid/led/map',0,0, *mask)

			# Other side
			mask = [0,0,0,0,0,0,0,0]

			# Child Buttons
			for y, _, _ in self.children:
				mask[y] = 2**6 + 2**7

			# Transport Buttons
			if self.tick%48 < 24:			# Down
				mask[2] = mask[2] + 3
				mask[3] = mask[3] + 3
			elif self.tick%48 >= 24: 	# Up
				mask[0] = mask[0] + 3
				mask[1] = mask[1] + 3

			# Tempo Nudging buttons
			mask[5] = mask[5] + 3 + 8 + 16

			# Factor
			mask[7] = mask[7] + 2**(self.trans_time_factor)

			liblo.send(13090, '/bridge/grid/led/map',8,0, *mask)


	def forward_to_child(self, path, args, types, src, data):
		liblo.send(data, path, *args)


	@liblo.make_method('/bridge/grid/key', None)
	def press_handler(self, path, args, types, src ):
		if args[2] == 1:
			#print('press responder' + str(args))
			x = args[0]
			y = args[1]

			# Mixer Section
			if x <=7:
				self.mix[x] = 7-y
				liblo.send(13090, '/bridge/grid/led/col', 
						args[0], 1, (255 - (2**y -1 )) )


			# Transport Section
			elif x >= 8 and x <= 12:

				if y == 0 or y ==1:
					# Start button
					if x == 8 or x == 9:
						self.trans_press_count = self.trans_press_count + 1
						if self.trans_press_count == 1:
							liblo.send(57120, '/sc/transport/start')
						
					# Tap button
					elif x == 10 or x == 11:
						self.trans_press_count = self.trans_press_count + 1
						if self.trans_press_count == 1:
							liblo.send(57120, '/sc/transport/tap')

				elif y == 2 or y == 3:
					# Stop button
					if x == 8 or x == 9:	
							self.trans_press_count = self.trans_press_count + 1
							if self.trans_press_count == 1:
								liblo.send(57120, '/sc/transport/stop')

					# Tap clear button
					elif x == 10 or x == 11:	
						self.trans_press_count = self.trans_press_count + 1
						if self.trans_press_count == 1:
							liblo.send(57120, '/sc/transport/clear_tap')


				# Nudge Tempo Down
				elif y == 5:	
					if x == 8 or x == 9:
						liblo.send(57120, '/sc/transport/tempo_sub_bpm', float(2))

				# Nudge Tempo Up
					if x == 11 or x == 12:
						liblo.send(57120, '/sc/transport/tempo_add_bpm', 2)

				# Time Factor
				elif y == 7:	
					old = self.trans_time_factor
					if old != x-8:
						self.trans_time_factor = x-8
						if x-8 == 0: liblo.send(57120, '/sc/transport/factor', float(4))
						if x-8 == 1: liblo.send(57120, '/sc/transport/factor', float(2))
						if x-8 == 2: liblo.send(57120, '/sc/transport/factor', float(1))
						if x-8 == 3: liblo.send(57120, '/sc/transport/factor', float(0.5))
						if x-8 == 4: liblo.send(57120, '/sc/transport/factor', float(0.25))

						liblo.send(13090, '/bridge/grid/led/set', 8+old,7,0)
						liblo.send(13090, '/bridge/grid/led/set', x,7,1)


			# Program Switching 
			elif x in [14,15]:
				for y, prefix, port in self.children:
					#print str(y) + prefix 
					if y == args[1]:
						#print 'switching to child'
						liblo.send(13090, '/sys/prefix', prefix)
						liblo.send(port, '/' + prefix + '/show')
						self.show = False

		elif args[2] == 0:
			if args[0] in [8,9,10,11] and args[1] in [0,1,2,3]:
				self.trans_press_count = self.trans_press_count - 1;
				if self.trans_press_count < 0:
					self.trans_press_count = 0



	@liblo.make_method('/bridge/quit', None)
	def quit_responder(self, path, args, types, src ):
		self.on = False
		#curses.reset_shell_mode
		curses.nocbreak(); 
		self.stdscr.keypad(0); 
		curses.echo()
		curses.endwin()
		liblo.send(57120, '/sc/transport/stop')
		liblo.send(13090, '/bridge/grid/led/all', 0)
		self.free()


	@liblo.make_method('/tick', 'i')
	def tick_responder(self, path, args, types, src ):
		self.tick = args[0]

		if self.transport == False:
			self.transport == True

		if self.show:
			mask_8 = 0
			mask_9 = 0
			if self.trans_time_factor == 0:
				mask_8 = mask_8 + 128
			if self.trans_time_factor == 1:
				mask_9 = mask_9 + 128
			if args[0]%48 == 0:
				liblo.send(13090, '/bridge/grid/led/col', 8, 0, 12 + 2**5 + mask_8)
				liblo.send(13090, '/bridge/grid/led/col', 9, 0, 12 + 2**5 + mask_9)
			elif args[0]%48 == 24:
				liblo.send(13090, '/bridge/grid/led/col', 8, 0, 3 + 2**5 + mask_8)
				liblo.send(13090, '/bridge/grid/led/col', 9, 0, 3 + 2**5 + mask_9)


	@liblo.make_method('/bridge/show', None)
	def show_responder(self, path, args, types, src ):
		self.show = True
		liblo.send(13090, '/sys/prefix', 'bridge')
		self.light_monome()
		self.trans_press_count = 0;

	@liblo.make_method('/bridge/stop', None)
	def stop_responder(self, path, args, types, src ):
		if self.transport == True:
			self.transport = False

		if self.show == True:
			liblo.send(13090, '/bridge/grid/led/col', 8, 0, 32)
			liblo.send(13090, '/bridge/grid/led/col', 9, 0, 32)


	@liblo.make_method('/bridge/tempo', 'f')
	def tempo_responder(self, path, args, types, src ):
		self.tempo = args[0]
		self.update_tempo_line()

	# Responders to serialosc notifications
	@liblo.make_method('/sys/connect', None)
	def connectrespond(self, path, args, types, src ):
		note = pynotify.Notification("Serialosc", "A monome has been connected",
		"dialog-information")
		note.set_category("bridge")
		note.show ()

	@liblo.make_method('/sys/disconnect', None)
	def disconnectrespond(self, path, args, types, src ):
		note = pynotify.Notification(
				"Serialosc", 
				"Monome Disconnected.",
				"dialog-information")
		note.show ()

	@liblo.make_method('/sys/prefix', None)
	def prefixrespond(self, path, args, types, src ):
		self.serialosc_prefix = args[0]
		note = pynotify.Notification(
				"Serialosc", 
				"Prefix: " + str(self.serialosc_prefix),
				"critical")
		note.show ()

	# Generic responder. Make sure this is registered last!
	def oscrespond(self, path, args, types, src ):
		self.report(str(src.port) + ': ' + path + ' '+ str(args))





if __name__ == "__main__":
	#parser = optparse.OptionParser()
	#parser.add_option('-l', '--listen-port', help='port to listen on')


	#(opts, args) = parser.parse_args()



	#Start and run the bridge
	#bridge = Bridge(8000, [(0, 'test', 12345),(7, 'qseq', 7720)])
	bridge = Bridge(8000, 
			[(1, 'sc/micronome', 57120),
				(2, 'sc/jenniferhale', 57120),
				(6, 'cutter', 57120),
				(7, 'dseq', 8001)])
	bridge.run()

	bridge.free()


