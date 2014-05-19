import liblo
import pynotify
import os
import curses
from collections import deque

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

		# Monome interface State variables
		self.monome_intensity = 0;
		self.primaryShow = True
		self.secondaryShow = False
		self.primaryMonome = primaryMonome
		self.secondaryMonome = secondaryMonome
		self.primarySelected = ('bridge', 8000)
		self.secondarySelected = ('bridge', 8000)

		# Transport
		self.tick = 0
		self.tempo = 120
		self.transport = False
		self.trans_press_count = 0
		self.trans_time_factor = 2
			# This will store the numbers 0, 1, 2, 3, 4 representing .25 .5 1 2 4
			# respectively. This is for led lighting convienience.
		
		# Clients
		self.leftClients = leftClients
		self.rightClients = rightClients

		# Register server methods
		for prefix, port in leftClients:
			#print prefix
			self.add_method('/' + prefix + '/grid/key', None,
					self.forward_to_client, port)
			# we need to know from which monome the press came from 

		for prefix, port in rightClients:
			#print prefix
			self.add_method('/' + prefix + '/grid/key', None,
					self.forward_to_client, port)

		self.add_method(None, None, self.oscrespond)

		self.report("Key commands:")
		self.report("q: quit")
		self.report("<>: intensity")
		
		self.init_monomes()



	def init_monomes():
		# Light up monome and display some info
		# When the bridge starts, it takes command
		liblo.send(self.primaryMonome, '/sys/prefix', 'bridge')
		liblo.send(self.primaryMonome, '/bridge/grid/led/intensity', 
				self.monome_intensity)
		self.light_monome()
		liblo.send(self.primaryMonome, '/sys/info', self.get_port())


		liblo.send(12002, '/serialosc/notify', 'localhost', 8000)
		liblo.send(12002, '/serialosc/list', 'localhost', 8000)


	def run(self):
		while self.on:
			self.recv(30)

			key = self.stdscr.getch()
			if key == curses.KEY_BACKSPACE:
				# show responder
				self.primaryShow = True
				liblo.send(self.primaryMonome, '/sys/prefix', 'bridge')
				self.light_monome()
				# Child hide
				for prefix, port in self.leftClients:
					if prefix != '':
						liblo.send(port, '/'+prefix+'/hide')

			elif key == ord('q'):
				curses.reset_shell_mode
				curses.endwin()
				self.on = False
				liblo.send(57120, '/sc/transport/stop')
				liblo.send(self.primaryMonome, '/bridge/grid/led/all', 0)
				# send quit messages to all leftClients
				for y, prefix, port in self.leftClients:
					liblo.send(port, '/'+prefix+'/quit')
				self.free()

			# Monome Button Brightness Adjustment
			# show needs to be true because bridge cant doanything while something
			# else is up
			elif key == ord('<') and self.primaryShow == True:
				self.monome_intensity = self.monome_intensity - 1
				if self.monome_intensity < 0:
					self.monome_intensity = 0
				else:
					liblo.send(self.primaryMonome, '/bridge/grid/led/intensity',
							self.monome_intensity)

			elif key == ord('>') and self.primaryShow == True:
				self.monome_intensity = self.monome_intensity + 1
				if self.monome_intensity > 15:
					self.monome_intensity = 16
				else:
					liblo.send(self.primaryMonome, '/bridge/grid/led/intensity',
							self.monome_intensity)

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
		right = self.primarySelected[0]
		center = ' '*(width - (len(left) + len(right)))
		self.stdscr.addstr(0,0, left + center + right, 
				curses.color_pair(4) | curses.A_BOLD)

	def report(self, str):
		self.print_list.append(str)
		height, width = self.stdscr.getmaxyx()

		while len(self.print_list) >= height:
			self.print_list.popleft()


	def light_monome(self):
		if self.primaryShow:
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

			liblo.send(self.primaryMonome, '/bridge/grid/led/map',0,0, *mask)


			# Other side
			mask = [0,0,0,2**3 + 2**4,2**3 + 2**4,0,0,0]

			# leftClient buttons on the other half
			for y, (prefix, _) in zip(range(4), self.leftClients):
				if prefix != '':
					mask[2*y] = mask[2*y] + 3

			# Right Client Buttons
			for y, (prefix, _) in zip(range(4), self.rightClients):
				if prefix != '':
					mask[2*y] = mask[2*y] + 2**6 + 2**7

			# Light the shift button

			liblo.send(self.primaryMonome, '/bridge/grid/led/map',8,0, *mask)


	def forward_to_client(self, path, args, types, src, data):
		if src.get_port() == self.primaryMonome:
			sendPort = self.primarySelected[1]
			liblo.send(sendPort, path, *args)

		elif src.get_port() == self.secondaryMonome:
			sendPort = self.secondarySelected[1]
			liblo.send(sendPort, path, *args)
			
		#liblo.send(data, path, *args)
		#self.report(str(path))
		#self.report(str(data))
		#self.report(str(args))
		#self.report(str(src.get_hostname()))
		#self.report(str(src.get_port()))
		#self.report(str(src.get_protocol()))
		#self.report(str(src.get_url()))

	
	def transport_press(self, x,y):
		if y == 0: # Toggle on off
			self.trans_press_count = self.trans_press_count + 1
			if self.trans_press_count == 1:
				if self.transport == True:
					# lighting and bridge-side transport state should not be altered
					# here. That is only done when the transport sends back some
					# confirmation
					liblo.send(57120, '/sc/transport/stop')
				else:
					liblo.send(57120, '/sc/transport/start')

		elif y in [1,2,3]:
			self.trans_press_count = self.trans_press_count + 1
			if self.trans_press_count == 1:
				# Tap button
				if x <= 2:	
					liblo.send(57120, '/sc/transport/clear_tap')

				# Tap clear button
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
			old = self.trans_time_factor
			if old != x:
				self.trans_time_factor = x
				if self.trans_time_factor in [2,3]:
					liblo.send(self.primaryMonome, '/bridge/grid/led/row', 0, 6, 12)
					liblo.send(self.primaryMonome, '/bridge/grid/led/row', 0, 7, 12)
					liblo.send(57120, '/sc/transport/factor', float(1))
				else:
					liblo.send(self.primaryMonome, '/bridge/grid/led/row', 0, 6, 2**x)
					liblo.send(self.primaryMonome, '/bridge/grid/led/row', 0, 7, 2**x)

					if x == 0: liblo.send(57120, '/sc/transport/factor', float(4))
					elif x == 1: liblo.send(57120,'/sc/transport/factor',float(2))
					elif x == 4: liblo.send(57120,'/sc/transport/factor',float(0.5))
					elif x == 5: liblo.send(57120,'/sc/transport/factor',float(0.25))


	@liblo.make_method('/bridge/grid/key', None)
	def press_handler(self, path, args, types, src ):
		x = args[0]
		y = args[1]

		if args[2] == 1:
			if x <= 5:
				self.transport_press(x,y)

			elif x in [8,9] and y%2 == 0:
				if self.leftClients[y/2][0] != '':
					if src.get_port() == self.primaryMonome:
						self.primarySelected = self.leftClients[y/2]
						prefix, port = self.leftClients[y/2]

						liblo.send(self.primaryMonome, '/sys/prefix', prefix)
						liblo.send(port, '/' + prefix + '/show')
						self.primaryShow = False

					elif src.get_port() == self.secondaryMonome:
						self.secondarySelected = self.leftClients[y/2]
						prefix, port = self.leftClients[y/2]

						liblo.send(self.secondaryMonome, '/sys/prefix', prefix)
						liblo.send(port, '/' + prefix + '/show')
						self.primaryShow = False


			elif x in [14,15] and y%2 == 0:
				if self.rightClients[y/2][0] != '':
					if src.get_port() == self.primaryMonome:
						self.primarySelected = self.rightClients[y/2]
						prefix, port = self.rightClients[y/2]

						liblo.send(self.primaryMonome, '/sys/prefix', prefix)
						liblo.send(port, '/' + prefix + '/show')
						self.primaryShow = False

					elif src.get_port() == self.secondaryMonome:
						self.secondarySelected = self.rightClients[y/2]
						prefix, port = self.rightClients[y/2]

						liblo.send(self.secondaryMonome, '/sys/prefix', prefix)
						liblo.send(port, '/' + prefix + '/show')
						self.primaryShow = False



			# the shift key
			#elif x in [11,12]:


		elif args[2] == 0:
			if x <= 5 and y <= 3:
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
		liblo.send(self.primaryMonome, '/bridge/grid/led/all', 0)
		self.free()


	@liblo.make_method('/tick', 'i')
	def tick_responder(self, path, args, types, src ):
		self.tick = args[0]

		if self.transport != True:
			clientLights = 0
			lprefix, _ = self.leftClients[0]
			rprefix, _ = self.rightClients[0]
			if lprefix != '':
				clientLights = 3
			if rprefix != '':
				clientLights = clientLights + 2**6 + 2**7
			liblo.send(self.primaryMonome, '/bridge/grid/led/row', 0, 0, 63, 
					clientLights)
			self.transport = True

		if self.primaryShow:
			if args[0]%48 == 0:
				if self.trans_time_factor in [2,3]:
					factor = 12
				else:
					factor = 2**self.trans_time_factor
				mask = [63,56,42,56,51,51,factor,factor]
				liblo.send(self.primaryMonome, '/bridge/grid/led/map',0,0, *mask)

			elif args[0]%48 == 24:
				if self.trans_time_factor in [2,3]:
					factor = 12
				else:
					factor = 2**self.trans_time_factor
				mask = [63,7,5+16,7,51,51,factor,factor]
				liblo.send(self.primaryMonome, '/bridge/grid/led/map',0,0, *mask)


	@liblo.make_method('/bridge/show', None)
	def show_responder(self, path, args, types, src ):
		self.primaryShow = True
		self.report(str(src.get_port()))
		liblo.send(self.primaryMonome, '/sys/prefix', 'bridge')
		liblo.send(self.primaryMonome, '/bridge/grid/led/all', 0)
		self.light_monome()
		self.trans_press_count = 0;

	@liblo.make_method('/bridge/stop', None)
	def stop_responder(self, path, args, types, src ):
		if self.transport == True:
			self.transport = False

		if self.primaryShow == True:
			if self.trans_time_factor in [2,3]:
				factor = 12
			else:
				factor = 2**self.trans_time_factor
			mask = [0,0,0,0,51,51,factor,factor]
			liblo.send(self.primaryMonome, '/bridge/grid/led/map',0,0, *mask)


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
				#"Serialosc", 
				#"Prefix: " + str(self.serialosc_prefix),
				#"critical")
		#note.show ()

	# Generic responder. Make sure this is registered last!
	def oscrespond(self, path, args, types, src ):
		self.report(str(src.port) + ': ' + path + ' '+ str(args))





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
