
from PressTreeNode import *
from liblo import *

def snare():
	send(57120, "/sc/sampler/snare")

def kick():
	send(57120, "/sc/sampler/kick")

def hatc():
	send(57120, "/sc/sampler/hatc")
	
def ride():
	send(57120, "/sc/sampler/ride")

def snare_kick():
	send(57120, "/sc/sampler/snare")
	send(57120, "/sc/sampler/kick")


def microbrute_sustain():
	send(57120, "/sc/mono/sustain")




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
		


press = {
	(1): ride,
	(3): kick,
	(5): snare,
	(6): snare,
	(7): hatc,
	(8): hatc,

	(3, 6): snare_kick,
	(6, 3): snare_kick,

	(14):  microbrute_sustain,
}

release = {
	(5): snare,
	(8): hatc,
	(3, 6): snare_kick,
	(6, 3): snare_kick,

	(14):  microbrute_sustain,
}
