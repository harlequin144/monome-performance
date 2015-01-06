(
var mono_addr = NetAddr.new("127.0.0.1", 13090);
var scPort = NetAddr.new("localhost", 57120);

var lastLedPos = 0;
var frameIndex = 0;
var synth;
var synthGate = 0;

b = Buffer.read(s, Platform.resourceDir +/+"sounds/a11wlk01-44_1.aiff");
{PlayBuf.ar(1, b)}.play;


 SynthDef(\sampler,
 	{ |bufNum, startFrame, endFrame, len, rate = 1, t_trig|

		var env = Env([0,startFrame,endFrame],[0,len]);

 		frameIndex = EnvGen.ar(env, t_trig);

 		Out.ar(0, Pan2.ar(
 			BufRd.ar(
 				numChannels: 1,
 				bufnum: bufNum,
 				phase: frameIndex,
 				loop: 0
 			))
 		);

 		//Send back info on location for lighting the monome
 		SendTrig.kr(Impulse.kr(25), 0, frameIndex/endFrame);

 	}
 ).add;

synth = Synth(\sampler, [bufNum: b]);




OSCdef(
	\play_houston,
	{ |msg, time, addr, recvPort|
		if(msg[3] == 1){
			var startFrame = (b.numFrames / 16) *msg[1];
			var endFrame = b.numFrames;
			var sampleTime = ((endFrame - startFrame)/b.sampleRate);
			//startFrame.postln;
			//msg.postln;

			//synth.set(\t_trig, 0);
			synth.set(\startFrame, startFrame,
				\endFrame, endFrame,
				\len, sampleTime,
				\t_trig, 1);
		}
	},
	'/cutter/grid/key'
);



 OSCdef(
 	\trig_respond,
 	{ arg msg, time;
		var new = floor((msg[3]*16));
 		//[time, msg].postln;
		msg[3].postln;
		//new.postln;

 		if(lastLedPos != new,{
 			lastLedPos = new;
 			//lastLedPos.postln;

 			//mono_addr.sendMsg("/cutter", 1, new);
 			if(lastLedPos < 8){
 				mono_addr.sendMsg( "/cutter/grid/led/row",0, 3, 2**lastLedPos, 0);
 			}{
				mono_addr.sendMsg( "/cutter/grid/led/row",0,3, 0, 2**(lastLedPos-8));
 			}
 		});
 	},
 '/tr');

)




//Sources

// This thread gives the idea behind changing the envelope values
// There is something interesting in there about giving the envelope
// a 0 initial time. I need to understand that
// "http://new-supercollider-mailing-lists-forums-use-these.2681727.n2.nabble.com/How-to-retrigger-an-envelope-or-line-in-the-middle-using-its-current-value-as-new-starting-point-td7490360.html"
