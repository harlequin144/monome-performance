Mono {

	//classvar numMicronomes;
	const path = "/sc/mono";
	const lPath = "/sc/mono/grid/led";

	// State
	var show_cnt = 0;
	var hold = false;
	var ctrl_press_cnt = 0;

	// Notes
	var pitchRange = 12;
	var noteStack;
	// This keeps track of what is being pressed on the monome.
	// Despite anything else, this stack keeps track of what is being pressed
	// at any time. Also, when a note is added, a note on midi mesasge is
	// sent. When one is removed a note off must also be sent (except when the
	// hold is on).
	var holdNote = 0; // Only one note on at a time

	// Osc/Midi
	var bridge;
	var midiOut; // Keeps track of what notes are on microbrute-side


	*new {|bridgePortNum = 8000, uid|
		^super.new.init(bridgePortNum, uid)
	}

	init {|bridgePortNum = 8000, uid|
		bridge = NetAddr.new("localhost", bridgePortNum);
		midiOut = MIDIOut.new(1);
		midiOut.connect(uid);
		
		midiOut.latency = 0;

		// Data Structure Initialization
		noteStack = LinkedList[];
		//holdSet = Set[];

		midiOut.control(0,105, 127);  // Seq Play hold to off
		midiOut.control(0, 109,0);
		midiOut.sysex(Int8Array[16rB0, 16r65, 16r0, 16rB0, 16r64, 16r0,
			16rB0, 16r06, pitchRange]);

		// Osc Responder Registration
		OSCdef(\mono_press,
			{|msg, time|
				case
				{msg[3] == 1}{ this.pressResponder(msg[1], msg[2], time) }
				{msg[3] == 0}{ this.releaseResponder(msg[1], msg[2]) }
			},
			path +/+ 'grid/key');

		OSCdef(\mono_tick, 
			{|msg| 
				case
				{ msg[1] == 0 }{ this.trans_light_dn; }
				{ msg[1] == 24}{ this.trans_light_up; }
			}, '/transport/tick');

		OSCdef(\mono_hide,
			{
				show_cnt = show_cnt - 1;	//"show decreased".postln;
				if(show_cnt < 0){show_cnt = 0}
			}, path+/+'hide');

		OSCdef(\mono_show,
			{
				show_cnt = show_cnt + 1;	//"show increased".postln;
				this.show;
			}, path+/+'show');

		OSCdef(\mono_hold,
			{|msg|
				case
				{msg[1] == 0}{ this.holdEngage; }
				{msg[1] == 1}{ this.holdRelease; }
			}, path+/+'hold');


		this.show();
	}

	/*
	 * Press Functions
	 */

	pressResponder {|xPos, yPos, time|
		if( xPos < 8)
		{ show_cnt = 1 }
		{ show_cnt = 2 };
		//case
		//Mod or key press
		if( (xPos >= 4) && (yPos > 0) ){
			this.notePress(xPos, yPos)
		}{
			ctrl_press_cnt = ctrl_press_cnt + 1;
			if( ctrl_press_cnt == 1){
				case
				{ (yPos == 0) && (xPos >= 4) }
				{ this.setMod(xPos-3) }

				// Exit
				{ (yPos == 0) && (xPos < 2) }
				{ this.hide }

				//toggle trans
				{ yPos == 0 }{ this.transToggle } // On Off

				// Tap and Clear
				{ (yPos == 1) || (yPos == 2) }
				{ this.clearTapPress(xPos, time) }

				// Hold
				//{ yPos == 7 }{ this.holdToggle() }
			}
		};
		if(yPos >= 8){
			"got a press that is bigger than normal".postln;
			yPos.postln;
		}
	}

	releaseResponder {|xPos, yPos|
		if( (xPos >= 4) && (yPos >= 1) ){
			this.noteRelease(xPos, yPos);
		}{
			ctrl_press_cnt = ctrl_press_cnt - 1;
			if(ctrl_press_cnt < 0){ ctrl_press_cnt = 0 }
		}
	}


	/*
	 * Press helpers
	 */

	notePress { |xPos, yPos| //|time|
		var note = (12*yPos) + (xPos-4);

		if(noteStack.size() > 0)
		{ this.noteOff(noteStack.last()) };

		noteStack.add(note);
		this.noteOn(note);

		if(hold){
			this.noteOff(holdNote);
			holdNote = note;
		}
	}

	noteRelease {|xPos, yPos|
		var note = (12*yPos) + (xPos-4);

		if(note == noteStack.last()){
			if(noteStack.size > 1){// guearanteed to not be 0 here 
				this.noteOff(note);
				noteStack.remove(note);
				this.noteOn(noteStack.last());
			}{
				noteStack.remove(note);
				if(hold)
				{ holdNote = note }
				{ this.noteOff(note) }
			}
		}{ 
			noteStack.remove(note) 
		}
	}

	/*
	 * Other Functions
	 */

	transToggle {
		if( ~trans.on ){
			~trans.stop;
			bridge.sendMsg(lPath +/+ "set", 2,0,0);
			bridge.sendMsg(lPath +/+ "set", 3,0,0);
			bridge.sendMsg(lPath +/+ "row",0,1, 80,171);
			bridge.sendMsg(lPath +/+ "row",0,2, 0, 0);
		}{
			~trans.start;
			bridge.sendMsg(lPath +/+ "set", 2,0,1);
			bridge.sendMsg(lPath +/+ "set", 3,0,1);
		}
	}

	clearTapPress {|xPos, time|
		if(xPos < 2){
			~trans.clear_tap
		}{
			~trans.tap(time);
			if(~trans.on){
				bridge.sendMsg(lPath+/+"set", 2,0,1);
				bridge.sendMsg(lPath+/+"set", 3,0,1);
			}
		}
	}

	setMod { |val| // enter the value that you want it to be changed to, 1-12
		if((val >= 0) && (val <= 12)){
			pitchRange = val;
			midiOut.sysex(Int8Array[16rB0, 16r65, 16r0, 16rB0, 16r64, 16r0,
				16rB0, 16r06, val]);
			bridge.sendMsg(lPath +/+ "row", 0,0,
				if(val<4) {1 + (16*( (2**val)-1 ))} {241},
				if(val<4) {0} { 2**(val-4)-1} );
		}
	}

	holdEngage {
		hold = true;
		midiOut.noteOff(0, holdNote);
		bridge.sendMsg(lPath +/+ "row",0,5, 80,171);
		bridge.sendMsg(lPath +/+ "row",0,6, 2, 0);
		bridge.sendMsg(lPath +/+ "row",0,7, 80,171);
	}

	holdRelease {
		hold = false;
		midiOut.noteOff(0, holdNote);
		bridge.sendMsg(lPath +/+ "row",0,5, 87,171);
		bridge.sendMsg(lPath +/+ "row",0,6, 5, 0);
		bridge.sendMsg(lPath +/+ "row",0,7, 87,171);
	}

	trans_light_up{
		if(show_cnt > 0){
			bridge.sendMsg(lPath +/+ "row",0,1, 83,171);
			bridge.sendMsg(lPath +/+ "row",0,2, 3,0);
		}
	}

	trans_light_dn{
		if(show_cnt > 0){
			bridge.sendMsg(lPath +/+ "row",0,1, 92,171);
			bridge.sendMsg(lPath +/+ "row",0,2, 12,0);
		}
	}

	/*
	 * Hide n show
	 */
	show { // Light up the monome
		if( show_cnt > 0){

			var hold_mask_mid = if(hold){2}{5};
			var hold_mask_out = if(hold){0}{7};

			var trans_mask = if(~trans.on){12}{0};
			var mod_range_mask = 
		
				if(pitchRange >=5 ){241}
				{ 1+(16*((2**pitchRange)-1)) };

			// Left side
			bridge.sendMsg(lPath +/+ "map", 0,0,
				mod_range_mask + trans_mask,
				80, 0, 80, 80,
				80 + hold_mask_out, 
				0 + hold_mask_mid, 
				80 + hold_mask_out
			);

			// Right Side
			bridge.sendMsg(lPath +/+ "map",8,0,
				if(pitchRange >=5 ){ 2**(pitchRange-4)-1 }{0},
				171,0,171,171,171,0,171);

		};
		//if(show_cnt > 1){
			//bridge.sendMsg(lPath +/+ "map",8,8,
				//0, 171,0,171,171,171,0,171);
		//}
	}

	hide{
		show_cnt = 0;
		bridge.sendMsg(path+/+"hide"); //sending a request to bridge to hide
		ctrl_press_cnt = 0;
		{noteStack.size > 0}.while(
			//{ midiOut.noteOff(0, noteStack.pop()) }
			{ this.noteOff(noteStack.pop()) }
		);
	}

	/*
	 * Note Management
	 */
     
	noteOff { |note| midiOut.noteOff(0,note) }
	noteOn { |note| midiOut.noteOn(0,note) }

	killallnotes { (1..150).do({|note| midiOut.noteOff(0, note) }); }
}
