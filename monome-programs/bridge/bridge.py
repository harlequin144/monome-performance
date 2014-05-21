from Bridge import *





if __name__ == "__main__":
	#parser = optparse.OptionParser()
	#parser.add_option('-l', '--listen-port', help='port to listen on')


	#(opts, args) = parser.parse_args()

	# Here is a good place to parse a config file to be used for starting the
	# bridge


	#Start and run the bridge
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
