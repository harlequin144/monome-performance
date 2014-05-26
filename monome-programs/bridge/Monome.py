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
	def __init__(self, monome_port, lClients, rClients):

		# Monome interface State variables
		self.monome_port = monome_port
		self.selected_client = ('bridge', 8000)
		self.led_intensity = 0;
		self.l_clients = lClients
		self.r_clients = rClients

		# Transport
		self.trans = False
		self.trans_factor = 12
		self.trans_press_count = 0

		self.client_map = [0,0,0,24,24,0,0,0]
			# The client map is kept static and is used only to save computation
			# when lighting up the bright. This list can be used every time since
			# the clients are meant to never be changed dynamically

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
		# Transport Side
		if self.trans == False:
		# We'll just let tick take care of the case where the transport is on
			self.light_map(0,0,[0,0,0,0,51,51,self.trans_factor,self.trans_factor])

		# Other side
		self.light_map(8,0, self.client_map)


	def trans_stop(self):
		self.trans = False
		if self.is_at_bridge():
			self.light_map(0,0,[0,0,0,0,51,51,self.trans_factor,self.trans_factor])
		

	def trans_up(self):
		if self.trans != True:
			self.trans = True

		if self.is_at_bridge():
				mask = [63,56,42,56,51,51,self.trans_factor,self.trans_factor]
				self.light_map(0,0, mask)


	def trans_down(self):
		if self.trans != True:
			self.trans = True

		if self.is_at_bridge():
				mask = [63,7,21,7,51,51,self.trans_factor,self.trans_factor]
				self.light_map(0,0, mask)


	#
	# Monome Object State Changing
	#		(not the device or serialosc itself)
	#

	def is_at_bridge(self):
		if self.client == ('bridge', 8000):
			return True
		else:
			return False

	def monome_send(self, path, args):
		liblo.send(self.monome_port, '/bridge'+path, *args)

	def update_factor_mask(self, i):
		if i in [-2,-1]:
			factor = 2**(i + 2)
		elif i == 0:
			factor = 12
		elif i in [1,2]:
			factor = 2**(i + 3)

		if factor != self.trans_factor:
 			self.trans_factor = factor
			if self.is_at_bridge() and self.trans == False:
				self.light_map(0, 0, [0,0,0,0,51,51, factor, factor])


	#
	# Client Interaction
	#

	def client_send(self, path, args):
		liblo.send(self.client[1], '/'+self.client[0]+path, *args)

	def client_send(self, path):
		liblo.send(self.client[1], '/'+self.client[0]+path, None)
		
	def forward_press(self, x, y, z):
		liblo.send(self.client[1], '/'+self.client[0]+'/grid/key', x, y, z)

	def trans_button(self, x, y, z):
		if z == 1:
			if y == 0: # Toggle on off
				self.trans_press_count = self.trans_press_count + 1
				if self.trans_press_count == 1:
					liblo.send(57120, '/sc/transport/toggle')
	
			elif y in [1,2,3]: #tap and clear
				self.trans_press_count = self.trans_press_count + 1
				if self.trans_press_count == 1:
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
