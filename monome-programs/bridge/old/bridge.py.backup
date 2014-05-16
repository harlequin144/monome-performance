import liblo
import curses
from collections import deque


# TO DO
# - option parsing ??? necessary
# - command line interface
#	- Brightness control


class Bridge(liblo.Server):
	def __init__(self, listen_port, childs):
		liblo.Server.__init__(self, listen_port)

		# Curses
		self.stdscr = curses.initscr()
		curses.start_color()
		curses.noecho() #keys are not echoed to screen
		curses.cbreak() # uncomment to allow instant reaction to keys, not buffd
		self.stdscr.keypad(1) # return keypad strokes
		self.stdscr.nodelay(1)
		curses.curs_set(0)

		# Color stuff
		curses.init_pair(1, curses.COLOR_CYAN, curses.COLOR_BLACK )
		curses.init_pair(2, curses.COLOR_BLACK, curses.COLOR_CYAN )


		# State variables
		self.on = True
		self.show = True
		self.tick = 0
		self.tempo = 120
		self.print_list = deque()
		self.trans_press_count = 0
		
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

		# Light up monome and display some info
		liblo.send(13090, '/sys/prefix', 'bridge')
		self.light_monome()
		liblo.send(13090, '/sys/info', self.get_port())


	def run(self):
		while self.on:
			self.recv(30)

			key = self.stdscr.getch()
			#if key != -1:
				#self.stdscr.addstr(0,0, );
			if key == curses.KEY_BACKSPACE:
				# show responder
				self.show = True
				liblo.send(13090, '/sys/prefix', 'bridge')
				self.light_monome()
				# Child hide
				for y, prefix, port in self.children:
					liblo.send(port, '/'+prefix+'/hide')

			if key == ord('q'):
				curses.nocbreak(); 
				self.stdscr.keypad(0); 
				curses.echo()
				curses.start_color()
				curses.endwin()
				self.on = False
				# send quit messages to all children
				for y, prefix, port in self.children:
					liblo.send(port, '/'+prefix+'/quit')
				break

			self.print_screen()


	def print_screen(self):
		self.stdscr.addstr(0,0, "Tempo: " + str(self.tempo) + " bpm",
				curses.color_pair(1) | curses.A_BOLD)
		for i in range(len(self.print_list)):
			self.stdscr.addstr(i+1,0, self.print_list[i], 
					curses.color_pair(2) );
		self.stdscr.refresh()

	def report(self, str):
		self.print_list.append(str)
		height, width = self.stdscr.getmaxyx()

		while len(self.print_list) >= height:
			self.print_list.popleft()


	def light_monome(self):
		if self.show:

			# Mixer
			for x,y in zip(range(8), self.mix):
				liblo.send(13090, '/bridge/grid/led/col', 
						x, 1, (255 - (2**(7-y)-1)) )

			# Child Buttons
			liblo.send(13090, '/bridge/grid/led/col', 15, 0, 0)
			liblo.send(13090, '/bridge/grid/led/col', 14, 0, 0)
			for y, prefix, port in self.children:
				#print prefix
				liblo.send(13090, '/bridge/grid/led/set', 15, y, 1)
				liblo.send(13090, '/bridge/grid/led/set', 14, y, 1)

			# Transport Buttons
			if self.tick%48 < 24:
				liblo.send(13090, '/bridge/grid/led/col', 'iii', 8, 0, 192)
				liblo.send(13090, '/bridge/grid/led/col', 'iii', 9, 0, 192)
			elif self.tick%48 >= 24:
				liblo.send(13090, '/bridge/grid/led/col', 'iii', 8, 0, 48)
				liblo.send(13090, '/bridge/grid/led/col', 'iii', 9, 0, 48)

			# Clear others
			liblo.send(13090, '/bridge/grid/led/col', 10, 0, 0)
			liblo.send(13090, '/bridge/grid/led/col', 11, 0, 0)
			liblo.send(13090, '/bridge/grid/led/col', 12, 0, 0)
			liblo.send(13090, '/bridge/grid/led/col', 13, 0, 0)



	def forward_to_child(self, path, args, types, src, data):
		liblo.send(data, path, *args)

	@liblo.make_method('/bridge/grid/key', None)
	def press_handler(self, path, args, types, src ):
		if args[2] == 1:
			#print('press responder' + str(args))

			# Mixer Section
			if args[0] <=7:
				self.mix[args[0]] = 7-args[1]
				liblo.send(13090, '/bridge/grid/led/col', 
						args[0], 1, (255 - (2**args[1]-1)) )


			# Transport Section
			elif args[0] in [8,9]:
				self.trans_press_count = self.trans_press_count + 1

				# Start button
				if args[1] in [0,1] and self.trans_press_count == 1:	
						liblo.send(57120, '/transport/start')
						
				# Stop button
				if args[1] in [2,3] and self.trans_press_count == 1:	
						liblo.send(57120, '/transport/stop')
						liblo.send(13090, '/bridge/grid/led/col', 8, 0, 0)
						liblo.send(13090, '/bridge/grid/led/col', 9, 0, 0)
						liblo.send(13090, '/bridge/grid/led/col', 10, 0, 0)
						liblo.send(13090, '/bridge/grid/led/col', 11, 0, 0)

			elif args[0] in [10,11]:
				self.trans_press_count = self.trans_press_count + 1

				# Tap button
				if args[1] in [0,1] and self.trans_press_count == 1: 
						liblo.send(57120, '/transport/tap')

				# Tap clear button
				if args[1] in [2,3] and self.trans_press_count == 1: 
						liblo.send(57120, '/transport/clear_tap')


			# Program Switching 
			elif args[0] in [14,15]:
				for y, prefix, port in self.children:
					#print str(y) + prefix 
					if y == args[1]:
						#print 'switching to child'
						liblo.send(13090, '/sys/prefix', prefix)
						liblo.send(port, '/' + prefix + '/show')
						self.show = False

		elif args[2] == 0:
			if args[0] in [8,9,10,11] and args[1] in [1,2,3,4]:
				self.trans_press_count = self.trans_press_count - 1;



	@liblo.make_method('/bridge/quit', None)
	def quit_responder(self, path, args, types, src ):
		self.on = False
		curses.nocbreak(); 
		self.stdscr.keypad(0); 
		curses.echo()
		curses.endwin()
		liblo.send(57120, '/transport/stop')
		self.free()


	@liblo.make_method('/tick', 'i')
	def tick_responder(self, path, args, types, src ):
		self.tick = args[0]
		if self.show:
			if args[0]%48 < 24:
				liblo.send(13090, '/bridge/grid/led/col', 8, 0, 3)
				liblo.send(13090, '/bridge/grid/led/col', 9, 0, 3)
			elif args[0]%48 >= 24:
				liblo.send(13090, '/bridge/grid/led/col', 8, 0, 12)
				liblo.send(13090, '/bridge/grid/led/col', 9, 0, 12)


	@liblo.make_method('/bridge/show', None)
	def show_responder(self, path, args, types, src ):
		self.show = True
		liblo.send(13090, '/sys/prefix', 'bridge')
		self.light_monome()


	#@liblo.make_method(None, None)
	def oscrespond(self, path, args, types, src ):
		self.report(path + ' ' + str(args))





if __name__ == "__main__":
	#parser = optparse.OptionParser()
	#parser.add_option('-l', '--listen-port', help='port to listen on')


	#(opts, args) = parser.parse_args()



	#Start and run the bridge
	#bridge = Bridge(8000, [(0, 'test', 12345),(7, 'qseq', 7720)])
	bridge = Bridge(8000, [(7, 'slider', 7720)])
	bridge.run()

	bridge.free()


