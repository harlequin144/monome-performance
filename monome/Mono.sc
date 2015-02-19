Mono {
	const path = "/sc/mono";
	const lPath = "/sc/mono/grid/led";

	// State
	var show_cnt = 0;
	var ctrl_press_cnt = 0;

	var pitchRange = 12;

	var bridge;
	var transport;
	var midiOut; // Keeps track of what notes are on microbrute-side



	*new {|bridgePortNum = 8000, midi|
		^super.new.init(bridgePortNum, midi)
	}

	init {|bridgePortNum = 8000, midi|
		bridge = NetAddr.new("localhost", bridgePortNum);

		midiOut = midi;

		midiOut.control(0,105, 127);  // Seq Play hold to off
		midiOut.control(0, 109,0);
		midiOut.sysex(Int8Array[16rB0, 16r65, 16r0, 16rB0, 16r64, 16r0,
			16rB0, 16r06, pitchRange]);

		// Osc Responder Registration
		OSCdef(\mono_press,
			{|msg, time|
				case
				{ msg[3] == 1}{ this.pressResponder(msg[1], msg[2], time) }
				{ msg[3] == 0}{ this.releaseResponder(msg[1], msg[2]) }
			}, path +/+ 'grid/key');

		OSCdef(\mono_hide,
			{
				show_cnt = show_cnt - 1;
				//"show decreased".postln; show_cnt.postln;
				if(show_cnt < 0){show_cnt = 0}
			}, path+/+'hide');

		OSCdef(\mono_show,
			{
				show_cnt = show_cnt + 1;
				//"show increased".postln; show_cnt.postln;
				if( show_cnt > 0){ this.show; };
			}, path+/+'show');

		this.show();
	}


	/*
	 * Press Functions
	 */

	pressResponder {|xPos, yPos, time|
		show_cnt = 1;

		if( xPos < 4 ){
			ctrl_press_cnt = ctrl_press_cnt + 1;
			if( ctrl_press_cnt == 1){
				if( xPos == 0 ){
					case
					{ yPos == 0 } { this.hide }
					{ yPos == 7 } {}
				}{
					case
					{ (yPos >= 4) && (yPos <= 6) } { this.sustainToggle() }
					{ (yPos >= 1) && (yPos <= 3) } { }
				}
			}
		}{
			if( yPos == 0 )
			{ this.setMod(xPos-3) }
			{ this.notePress(xPos, yPos) }
		};

		if( (yPos >= 8) || (yPos < 0) || (xPos < 0) || (xPos >= 16) )
		{ "got a bad press".postln; xPos.postln; yPos.postln; }
	}


	/*
	 * Press helpers
	 */
	releaseResponder {|xPos, yPos|
		if( (xPos >= 4) && (yPos >= 1) )
		{ this.noteRelease(xPos, yPos); }
		{
			ctrl_press_cnt = ctrl_press_cnt - 1;
			if(ctrl_press_cnt < 0){ ctrl_press_cnt = 0 }
		}
	}

	notePress { |xPos, yPos| //|time|
		var note = (12*yPos) + (xPos-4);
		midiOut.noteOn(note);
	}

	noteRelease {|xPos, yPos|
		var note = (12*yPos) + (xPos-4);
		midiOut.noteOff(note);
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

	sustainToggle {
		if( midiOut.sustaining ) 
		{ midiOut.sustainOff; } 
		{ midiOut.sustainOn; };

		this.showLeft;
	}

	show { 
		this.showLeft;
		this.showRight;
	}

	showLeft { 
		bridge.sendMsg(lPath +/+ "map", 0,0,
			1 + if(pitchRange >= 5){240}{ 16*((2**pitchRange)-1) },

			0, //80, 
			84, //4, 
			0, //80, 

			//sustain
			80 + if(midiOut.sustaining){14}{0}, 
			80 + if(midiOut.sustaining){10}{4},
			0 + if(midiOut.sustaining){14}{0},
			81
			);
	}

	showRight { 
		bridge.sendMsg(lPath +/+ "map",8,0,
			(pitchRange >= 5).if{ 2**(pitchRange-4)-1 }{0},
			0, 171,0,171,171,0,171);
	}

	hide{
		show_cnt = 0;
		bridge.sendMsg(path+/+"hide"); //sending a request to bridge to hide
		ctrl_press_cnt = 0;
		midiOut.killNotesOn();
	}

}
