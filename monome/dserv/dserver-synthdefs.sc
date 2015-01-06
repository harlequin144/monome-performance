#!/usr/local/bin/sclang

/*
 * This is a synthdef that the dserver uses when triggering things server
 * side. It will be assumed that this is already loaded into the synthdefs
 * directory when dserv is started, so consider running this script and
 * loading this synth def into a directory that sc will see and use properly a
 * part of the build process for this program. i. e. look to see that this
 * synthdef is properly done when there are errors.
 *
 */


/*
 * This synth just does simple triggering. It will just play the sample to
 * it's end when it is triggered. No loop. No stopping once it's started.
 */

SynthDef(\dtrigger,
	{ |bufnum, t_trig = 0, out = 0|
		Out.ar(out, Pan2.ar(
			PlayBuf.ar(1, bufnum, BufRateScale.kr(bufnum), t_trig, 0,
			doneAction: 0)
		))
	}
).load;


/*
 * This synth will work like this. When a user triggers it with no parameters,
 * it will start and continue on until it recieves a stop message. When it
 * stops it will remember where it was at so that when it is started again, it
 * will start at the same place in the buffer. When it reaches the end of the
 * buffer, it will just top. The user will have to trigger it again in order
 * to make it go again.
 * 
 * When a user sends info about where to start, the playback will begin there
 * and will stop at the end. Again, no loop around.
 * 
 * The user will also be able to send some info about where to send back info
 * about the state of the sample play back. The synth along with dserver will
 * send back regular messages about the progress of play back and a mesasge at
 * the end when it is done.
 */



 // Make this synth more general??? as in totally general for looping
 // purposes?


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
//


