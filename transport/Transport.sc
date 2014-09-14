Transport {

	//calssvar numtransports
	const path = "/sc/transport";

	var tapTimes; // = List[];
	var tickPeriod = 1;
	var factor = 1;
	var new_factor = 0;
	//var down_tick = 0;
	var <tick = 0;
	var <on = false;

	// fancy tap exp
	//var rate = 0;
	//var scale = 1;
	//var adj_remaining = 0; // adjusted ticks remaining.
	//var adj_total = 0;

	var bridge, otherPorts, scPort;
	//var midiOut;

	*new {|bridgePort = 8000 ... othPorts|
		^super.new.init(bridgePort, *othPorts);
	}

	init {|bridgePort = 8000 ... othPorts|
		bridge = NetAddr.new("localhost", bridgePort);
		scPort = NetAddr.new("localhost", 57120);
		otherPorts = othPorts.collect(
			{|port| NetAddr.new("localhost", port);}
		);
		//midiOut = MIDIOut(0, midiUid);
		//midiOut.latency = 0;

		// Initializations
		tapTimes = List[];
		tickPeriod = (1 / 96);

		//rate = 0.35787944117; // e^-1
		//rate = 0.13533528323; // e^-2	

		OSCdef(\trans_stop, { this.stop; }, path+/+'stop');
		OSCdef(\trans_start, { this.start; }, path+/+'start');
		OSCdef(\trans_toggle, { this.toggle; }, path+/+'toggle');
		OSCdef(\trans_tap, {|msg, time| this.tap(time) }, path+/+'tap');
		OSCdef(\trans_clear_tap, { this.clear_tap }, path+/+'clear_tap');
		OSCdef(\trans_set_bpm, {|msg| this.setBpm(msg[1]) },path+/+'set_bpm');

		// Validation happens in setBpm
		OSCdef(\trans_tempo_add_bpm,
			{|msg| this.setBpm( this.getBpm()+msg[1] ) },
			path+/+'tempo_add_bpm');

		OSCdef(\trans_tempo_sub_bpm,
			{|msg| this.setBpm( this.getBpm()-msg[1] ) },
			path+/+'tempo_sub_bpm');

		OSCdef(\trans_factor_power, {|msg| this.setFactorPower(msg[1]) },
			path+/+'factor_power');
	}

	// Conveinence Methods
	getBpm { ^( (1/(tickPeriod*(2**(-1*factor))))*(60/48) ) }

	setBpm { |bpm|
		if((bpm >= 10) && (bpm <= 600)){
			tickPeriod = (1 / ((bpm * 48)/60));
			bridge.sendMsg("/bridge/tempo", this.getBpm );
		}
	}

	setFactorPower { |fact|
		if( (fact==(-2)) || (fact==(-1)) || (fact==0) || (fact==1) || (fact==2)){
			new_factor = fact;
			bridge.sendMsg("/bridge/factor_power", new_factor );
		}
	}

	schedTickClock {
		SystemClock.sched((tickPeriod * (2**(-1*factor))),
			{|ticktime|
				var adjPeriod; // periodicity adjusted by the factor choice
				var delta = 0;
				case
				{tick == 0}
				{ 
					bridge.sendMsg("/transport/tick", tick); 
					//last_down_tick_time = ticktime;
				}
				{tick == 24}
				{ bridge.sendMsg("/transport/tick", tick) };

				scPort.sendMsg("/transport/tick", tick);
				otherPorts.do(
					{|port| port.sendMsg("/transport/tick", tick) });

				tick = (tick+1)%48; tick.postln;
				// The execution of this function marks the begining of the
				// tick that tick has just been set to.
				// Delay factor update until down beat
				if((tick == 0) && (factor != new_factor)){
					factor = new_factor;
					bridge.sendMsg("/bridge/tempo", this.getBpm());
				};
				
				// resetting the tap times list
				//if(tapTimes.size > 0){
					//if(ticktime > (2.5 + tapTimes[tapTimes.size-1]))
					//{ tapTimes = List[] };
				//};

				//if(adj_remaining > 0){
				//	// get the scale
				//	if(scale != 1){
				//		scale = ((tapTimes.last - ticktime) + (tickPeriod*tick));
				//		scale = (scale * (1 - rate));
				//		scale = (scale / adj_total);
				//	};
				//	delta = (scale * (rate**adj_remaining));
				//	// decrement the counter.
				//	adj_remaining = adj_remaining - 1;
				//	//delta.postln;
				//	//tickPeriod.postln;


				//}{
				//	scale = 1;
				//};


				//adjPeriod = tickPeriod * (2**(-1*factor));
				//adjPeriod = (tickPeriod + (scale * (rate**tick)));
				(tickPeriod + delta).postln;
				adjPeriod = (tickPeriod + delta);
		})
	}

	// The real meat

	start {
		on = true;
		SystemClock.clear;
		this.schedTickClock.value();
		//midiOut.start;
		bridge.sendMsg("/bridge/tempo", this.getBpm());
	}

	stop {
		on = false;
		SystemClock.clear;
		tapTimes = List[];
		//tick = down_tick;

		//midiOut.stop;
		bridge.sendMsg("/bridge/stop");
		otherPorts.do({|port| port.sendMsg("/dtrig/stop") });
	}

	toggle { if(on){ this.stop }{ this.start } }

	tap { |time|
		// first add time to the list appropriately
		if(tapTimes.size != 0)
		{
			//If the taps are old, clear em out
			if( (tapTimes.size >= 1) &&
				(time - tapTimes.last) > (3 * 48 * tickPeriod) )
			{ tapTimes = List[time]; }

			// Just adding on to a current list.
			{ tapTimes.add(time); };

			if(tapTimes.size > 8){ tapTimes.removeAt(0) };

			// Now calculate tempo
			if(tapTimes.size >= 2){
				var newPeriod = 0;

				// Sum of the differences
				1.for(tapTimes.size-1, {|indx|
					newPeriod = (newPeriod + (tapTimes[indx]-tapTimes[indx-1]))
				});

				// Average and scale
				newPeriod = ((newPeriod / (tapTimes.size-1))/48);
	
				if((newPeriod < 0.125) &&  (newPeriod > 0.003)){
					tickPeriod = newPeriod;
					//tickPeriod.postln;
					bridge.sendMsg("/bridge/tempo", this.getBpm );
				};
			}
		}

		// the list is empty, we're starting a new beat.
		{ tapTimes.add(time) };
		tapTimes.postln;

		on = true; // This is here just cuz we needed the old state earlier.

		tick = 0;
		//if(tick < 22,{ // we're thinking about the next tick
		//	adj_remaining = (48 - (tick + 1));
		//	//adj_total = (48 - (tick + 1));
		//	adj_total = ((rate**(tick + 1)) - (rate**48));

		//	// calculate the scale in the tick handler, b/c we need tick time

		//}
		//,{
			//adj_remaining = (96 - (tick + 1));
			//adj_total = ((rate**(tick + 1)) - (rate**96));
		//}
		//);

		// Let 'er rip
		// The clock is restarted on every single tap
		SystemClock.clear;
		this.schedTickClock.value();
	}

	clear_tap { tapTimes = List[] }

}
