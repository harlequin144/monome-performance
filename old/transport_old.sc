(

var bridge = NetAddr.new("localhost", 8000);
var dseq = NetAddr.new("localhost", 8001);
var selfsend = NetAddr.new("localhost", 57120);

var tapTimes = List[];
//var status = False;
var tickperiod = 0.5 / 48;
var factor = 1;
var pos = 0;

var schedTickClock = {
	SystemClock.sched(tickperiod,
		{|ticktime|
			bridge.sendMsg("/tick", pos);
			dseq.sendMsg("/tick", pos);
			//selfsend.sendMsg("/tick", pos);
			pos = (pos+1)%48; //pos.postln;
			// resetting the tap times list
			if(tapTimes.size > 0){
				if(ticktime > (2.5 + tapTimes[tapTimes.size-1]))
				{ tapTimes = List[] };
			};
			~mOut.midiClock;

			tickperiod * factor;
		};
	)
};


OSCdef(\transport_stop,
	{|msg, time, addr, recvPort|
		SystemClock.clear;
		//status = False;
		tapTimes = List[];
		~mOut.stop;
		bridge.sendMsg("/bridge/stop");
		dseq.sendMsg("/dseq/stop");
	},
	'/sc/transport/stop');


OSCdef(\transport_start,
	{|msg, startime, addr, recvPort|
		pos = 0;
		//"starting".postln;
		SystemClock.clear;
		schedTickClock.value();
		//status = True;
		~mOut.start;
		bridge.sendMsg("/bridge/tempo", (1/(tickperiod*factor))*(60/48));
	},
	'/sc/transport/start');

OSCdef(\transport_tap,
	{|msg, time, addr, recvPort|
		tapTimes.add(time);
		//get the lengths
		if(tapTimes.size > 1){
			var sum = 0;
			1.for(tapTimes.size-1, {|indx|
				sum = sum + (tapTimes[indx]-tapTimes[indx-1])});

			tickperiod = (sum / (tapTimes.size-1))/48;
			//tickperiod.postln;
			pos = 0;
			SystemClock.clear;
			schedTickClock.value();
			//Send the timing to the bridge in bpm
			bridge.sendMsg("/bridge/tempo",
				(1/(tickperiod*factor))*(60/48));
		};
	},
	'/sc/transport/tap'
);

OSCdef(\transport_clear_tap,
	{ tapTimes = List[] },
	'/sc/transport/clear_tap'
);

OSCdef(\transport_set,
	{|msg, time, addr, recvPort|
		tickperiod = (1 / ((msg[1] * 48)/60));
		bridge.sendMsg("/bridge/tempo", (1/(tickperiod*factor))*(60/48));
	},
	'/sc/transport/set_bpm');

OSCdef(\transport_tempo_add_bpm,
	{|msg, time, addr, recvPort|
		var bpm = (1/tickperiod)*(60/48);
		//msg.postln; tickperiod.postln; bpm.postln;
		bpm = bpm + msg[1];
		if(bpm > 350) {bpm = 350};
		tickperiod = (1 / ((bpm * 48)/60));
		bridge.sendMsg("/bridge/tempo", (1/(tickperiod*factor))*(60/48));
	},
	'/sc/transport/tempo_add_bpm');

OSCdef(\transport_tempo_sub_bpm,
	{|msg|
		var bpm = (1/tickperiod)*(60/48);
		bpm = bpm - msg[1];
		if(bpm < 20) {bpm = 20};
		tickperiod = (1 / ((bpm * 48)/60));
		bridge.sendMsg("/bridge/tempo", (1/(tickperiod*factor))*(60/48));
	},
	'/sc/transport/tempo_sub_bpm');

OSCdef(\transport_factor,
	{|msg|
		if((msg[1] > 0.0) && (msg[1] <= 9.0)) {
			factor = msg[1]
		};
		//factor.postln;
		bridge.sendMsg("/bridge/tempo", (1/(tickperiod*factor))*(60/48));
	},
	'/sc/transport/factor');
)