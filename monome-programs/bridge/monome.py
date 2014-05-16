import liblo

class Monome:
	def __init__(self, port):
		self.lights = []
		self.blinkers = []

		self.port = port
		liblo.send(13090, '/bridge/grid/led/all', 0)

		#liblo.send(57120, '/transport/stop')
		#self.light_on((0,0))
		self.light_on((0,1))
		self.blinker_on((0,0))
		self.blinker_on((0,1))
		self.blinker_on((0,2))
		self.blinker_on((0,3))

	def refresh_all(self):
		"""Call this when everything needs to be redrawn, like when switching
		screens. If one thing is being changed then this probably doesn need to be
		called."""
		
		liblo.send(13090, '/bridge/grid/led/all', 0)
		for x,y in self.lights:
			if( ((x,y) not in self.blinkers) or
					((x,y) in self.blinkers and self.blink == True) ):
				liblo.send(13090, '/bridge/grid/led/set', x, y, 1)

		for x,y in self.blinkers:
			if( self.blink == True ):
				liblo.send(13090, '/bridge/grid/led/set', x, y, 1)

	def blink_lights_on(self):
		for x,y in self.blinkers:
			if( (x,y) not in self.lights):
				liblo.send(13090, '/bridge/grid/led/set', x, y, 1)
			else:
				liblo.send(13090, '/bridge/grid/led/set', x, y, 0)

	def blink_lights_off(self):
		for x,y in self.blinkers:
			if( (x,y) not in self.lights):
				liblo.send(13090, '/bridge/grid/led/set', x, y, 0)
			else:
				liblo.send(13090, '/bridge/grid/led/set', x, y, 1)


	def press(self,x,y):
		if x == 15:		# Program selectors
			liblo.send(13090, '/bridge/grid/led/all', 0)
			if y == 7:
				liblo.send(13090, '/sys/prefix', 'qseq')
				liblo.send(7720, '/qseq/show')

		elif x == 0: #transport for now.
			if y == 0:
				liblo.send(57120, '/transport/start')
				self.light_off((0,0))
				self.light_off((0,1))
				self.lights.append((0,2))
				self.lights.append((0,3))
			if y == 1:
				liblo.send(57120, '/transport/stop')
				self.light_off((0,0))
				self.light_on((0,1))
				self.light_off((0,2))
				self.light_off((0,3))
			if y == 2:
				liblo.send(57120, '/transport/tap')
			if y == 3:
				liblo.send(57120, '/transport/clear_tap')


		
	def light_on(self, (x,y)):
		self.lights.append((x,y))
		liblo.send(13090, '/bridge/grid/led/set', x, y, 1)
		
	def light_off(self, (x,y)):
		if( (x,y) in self.lights ):
			self.lights.remove((x,y))
		liblo.send(13090, '/bridge/grid/led/set', x, y, 0)
		
	def blinker_on(self, (x,y)):
		self.blinkers.append((x,y))
		if( (x,y) not in self.lights ):
			liblo.send(13090, '/bridge/grid/led/set', x, y, 0)
		else:
			liblo.send(13090, '/bridge/grid/led/set', x, y, 1)


	def blinker_off(self, (x,y)):
		if (x,y) in self.blinkers:
			self.blinkers.remove((x,y))
			if( (x,y) in self.lights):
				liblo.send(13090, '/bridge/grid/led/set', x, y, 1)
			else:
				liblo.send(13090, '/bridge/grid/led/set', x, y, 0)


		

	
