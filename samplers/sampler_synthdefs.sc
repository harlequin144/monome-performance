#!/usr/local/bin/sclang

// Run this script with sclang to 'install' these synths. Remember the load
// command compiles the synthdef into a file that is loaded everytime that the
// synth is booted. 


SynthDef(\dtrigger,
	{ |bufnum, t_trig = 0, out = 0|
		Out.ar(out, Pan2.ar(
			PlayBuf.ar(1, bufnum, BufRateScale.kr(bufnum), t_trig, 0,
			doneAction: 0)
		))
	}
).load;


// Sampler ideas
// 
//	- trigger 
// 		- start it and let it go
//		- the sample can be stopped 
//		- starting can be quantized
//
//	- randTrigger - trigger a random sample
//	- toggleLoop
//		- start a loop
//		- don't stop until told to
//
//	- interval trigger
//		- when on, trigger a sample at regular intervals with the transport
// 
// 
//


//SynthDef(\sampler,
// 	{ |bufNum, startFrame, endFrame, len, rate = 1, t_trig|
//		var env = Env([0,startFrame,endFrame],[0,len]);
//		
//		frameIndex = EnvGen.ar(env, t_trig);
//		
//		Out.ar(0, Pan2.ar(
//			BufRd.ar(
//				numChannels: 1,
//				bufnum: bufNum,
//				phase: frameIndex,
//				loop: 0 )));
//
//		// Send back info on location for lighting the monome
//		SendTrig.kr(Impulse.kr(25), 0, frameIndex/endFrame); 
//	}
//).add;
