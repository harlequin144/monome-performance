// This is meant to be an implementation of an isomorphic keyboard on the
// monome. I will also be inteded to work with polyphonic synths so as to
// capitalize on the enhanced chord making nature of isomorphc keyboards.
// Currently it will also be use mainly to control a software synth. The
// monome will be set up with the actuall keyboard on the right 8 X 8 square
// and the left swuare will be devoted to controls. Controls are not yet
// decided upon, but they could include the following:
// -	transport control
// -	vibrato/pitch shifter
// -	mod wheel roller


// When both monomes are devoted to this program, it could be that the two
// send midi to two different channels, so that they could control two synths
// at once.


Isochord {

	//classvar numMicronomes;
	const path = "/sc/isochord";
	const lPath = "/sc/isochord/grid/led";

	// State
	var show_cnt = 0;
	//var ctrl_press_cnt = 0;

	// Notes
	//var pitchRange = 12;
	//var notesOn;
	// This keeps track of noteons that have been sent via the midiout. In
	// this program, everytime a note on is sent it is added to this
	// structure, so that we can keep perfect track of what has been sent. 

	// Osc/Midi
	var bridge;
	var midiOut; // Keeps track of what notes are on microbrute-side


	*new {|midiUid, midiChannel, bridgePortNum = 8000|
		^super.new.init(midiUid, midiChannel, bridgePortNum)
	}

	init {|midiUid, midiChannel = 0, bridgePortNum = 8000|
		bridge = NetAddr.new("localhost", bridgePortNum);
		midiOut = MIDIOut(midiChannel, midiUid);
		midiOut.latency = 0;

		// Data Structure Initialization
		//notesOn = Set[];

		// Osc Responder Registration
		OSCdef(\isochord_button,
			{|msg, time|
				if(msg[3] == 1)
				{ this.press_responder(msg[1], msg[2]) }
				{ this.release_responder(msg[1], msg[2]) }
			},
			path +/+ 'grid/key');

		//OSCdef(\isochord_tick, {|msg| this.tickResponder(msg[1]) }, '/tick');

		OSCdef(\isochord_hide, 
			{ 
				show_cnt = show_cnt - 1; 
				"show decreased".postln;
				show_cnt.postln;
				if(show_cnt < 0){show_cnt = 0} 
			}, 
			path+/+'hide');

		OSCdef(\isochord_show, 
			{ 
				show_cnt = show_cnt + 1; 
				"show increased".postln;
				show_cnt.postln;
				this.show;
			}, 
			path+/+'show');

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

				// Record and Play
				{ (yPos == 3) || (yPos == 4) }{
					if(xPos < 2)
					{ this.recSeqButtonPress }
					{ this.playSeqButtonPress }
				}
				// Pattern Select
				{ yPos == 5 }{ this.selectPattern(xPos) }

				// Pattern Note Play Speed
				{ yPos == 6 }{ this.setSeqPlaySpeed(xPos) }

				// Hold
				{ yPos == 7 }{ this.holdToggle() }
			}
		};
		if(yPos >= 8){
			"got a press that is bigger than normal".postln;
			yPos.postln;
		}
	}


	/*
	 * Press helpers
	 */
	releaseResponder {|xPos, yPos|
		if( (xPos >= 4) && (yPos >= 1) ){
			this.noteRelease(xPos, yPos);
		}{
			ctrl_press_cnt = ctrl_press_cnt - 1;
			if(ctrl_press_cnt < 0){ ctrl_press_cnt = 0 }
		}
	}

	notePress { |xPos, yPos| //|time|
		var note = (12*yPos) + (xPos-4);
		if(hold && noteStack.size == 1){
			if(noteStack.last() < 0){ noteStack.pop() }
		};
		// This is the main purpose of this function, operations specific to
		// states are handled in the cases below.
		noteStack.add(note);
	}

	noteRelease {|xPos, yPos|
		var note = (12*yPos) + (xPos-4);
		var currentlyPlayingNote = noteStack.last();
		noteStack.remove(note);

	}

	/*
	 * Other Functions
	 */

	//clearTapPress {|xPos, time|
	//	if(xPos < 2){
	//		~trans.clear_tap
	//	}{
	//		~trans.tap(time);
	//		if(~trans.on){
	//			bridge.sendMsg(lPath+/+"set", 2,0,1);
	//			bridge.sendMsg(lPath+/+"set", 3,0,1);
	//		}
	//	}
	//}


	//transToggle {
	//	if( ~trans.on ){
	//		~trans.stop;
	//		bridge.sendMsg(lPath +/+ "set", 2,0,0);
	//		bridge.sendMsg(lPath +/+ "set", 3,0,0);
	//		bridge.sendMsg(lPath +/+ "row",0,1, 80,171);
	//		bridge.sendMsg(lPath +/+ "row",0,2, 0, 0);
	//		if(seqState == 2){ this.killNotesOn() };
	//	}{
	//		~trans.start;
	//		bridge.sendMsg(lPath +/+ "set", 2,0,1);
	//		bridge.sendMsg(lPath +/+ "set", 3,0,1);
	//	}
	//}

	//setMod { |val| // enter the value that you want it to be changed to, 1-12
	//	if((val >= 0) && (val <= 12)){
	//		pitchRange = val;
	//		midiOut.sysex(Int8Array[16rB0, 16r65, 16r0, 16rB0, 16r64, 16r0,
	//			16rB0, 16r06, val]);
	//		bridge.sendMsg(lPath +/+ "row", 0,0,
	//			if(val<4) {1 + (16*( (2**val)-1 ))} {241},
	//			if(val<4) {0} { 2**(val-4)-1} );
	//	}
	//}


	/*
	 * Hide n show
	 */
	show { // Light up the monome
		var seqFact = if(seqState == 2){3}{seqState};
		var pitchG4 = (pitchRange >=5 );
		var selState = (selectedSeq <2);
		if( show_cnt > 0){
			// Left side
			bridge.sendMsg(lPath +/+ "map", 0,0,
				( pitchG4.if{241}{ 1+(16*((2**pitchRange)-1)) } ) +
				( if(~trans.on){12}{0} ),
				80, 0, // Tap and clear - let tick handle update this
				80 + (4*seqFact), // Play and Record
				80 + (4*seqFact),
				80 + (2** selectedSeq), // Select Seq
				2**seqPlaySpeed,//Playback speed
				83 + (9*hold.if{1}{0}) // Hold
			);
			// Right Side
			bridge.sendMsg(lPath +/+ "map",8,0,
				(pitchG4).if{ 2**(pitchRange-4)-1 }{0},
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
		if(hold == false){
			this.killNotesOn();
			{noteStack.size > 0}.while({noteStack.pop()});
		};
	}

	/*
	 * Note Management
	 */
	//noteOn {|num|
	//	midiOut.noteOn(0, num);
	//	notesOn.add(num);
	//}

	//noteOff {|num|
	//	midiOut.noteOff(0, num);
	//	notesOn.remove(num);
	//	//notesOn.postln;
	//	//("sent note off: " +num).postln;
	//	//noteStack.remove(num);
	//}

}
