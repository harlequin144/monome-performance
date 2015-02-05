(
OSCdef(\mono_press,
	{|msg, time|
		if(msg[1] == 0,
		{
			{(SinOsc.ar(600, 0, 0.5)) * (Line.ar(1, 0, 0.125, doneAction: 2)) }.play;
			//"tick".postln;
		});

		//msg.postln;
	},
	'/sc/transport/tick');
)