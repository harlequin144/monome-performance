Transport {

	//calssvar numtransports
	const path = "/sc/transport";

	var midiOut;
	var tapTimes; // = List[];
	var tickperiod;
	var factor = 1;
	var new_factor = 0;
	var <tick = 0;
	var <on = false;

	var bridge, otherPorts, scPort;
	var midiOut;

	*new {|midiUid, bridgePort = 8000 ... othPorts|
		^super.new.init(midiUid, bridgePort, *othPorts);
	}

	init {|midiUid, bridgePort = 8000 ... othPorts|
		bridge = NetAddr.new("localhost", bridgePort);
		scPort = NetAddr.new("localhost", 57120);
		otherPorts = othPorts.collect(
			{|port| NetAddr.new("localhost", port);}
		);
		midiOut = MIDIOut(0, midiUid);
		midiOut.latency = 0;

		// Initializations
		tapTimes = List[];
		tickperiod = (0.5 / 48);

		OSCdef(\trans_stop, { this.stop; }, path+/+'stop');
		OSCdef(\trans_start, { this.start; }, path+/+'start');
		OSCdef(\trans_toggle, { this.toggle; }, path+/+'toggle');
		OSCdef(\trans_tap, {|msg, time| this.tap(time) }, path+/+'tap');
		OSCdef(\trans_clear_tap, { this.clear_tap }, path+/+'clear_tap');
		OSCdef(\trans_set_bpm, {|msg| this.setBpm(msg[1]) },path+/+'set_bpm');

			// This is a bug right here. No validation of the subtraction
			// value
		OSCdef(\trans_tempo_add_bpm,
			{|msg| this.setBpm( this.getBpm()+msg[1] ) },
			path+/+'tempo_add_bpm');

			// This is a bug right here. No validation of the subtraction
			// value
		OSCdef(\trans_tempo_sub_bpm,
			{|msg| this.setBpm( this.getBpm()-msg[1] ) },
			path+/+'tempo_sub_bpm');

		//OSCdef(\trans_factor,{|msg| this.setFactor(msg[1]); },path+/+'factor');
		OSCdef(\trans_factor_power, {|msg| this.setFactorPower(msg[1]) },
			path+/+'factor_power');
	}

	// Conveinence Methods
	getBpm { ^( (1/(tickperiod*(2**(-1*factor))))*(60/48) ) }

	setBpm { |bpm|
		if((bpm >= 10) && (bpm <= 600)){
			tickperiod = (1 / ((bpm * 48)/60));
			bridge.sendMsg("/bridge/tempo", this.getBpm );
		}
	}

	setFactorPower { |fact|
		if( (fact==(-2)) || (fact==(-1)) || (fact==0) || (fact==1) || (fact==2))
		{
			new_factor = fact;
			bridge.sendMsg("/bridge/factor_power", new_factor );
		}
	}

	schedTickClock {
		SystemClock.sched((tickperiod * (2**(-1*factor))),
			{|ticktime|
				bridge.sendMsg("/tick", tick);
				otherPorts.do({|port| port.sendMsg("/tick", tick) });
				scPort.sendMsg("/tick", tick);

				tick = (tick+1)%48; //tick.ticktln;
				// Delay factor update until down beat
				if(tick == 0){
					if(factor != new_factor){
						factor = new_factor;
						bridge.sendMsg("/bridge/tempo", this.getBpm());
					}
				};

				// resetting the tap times list
				if(tapTimes.size > 0){
					if(ticktime > (2.5 + tapTimes[tapTimes.size-1]))
					{ tapTimes = List[] };
				};
				//if(midiOut.respondsTo('midiClock')){
				midiOut.midiClock;// };

				//(tickperiod * (2**(-1*factor))).postln;
				tickperiod * (2**(-1*factor));
		})
	}

	// The real meat

	start {
		on = true;
		tick = 0;
		SystemClock.clear;
		this.schedTickClock.value();
		//midiOut.start;
		bridge.sendMsg("/bridge/tempo", this.getBpm());
	}

	stop {
		on = false;
		SystemClock.clear;
		tapTimes = List[];
		//if(midiOut.respondsTo('stop')){ midiOut.stop };
		//midiOut.stop;
		bridge.sendMsg("/bridge/stop");
		otherPorts.do({|port| port.sendMsg("/dseq/stop") });
	}

	toggle { if(on){ this.stop }{ this.start } }

	tap { |time|
		var newPeriod;

		on = true;
		tapTimes.add(time);
		if(tapTimes.size > 8){ tapTimes.removeAt(0) };
		//get the lengths
		if(tapTimes.size > 1){
			var sum = 0;
			1.for(tapTimes.size-1, {|indx|
				sum = sum + (tapTimes[indx]-tapTimes[indx-1])});

			newPeriod = (sum / (tapTimes.size-1))/48;

			if((newPeriod < 0.125) &&  (newPeriod > 0.003))
			{
				tickperiod = newPeriod;
				//tickperiod.postln;
				bridge.sendMsg("/bridge/tempo", this.getBpm );
			}
			{ tapTimes = List[time] };
		};

		tick = 0;
		SystemClock.clear;
		this.schedTickClock.value();
	}

	clear_tap { tapTimes = List[] }

}
