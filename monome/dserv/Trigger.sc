
// This is how we make the synth, witch should be loaded already
//n = Synth(\dtrigger, [\bufnum, b]);

// This is how we trigger it.
//n.set(\t_trig, 1)


OldddTrigger {

	//classvar numMicronomes;
	const path = "/sc/trigger";
	const lPath = "/sc/trigger/grid/led";
	//const up_map = Array[0,0]
	//const down_map = Array[0,0]

	// State
	var show = false;
	var trans_press_count = 0;
	var show_map;
		// This structure is meant to make lighting up the monome easier by
		// avoiding redundant computations
var speed_selector;
		// This structure keeps track of the edge button speeds. the first
		// element in the array is the top left speed and the others follow
		// in a clockwise pattern

	// Osc/Midi
	var bridge;


	*new {|bridgePortNum = 8000|
		^super.new.init(bridgePortNum)
	}

	init {|bridgePortNum = 8000|
		bridge = NetAddr.new("localhost", bridgePortNum);

		// Data Structure Initialization
		speed_selector = Array[0,0,0,0,0,0,0];
		show_map = Array[7 + 2**7,0,0,0,0,4,2,1];

		// Osc Responder Registration
		OSCdef(\micronome_press,
			{|msg, time| msg.postln; this.press_responder(msg[1], msg[2], msg[3], time) },
			path +/+ 'grid/key');

		OSCdef(\micronome_hide, { show = false; }, path+/+'hide');
		OSCdef(\micronome_tick, {|msg| this.tick_responder(msg[1]) }, '/tick');
		OSCdef(\micronome_show, { show = true; this.show;}, path+/+'show');
	}


	/*
	 * Press Functions
	 */
	press_responder {|xPos, yPos, zPos, time|
		show = true;

		if( xPos >= 8 ){
			this.trans_press(xPos, yPos, zPos, time)
		}{
			if( zPos == 1 ){
				this.trigger_press(xPos, yPos)
			}
		}
	}

	trigger_press {|xPos, yPos|
		case
		{ (yPos > 4) && (xPos < 11) }
		{
			// 3 X 3 of non-repeating triggers
		}

		{ (yPos == 4) && (xPos < 12)}
		{
			// horizontal repeaters and the corner one
		}

		{ (yPos > 4) && (xPos == 11)}
		{
			// vertical repeaters
		}


		{ (yPos <= 2) && (xPos < 11) }
			// The top three speed setters

		{
		}

		{ (yPos > 4) && (xPos > 12) }
		{
			// The side speed setters
		}

		{ (yPos <= 2) && (xPos > 12) }
		{
			// The diagonal speed setter
			if( xPos == yPos){
			}
		}

	}

	trans_press {|xPos, yPos, time|
		case
		// Toggle
		//{ yPos == 0 }
		//{
		//}

		// Tap and Clear
		{ (yPos >= 1) && (yPos <= 4) }
		{
			if( xPos <= 3 )
			{ ~trans.clear_tap }
			{ ~trans.tap(time) }
		}

		// Exit
		{ (yPos == 7) && ( xPos == 0)}
		{ this.hide() }

	}

	/*
	 * Press helpers
	 */


	/*
	 * Other Functions
	 */
	transToggle {
		if( ~trans.on ){
			~trans.stop;
			bridge.sendMsg(lPath +/+ "map",0,0, 0,0,0,0,0,0,0,0)
		}{
			~trans.start;
		}
	}

	tick_responder{ |tick|
		case
		{ tick%48 == 0} // Down
		{
			bridge.sendMsg(lPath +/+ "map",0,0, 255, 240, 150,150, 240, 0,0,1)
		}
		{ tick%48 == 24} // Up
		{
			bridge.sendMsg(lPath +/+ "map",0,0, 255, 15, 105,105, 15, 0,0,1)
		}
	}

	/*
	 * Hide n show
	 */

	show { 
		bridge.sendMsg(lPath +/+ "map",8,0, show_map)
	}

	hide{
		show = false;
		bridge.sendMsg(path+/+"hide"); //sending a request to bridge to hide
		trans_press_count = 0;
	}

}
