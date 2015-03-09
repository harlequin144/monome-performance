from liblo import send
from gatillod import look_for_device, response_loop



def snare():
	send(57120, "/sc/sampler/snare")
	print "snare"

def kick():
	send(57120, "/sc/sampler/kick")
	print "kick"

def hatc():
	send(57120, "/sc/sampler/hatc")
	print "hatc"
	
def ride():
	send(57120, "/sc/sampler/ride")
	print "ride"

def snare_kick():
	snare()
	kick()


def microbrute_sustain():
	print "micro sustain"
	#send(57120, "/sc/mono/sustain")


press_combos = {
	(2,): kick,
	(5,): snare,
	(6,): snare,
	(7,): hatc,
	(8,): hatc,
	(9,): ride,

	(2, 6): snare_kick,
	(6, 2): snare_kick,
	(9, 10): snare_kick,

	(14,):  microbrute_sustain,
	(4,10,7): kick,
	(4,9,7): kick,
	(4,7):kick,
	(2,7):kick,
}

release = {
	(5): snare,
	(8): hatc,
	(2, 6): snare_kick,
	(6, 2): snare_kick,

	(14):  microbrute_sustain,
}


#old_teensy_id = "USB VID:PID=16c0:0483 SNR=478540"
teensy_id = "USB VID:PID=16c0:0483 SNR=725240"



release = {
		}

while(True):
	device = look_for_device("/dev/ttyButtonBox", teensy_id)
	response_loop(device, press_combos, release)





