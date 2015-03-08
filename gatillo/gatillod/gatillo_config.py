from PressTreeNode import *
from liblo import *


#old_teensy_id = "USB VID:PID=16c0:0483 SNR=478540"
teensy_id = "USB VID:PID=16c0:0483 SNR=725240"


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




#tree_head = PressTreeNode
	

#tree = {
#
#	3: { 
#		"func": kick,  
#		7: {
#			"func": snare_kick 
#			} 
#		},
#
#	7:	( snare, { 3: (snare_kick, {}) } ),
#       
#	14: ( microbrute_hold, {} )
#}
		

press_combos = {
	(3,): kick,
	(5,): snare,
	(6,): snare,
	(7,): hatc,
	(8,): hatc,
	(9,): ride,

	(3, 6): snare_kick,
	(6, 3): snare_kick,
	(9, 10): snare_kick,

	(14):  microbrute_sustain,
}



#test_press_combos = {
#	(0,): ride,
#	(1,): ride,
#	(2,): ride,
#	(3,): kick,
#	(4,): ride,
#	(5,): snare,
#	(6,): snare,
#	(7,): hatc,
#	(8,): hatc,
#	(9,): ride,
#	(10,): ride,
#	(11,): ride,
#	(12,): ride,
#	(13,): ride,
#	(14,): ride,
#	(15,): ride,
#
#	(3, 6): snare_kick,
#	(6, 3): snare_kick,
#	(9, 10): snare_kick,
#}

release = {
	(5): snare,
	(8): hatc,
	(3, 6): snare_kick,
	(6, 3): snare_kick,

	(14):  microbrute_sustain,
}
