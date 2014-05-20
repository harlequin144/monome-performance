import liblo

# This class is meant to help encapsulate data that is specific to each
# monome and to give some convienence methods for lighting the monomes. It
# will not hold any variables that are used for the bridge as a whole, for
# example variables that are specific to the transport.


class Monome(): #(liblo.Address):
	def __init__(self, monomePort, clientPort, prefix):
		#liblo.Server.__init__(self, listen_port)

		# Monome interface State variables
		self.monomePort = monomePort
		self.clientPort = clientPort
		self.prefix = prefix
		self.monome_intensity = 0;
		self.show = True

		self.trans_press_count = 0

	def clear_leds(self):
		liblo.send(self.monomePort, '/bridge/grid/led/all', 0)
		self.trans_press_count = 0


	def switch_to_client(self, prefix, port):
		self.prefix = prefix
		self.clientPort = port
		liblo.send(self.monomePort, '/bridge/grid/led/all', 0)
		#self.send('/show', [])
		liblo.send(self.clientPort, '/'+self.prefix+'/show' )
		#print '/'+self.prefix+'/show'

	def is_at_bridge(self):
		if self.prefix == 'bridge':
			return True
		else:
			return False

	def client_send(self, path, args):
		liblo.send(self.clientPort, '/'+self.prefix+path, *args)

	def monome_send(self, path, args):
		liblo.send(self.monomePort, '/bridge'+path, *args)

	def forward_press(self, x, y, z):
		liblo.send(self.clientPort, '/'+self.prefix+'/grid/key', x, y, z)
		#print self.prefix
		#print self.clientPort

	def transport_button(self, x, y, z):
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

		elif z == 0:
			if x <= 5 and y <= 3:
				self.trans_press_count = self.trans_press_count - 1;
				if self.trans_press_count < 0:
					self.trans_press_count = 0







	# Responders to serialosc notifications
	#@liblo.make_method('/sys/connect', None)
	#def connectrespond(self, path, args, types, src ):
	#	note = pynotify.Notification("Serialosc", "A monome has been connected",
	#	"dialog-information")
	#	note.set_category("bridge")
	#	note.show()
