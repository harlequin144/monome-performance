
/*
 * The dserver does a couple of things. It is will keep track of the drum
 * sample buffers. It will load a default set of samples when it is
 * constructed and will be used to manage changing those samples. Also, it
 * will set up osc responders that will allow for triggering those sample with
 * osc messages from other programs. For now, fast triggering will not make
 * sample playback overlap. 
 *
 * The server is intended for drum samples only because it is ideal for them.
 * It can be used to trigger general samples however. For example, the dserver
 * can also be used to server for the sound board program eventually.
 *
 */

// This is how we make the synth, witch should be loaded already
//n = Synth(\dtrigger, [\bufnum, b]);

// This is how we trigger it.
//n.set(\t_trig, 1)


Dserver {

	//classvar numDservers;

	var buffer;
	var synth;

	// Osc/Midi


	*new {
		^super.new.init(bridgePortNum)
	}

	init {
		bridge = NetAddr.new("localhost", bridgePortNum);

		// Buffer Initialization

		// Osc Responder Registration
		OSCdef(\dserver, {|msg| this.trigger(msg[1]) }, '/sc/dserver/trigger');

	}

	trigger {|num|
		if( (num >= 0) && (num <= 12))
		{ synths[num].set(\t_trig, 1) }
	}

	set_buffer {|num, file_path|
		if( (num >= 0) && (num <= 12)){ 
			// free the old one
			buffer[num].free;
			// allocate the new
			// free the cooresponding synth
			// allocate the new
		}
	}
}
