
(

OSCdef(\mono_press,
	{|msg, time|
		case
		{msg[1] == 0}{
			"tick!".postln;
			//var osc, env, output;

			//osc = {SinOsc.ar(60)};
			//env = {Line.ar(1, 0, 1, doneAction: 2)};

			//output = osc * env;

			//Out.ar(0,
			//  Pan2.ar(output, 0)
		}
	},
	'/sc/transport/tick');
)


(
var osc, env, output;

osc = {SinOsc.ar(400)};
env = {Line.ar(1, 0, 1, doneAction: 2)};

output = osc * env;

Out.ar(0,Pan2.ar(output, 0));
)