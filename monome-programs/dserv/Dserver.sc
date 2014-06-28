
/*
 * The dserver does basically two things. It is will serve as a place to keep
 * track of and manage the samples that are loaded into buffers to be used by
 * the server and it will respond to osc messages that can tell it to play
 * those sample back in various ways. It will load a default set of samples
 * when it is constructed.

 * The server is intended for drum samples just because it is ideal for them.
 * It can be used to trigger general samples however. For example, the dserver
 * can also be used to server for the sound board program eventually (we'll
 * see).
 * 
 * Ways that a sample can be told to play:
 * - one shot
 *
 * - loop - just a static loop. Plays the whole sample start to end over and
 * 			over again. user tells server when to start and when to stop.
 *
 * - walk - similar to loop, but it has position memory - picks up back where
 * 			you left off 
 * - 
 * - 
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

		buffer =(
			sn1: Buffer.read(serv, defaultpath+/+"TR-808Snare01.wav"),
			sn2: Buffer.read(serv, defaultpath+/+"TR-808Clap01.wav"),
			kk1: Buffer.read(serv, defaultpath+/+"TR-808Kick03.wav"),
			kk2: Buffer.read(serv, defaultpath+/+"TR-808Kick15.wav"),
			hho: Buffer.read(serv, defaultpath+/+"TR-808Hat_O01.wav"),
			hhc: Buffer.read(serv, defaultpath+/+"TR-808Hat_C01.wav"),
			rid: Buffer.read(serv, defaultpath+/+"TR-808Ride01.wav"),
			shk: Buffer.read(serv, defaultpath+/+"TR-808Shaker01.wav")

			//lp1: Buffer.read(serv, defaultpath+/+"TR-808Snare01.wav"),
			//lp2: Buffer.read(serv, defaultpath+/+"TR-808Snare01.wav")
			//lp2: Buffer.read(serv, defaultpath+/+"TR-808Snare01.wav"),
			//lp2: Buffer.read(serv, defaultpath+/+"TR-808Snare01.wav"),
		);


		// Synth Initialization
		synth =(
			sn1: Synth(\dtrigger, [\bufnum, buffer[\sn1] ]),
			sn2: Synth(\dtrigger, [\bufnum, buffer[\sn2] ]),
			kk1: Synth(\dtrigger, [\bufnum, buffer[\kk1] ]),
			kk2: Synth(\dtrigger, [\bufnum, buffer[\kk2] ]),
			hho: Synth(\dtrigger, [\bufnum, buffer[\hho] ]),
			hhc: Synth(\dtrigger, [\bufnum, buffer[\hhc] ]),
			rid: Synth(\dtrigger, [\bufnum, buffer[\rid] ]),
			shk: Synth(\dtrigger, [\bufnum, buffer[\shk] ])

			//lp1: Buffer.read(serv, defaultpath+/+"TR-808Snare01.wav"),
			//lp2: Buffer.read(serv, defaultpath+/+"TR-808Snare01.wav")
			//lp2: Buffer.read(serv, defaultpath+/+"TR-808Snare01.wav"),
			//lp2: Buffer.read(serv, defaultpath+/+"TR-808Snare01.wav"),
		);

		// Osc Responder Registration
		OSCdef.newMatching( \trig_respond,
			{|msg|
				synth.do{|sy, index|
					if(sy.asNodeID == msg[3]){ "yup".postln();}
					}
				}, '/n_off');

		OSCdef(\dserver_shot, {|msg| this.trigger(msg[1].asSymbol) }, 
			'/sc/dserver/trigger');//,
	 		//argTemplate: {|args| /*args.postln;*/ true});

		//OSCdef(\dserver_loop, {|msg| this.trigger(msg[1]) }, 
		//	'/sc/dserver/trigger',
	 	//	argTemplate: {|args| args.postln; true});

		//OSCdef(\dserver_walk, {|msg| this.trigger(msg[1]) }, 
		//	'/sc/dserver/trigger',
	 	//	argTemplate: {|args| args.postln; true});
	}

	trigger {|str|
		synth[str].run;
		synth[str].set(\t_trig, 1);
	}

	//set_buffer {|num, file_path|
	//	if( (num >= 0) && (num <= 12)){ 
	//		// free the old one
	//		buffer[num].free;
	//		// allocate the new
	//		// free the cooresponding synth
	//		// allocate the new
	//	}
	//}
}
