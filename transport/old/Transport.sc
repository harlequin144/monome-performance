Transport {

	const path = "/sc/transport";
	const lpath = "/sc/transport/grid/led";

	// Show state
	var show_cnt = 0;
	var ctl_press_cnt = 0;
	var tempo_press_cnt = 0;
	var right_mask; // to remember the state of the right side leds.

	// Transport state
	var tapTimes;		// List of times when taps happened
	var tickPeriod = 1;
	var factor = 1;
	var new_factor = 0;
	var <tick = 0;
	var <on = false;

	// Tempo selection
	var last_tempo_press;
	var tempo_pair_list;

	// fancy tap exp
	//var rate = 0;
	//var scale = 1;
	//var adj_remaining = 0; // adjusted ticks remaining.
	//var adj_total = 0;

	// Communication
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
		last_tempo_press = [0,0];
		tempo_pair_list = Set[[0,0]];
		right_mask = List[0,0,0,0,0,0,0];

		//rate = 0.35787944117; // e^-1
		//rate = 0.13533528323; // e^-2

		// Basic functionality responders
		OSCdef(\trans_stop, 	{ this.stop }, path+/+'stop');
		OSCdef(\trans_start, 	{ this.start }, path+/+'start');
		OSCdef(\trans_toggle, 	{ this.toggle }, path+/+'toggle');
		OSCdef(\trans_clear_tap,{ this.clearTap }, path+/+'clear_tap');

		OSCdef(\trans_tap,
			{ |msg, time|
				this.tap(time)
			},
			path+/+'tap');

		// Monome Related Responders

		OSCdef(\trans_button_press,
			{|msg, time|
				if(msg[3] == 1)
				{ this.pressResponder(msg[1], msg[2], time) }
				{ this.releaseResponder(msg[1], msg[2]) }
			},
			path+/+'grid/key');

		OSCdef(\trans_show,
			{
				show_cnt = show_cnt + 1;
				this.show; //"showing".postln;
			},
			path+/+'show');

		OSCdef(\trans_hide,
			{
				show_cnt = show_cnt - 1;
				if(show_cnt < 0){ show_cnt = 0 };
			},
			path+/+'hide');
	}


	/*
	 * 	Basic Functions
	 */

	start {
		on = true;
		SystemClock.clear;
		this.schedTickClock.value();
		//midiOut.start;
	}

	stop {
		on = false;
		SystemClock.clear;
		tapTimes = List[];
		//midiOut.stop;
		//otherPorts.do({|port| port.sendMsg("/dtrig/stop") });
	}

	toggle {
		if(on){
			this.stop;
			bridge.sendMsg(lpath+/+'map',0,0, 0,0,0,0,51,51,12,12)
		}
		{ this.start }
	}

	tap { |time|
		// first add time to the list appropriately
		if(tapTimes.size != 0)
		{
			//If the taps are old, clear em out
			if( (tapTimes.size >= 1) &&
				(time - tapTimes.last) > (3 * 144 * tickPeriod) )
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
				newPeriod = ((newPeriod / (tapTimes.size-1))/144);
				this.setPeriod(newPeriod);

			};

			this.setRightMask;
		}

		// the list is empty, we're starting a new beat.
		{ tapTimes.add(time) };
		//tapTimes.postln;

		on = true; // This is here just cuz we needed the old state earlier.

		tick = 0;
		//if(tick < 22,{ // we're thinking about the next tick
		//	adj_remaining = (144 - (tick + 1));
		//	//adj_total = (144 - (tick + 1));
		//	adj_total = ((rate**(tick + 1)) - (rate**144));

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

	clearTap { tapTimes = List[] }


	/*
	 * Conveinence Methods
	 */

	getBpm { ^( (1/(tickPeriod*(2**(-1*factor))))*(60/144) ) }

	setBpm { |bpm|
		if((bpm >= 10) && (bpm <= 600))
		{ tickPeriod = (1 / ((bpm * 144)/60)) };

		this.setRightMask;
	}

	setPeriod {|per|
		if((per < 0.125) &&  (per > 0.003))
		{ tickPeriod = per };
		//tickPeriod.postln;

		this.setRightMask;
	}

	setRightMask {
		// Fine grain adjustment lighting
		right_mask =
			[128+(2**(((this.getBpm%10).div(2))+1)-1), 0,0,0,0,0,0,0];

		//Now light things in an interesting way
		tempo_pair_list = Set[];
		for(0, min(this.getBpm().div(10), 56),
			{
				var pair = [exprand(8,16).div(1), exprand(1,8).div(1)];
				while({tempo_pair_list.includes(pair)},
					{ pair = [exprand(8,16).div(1), exprand(1,8).div(1)] }
				);
				tempo_pair_list.add(pair);
			}
		);

		tempo_pair_list.do({|pair|
			right_mask[pair[1]] =
				(right_mask[pair[1]] + (2**(pair[0] - 8)));
		});
		//right_mask.postln;
		this.lightRightSide;
	}

	setFactorPower { |fact|
		if( ((fact+2) >= 0) && ((fact+2) <= 4) ){ new_factor = fact }
	}


	/*
	 * The main clock loop
	 *
	 */

	schedTickClock {
		SystemClock.sched((tickPeriod * (2**(-1*factor))),
			{|ticktime|
				var adjPeriod; // periodicity adjusted by the factor choice
				var delta = 0;

				if(show_cnt > 0){
					case
					{tick == 0}{
						bridge.sendMsg(lpath+/+'map',0,0,
							63,56,42,56,51,51,12,12);
					}
					{tick == 72}{
						bridge.sendMsg(lpath+/+'map', 0,0,
							63,7,21,7,51,51,12,12);
					};
				};

				scPort.sendMsg("/transport/tick", tick);
				otherPorts.do(
					{|port| port.sendMsg("/transport/tick", tick) }
				);


				tick = (tick+1)%144; //tick.postln;
				// The execution of this function marks the begining of the
				// tick that tick has just been set to.
				// Delay factor update until down beat
				if((tick == 0) && (factor != new_factor))
				{ factor = new_factor };

				// resetting the tap times list
				if(tapTimes.size > 0){
					if(ticktime > (2.5 + tapTimes[tapTimes.size-1]))
					{ tapTimes = List[] };
				};

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
				//(tickPeriod + delta).postln;
				adjPeriod = (tickPeriod + delta);
		})
	}



	/*
	 *	functions for interaction with the monome
	 *
 	 */

	pressResponder {|xPos, yPos, time|
		case
		{(xPos >= 0) && (xPos < 6)}{
			case
			//toggle
			{yPos == 0}{
				ctl_press_cnt = ctl_press_cnt + 1;
				if(ctl_press_cnt == 1)
				{ this.toggle }
			}

			// Tap and clear
			{(yPos >= 1) && (yPos < 4)}{
				if(xPos < 3)
				{ this.clearTap; }{
					ctl_press_cnt = ctl_press_cnt + 1;
					if(ctl_press_cnt == 1){ this.tap(time) }
				}
			}

			// nudging
			{(yPos == 5) || (yPos == 4)}{
				case
				{ (xPos == 0) || (xPos == 1) }
				{ this.setBpm( (this.getBpm() - (3 - xPos)) ) }

				{ (xPos == 4) || (xPos == 5) }
				{ this.setBpm( (this.getBpm() + (xPos - 2)) ) }
			}

			// factor
			//{(yPos == 6) || (yPos == 7)}{ this.setFactorPower(xPos) }
		}

		// Presses on the right side
		{(xPos >= 8) && (xPos < 16)}{
			case
			{yPos == 0}{
				case
				// Exit button
				{xPos == 15}
				{ this.hide }

				// Fine tempo adjustment
				//{}
			}

			//{yPos == 0}
			{(yPos > 0) && (yPos <= 7)}{
				tempo_press_cnt = tempo_press_cnt + 1;

				case
				{tempo_press_cnt == 1}{
					last_tempo_press = [xPos, yPos];
					tempo_pair_list.add([xPos,yPos]);

					bridge.sendMsg(lpath +/+ 'set', xPos,yPos, 1);
					this.setBpm((tempo_pair_list.size() *10));
				}

				{tempo_press_cnt == 2}{
					var lastX = last_tempo_press[0];
					var lastY = last_tempo_press[1];
					var mask_num = 0;

					tempo_pair_list = Set[];

					// find the right number for setting the lighting mask
					right_mask = [128,0,0,0,0,0,0,0];
					for(lastX, xPos,{|x| mask_num = mask_num + (2**(x - 8))});

					// add pairs to set of pairs and set the mask rows
					for(lastY, yPos, {|y|
						for(lastX, xPos,{|x| tempo_pair_list.add([x,y]) });
						right_mask[y] = mask_num;
					});

					//last_tempo_press = [xPos, yPos];
					this.setBpm((tempo_pair_list.size() *10));
					this.lightRightSide;
				}
			}

		}
	}

	releaseResponder {|xPos, yPos|
		case
		{
			((xPos >= 0) && (xPos < 6) && (yPos == 0)) || 	// Toggle
			((xPos >= 3) && (xPos < 6) && (yPos >= 1) && (yPos < 4))  // Tap
		}
		{
			ctl_press_cnt = ctl_press_cnt - 1;
			if(ctl_press_cnt < 0) { ctl_press_cnt = 0 }
		}

		// Presses on the right side
		{(xPos >= 8) && (xPos < 16) && (yPos > 0) && (yPos < 8)}
		{
			tempo_press_cnt = tempo_press_cnt - 1;
			if(tempo_press_cnt < 0) { tempo_press_cnt = 0 }
		}
	}

	show {
		show_cnt.postln;
		if(show_cnt > 0){
			//let the main loop take care of lighting the blinking section
			bridge.sendMsg(lpath +/+ 'map', 0,0, 0,0,0,0,51,51,12,12);

			this.lightRightSide;
		}
	}

	// For showing convienience
	lightRightSide {
		if(show_cnt > 0){
			bridge.sendMsg(lpath +/+ 'map', 8,0,
				right_mask[0], right_mask[1], right_mask[2], right_mask[3],
				right_mask[4], right_mask[5], right_mask[6], right_mask[7])
		}
	}


	hide {
		show_cnt = 0;
		bridge.sendMsg(path +/+ "hide");
		ctl_press_cnt = 0;
		tempo_press_cnt = 0;
	}
}
