import liblo

# This class is meant to help encapsulate data and methods that are specific
# to each monome and to give some convienence methods for lighting the monomes
# and forwarding presses from them to clients.  Whenever an operation becomes
# monome-specific, the operation happens with this object. For example once
# the bridge knows that an led message must go to monome x, the actual led
# message is sent using this object. To keep things speedy, some information
# that is local to the bridge as a whole is kept in each monome object,
# specifically transport state and the list of clients. This is done to the
# end of achieving speed and elegance.


class Monome(): #(liblo.Address):
	def __init__(self, monome_port, left_client, lClients, rClients):

		# Monome interface State variables
		self.monome_port = monome_port
		self.led_intensity = 0;

		#self.press_count

		self.selected_client = ('bridge', 8000)
		#self.l_clients = lClients
		#self.r_clients = rClients

		self.mask = None
		self.client_map = [0,0,0,24,24,0,0,0]
			# The client map is kept static and is used only to save computation
			# when lighting up the leds. This list can be used every time since
			# the clients are meant to never be changed after starting.

		# Initialization
		for x, (lPre, lPort), (rPre, rPort) in zip(range(4), lClients, rClients):
			if lPre != '':
				self.client_map[2*x] = self.client_map[2*x] + 3
			if rPre != '':
				self.client_map[2*x] = self.client_map[2*x] + 2**6 + 2**7

		liblo.send(self.monome_port, '/bridge/grid/led/intensity', 0)

	#
	# Monome Key Press Function
	#		This baby does the main job of the program
	#

	def key_press(self, x, y, z):
		if self.client == ('bridge', 8000):
			self.bridge_press(x,y,z)
		else:
			self.forward_press(x,y,z)


	#
	# Led Convenience Functions
	#

	def light_clear(self):
		liblo.send(self.monome_port, '/bridge/grid/led/all', 0)

	def light_row(self, x_off, y, mask1, mask2=0):
		liblo.send(self.monome_port, '/bridge/grid/led/row', 
				x_off, y, mask1, mask2)

	def light_col(self, x, y_off, mask):
		liblo.send(self.monome_port, '/bridge/grid/led/col', x, y_off, mask)

	def light_map(self, x_off, y_off, mp):
		liblo.send(self.monome_port, '/bridge/grid/led/map', x_off, y_off, *mp)

	def light_set(self, x, y, z):
		liblo.send(self.monome_port, '/bridge/grid/led/set', x, y, z)

	def dec_intensity(self):
		self.led_intensity = self.led_intensity - 1;
		if self.led_intensity < 0:
			self.led_intensity = 0
		liblo.send(self.monome_port, '/bridge/grid/led/intensity',
				self.led_intensity)

	def inc_intensity(self):
		self.led_intensity = self.led_intensity + 1 ;
		if self.led_intensity > 15:
			self.led_intensity = 15
		liblo.send(self.monome_port, '/bridge/grid/led/intensity',
				self.led_intensity)


	#
	# Bridge Interaction
	#

	def switch_to_bridge(self):
		self.client = 'bridge', 8000

		#self.light_map(0,0,[0,0,0,0,0,0,0,0])
		#self.light_map(8,0, self.client_map)
		self.light_map(0,0, self.mask[0])
		self.light_map(8,0, self.mask[1])

	def is_at_bridge(self):
		if self.client == ('bridge', 8000):
			return True
		else:
			return False

	#
	# Send to the serialosc deamon for this monome
	#

	def monome_send(self, path, args):
		liblo.send(self.monome_port, '/bridge'+path, *args)

	#
	# Sending messages back to clients
	#

	def client_send(self, path, args):
		liblo.send(self.client[1], '/'+self.client[0]+path, *args)

	def client_send(self, path):
		liblo.send(self.client[1], '/'+self.client[0]+path)
		
	def forward_press(self, x, y, z):
		liblo.send(self.client[1], '/'+self.client[0]+'/grid/key', x, y, z)
