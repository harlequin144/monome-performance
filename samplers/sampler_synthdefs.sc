#!/usr/local/bin/sclang

SynthDef(\dtrigger,
	{ |bufnum, t_trig = 0, out = 0|
		Out.ar(out, Pan2.ar(
			PlayBuf.ar(1, bufnum, BufRateScale.kr(bufnum), t_trig, 0,
			doneAction: 0)
		))
	}
).load;

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
