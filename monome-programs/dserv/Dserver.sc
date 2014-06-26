
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
	classvar sampdir = "/home/dylan/music/samples";

	var buffer;
	var synth;

	// Osc/Midi


	*new {
		^super.new.init()
	}

	init {
		// Buffer Initialization
		var serv = Server.local;
		var defaultpath = sampdir+/+"VintageDrumSamples24bit/Roland TR-808";
		buffer = [
			Buffer.read(serv, defaultpath+/+"TR-808Snare01.wav"), // snare 1
			Buffer.read(serv, defaultpath+/+"TR-808Clap01.wav"), // snare 2
			Buffer.read(serv, defaultpath+/+"TR-808Kick03.wav"), // kick 1
			Buffer.read(serv, defaultpath+/+"TR-808Kick15.wav"), // kick 2
			Buffer.read(serv, defaultpath+/+"TR-808Hat_C01.wav"), // hh c
			Buffer.read(serv, defaultpath+/+"TR-808Hat_O01.wav"), // hh o
			Buffer.read(serv, defaultpath+/+"TR-808Shaker01.wav"), // symb oth
			Buffer.read(serv, defaultpath+/+"TR-808Ride01.wav"), // ride
			Buffer.read(serv, defaultpath+/+"TR-808Rim01.wav"), // aux 
			Buffer.read(serv, defaultpath+/+"TR-808Tom01.wav"), // aux 
			Buffer.read(serv, defaultpath+/+"TR-808Cow.wav"), // aux 
			Buffer.read(serv, defaultpath+/+"TR-808Tom02.wav") // aux 
		];

		// Synth Initialization
		synth = buffer.collect({|buf| Synth(\dtrigger, [\bufnum, buf]) });

		// Osc Responder Registration
		OSCdef(\dserver, {|msg| this.trigger(msg[1]) }, '/sc/dserver/trigger');

	}

	trigger {|num|
		if( (num >= 0) && (num <= 12))
		{ synth[num].set(\t_trig, 1) }
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
