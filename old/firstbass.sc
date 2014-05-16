



n = NetAddr("127.0.0.1", nil); // the url should be the one of computer of app 2

(
o = OSCdef(\bass_responder,
	{ |msg, time, addr, recvPort|
		//msg[1].type.postln;
		if(msg[1] != 0){
			SynthDef("bass",{ arg out=0;
				Out.ar(out,
					BPF.ar(
						LFSaw.ar(msg[1]), msg[1], 2,
						mul: EnvGen.kr(
							Env.perc( 0.01, 0.2, 1, 6 ),
							doneAction: 2)
				))
			}).play;
		}
	},
	'/note');
)

o.remove;

OSCresponderNode.all.do{|pl| pl.remove};

play({SinOsc.ar(freq: 440, mul: 0.3)});

