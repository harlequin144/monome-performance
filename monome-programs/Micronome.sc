
// THink about merging some of the press counters. try to prevent accidntal
// hits... Will have to keep keys and control section separate tho..

Micronome {

	//classvar numMicronomes;
	const path = "/sc/micronome";
	const lPath = "/sc/micronome/grid/led";

	// State
	//var show = false;
	var show_cnt = 0;
	var hold = false;
	var ctrl_press_cnt = 0;

	// Notes
	var pitchRange = 12;
	var noteStack;
	// This keeps track of what is being pressed on the monome.
	// - When the hold is off, the stack contains the notes that are being
	//   pressed in order that they were pressed.
	// - When the hold is on, the stack behaves the same except when the
	//   last note is released, it is replaced on the stack but as a
	//   negative number to represent that it is not being pressd but was
	//   the last note that was pressed.
	var notesOn;
	// This keeps track of noteons that have been sent via the midiout. In
	// this program, note ons may only ever be sent with this function, so that
	// we can keep perfect track of what has been sent. Note that this data
	// structure will work closely with the noteStack, but does not correspond
	// exactlly with it. notesOn tracks micronome side, noteStack is
	// supercollider side (lang).

	//Sequencer
	var seq;
	var seqState = 0; //0 off, 1 recording, and else is playing
	var selectedSeq = 0; // thru 3
	var seqPos = 0;
	var recordSilently = true;
	var seqPlaySpeed = 0;

	// Osc/Midi
	var bridge;
	var midiOut; // Keeps track of what notes are on microbrute-side


	*new {|midiUid, bridgePortNum = 8000|
		^super.new.init(midiUid, bridgePortNum)
	}

	init {|midiUid, bridgePortNum = 8000|
		bridge = NetAddr.new("localhost", bridgePortNum);
		//monome = NetAddr.new("localhost", monomePortNum);
		midiOut = MIDIOut(0, midiUid);
		midiOut.latency = 0;

		// Data Structure Initialization
		noteStack = LinkedList[];
		notesOn = Set[];
 		seq = Array[
			LinkedList[60],LinkedList[60],LinkedList[60],LinkedList[60] ];
		midiOut.control(0,105, 127);  // Seq Play hold to off
		midiOut.sysex(Int8Array[16rB0, 16r65, 16r0, 16rB0, 16r64, 16r0,
			16rB0, 16r06, pitchRange]);

		// Osc Responder Registration
		OSCdef(\micronome_press,
			{|msg, time|
				if(msg[3] == 1)
				{ this.pressResponder(msg[1], msg[2], time) }
				{ this.releaseResponder(msg[1], msg[2]) }
			},
			path +/+ 'grid/key');

		OSCdef(\micronome_tick, {|msg| this.tickResponder(msg[1]) }, '/tick');

		OSCdef(\micronome_hide, 
			{ 
				show_cnt = show_cnt - 1; 
				"show decreased".postln;
				show_cnt.postln;
				if(show_cnt < 0){show_cnt = 0} 
			}, 
			path+/+'hide');

		OSCdef(\micronome_show, 
			{ 
				show_cnt = show_cnt + 1; 
				"show increased".postln;
				show_cnt.postln;
				this.show;}, 
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

		case
		{ seqState == 0 }{ 	//free mode
			this.killNotesOn();
			this.noteOn(note);
		}

		{ seqState == 1 }{ // record mode
			this.killNotesOn();
			if(recordSilently == false){ this.noteOn(note) };
			if(seqPos == 0){
				seq[selectedSeq] = LinkedList[noteStack.last()];
				seqPos = seqPos + 1;
			}{
				seq[selectedSeq].add(noteStack.last()-seq[selectedSeq].first);
				seqPos = seqPos + 1;
			};
		}

		{ seqState > 1 }{ //Sequencer play mode
			if(~trans.on == false){
				this.killNotesOn();
				if(seqPos == 0)
				{ this.noteOn(noteStack.last()) }
				{ this.noteOn(noteStack.last() + seq[selectedSeq][seqPos]) };
				seqPos = (seqPos + 1)%seq[selectedSeq].size;
			}
		}
	}

	noteRelease {|xPos, yPos|
		var note = (12*yPos) + (xPos-4);
		var currentlyPlayingNote = noteStack.last();
		noteStack.remove(note);

		case
		{ seqState > 1}{
			case
			// Letting go of the one and last note on stack
			{ noteStack.size == 0 }{
				//noteStack.remove(note);
				if(hold)
				{ noteStack.add( -1*abs(note) ) }
				{ if(~trans.on == false){ this.killNotesOn() } };
			}

			// More than one on stack and letting go of one playing now
			{ note == currentlyPlayingNote }{
				//noteStack.remove(note);
				if( ~trans.on == false){
					this.killNotesOn();
					this.noteOn(noteStack.last()+seq[selectedSeq][seqPos]);
					seqPos = (seqPos + 1)%seq[selectedSeq].size;
				}
			}

			// More than one on stack and not letting go of playing note
			//{ note != noteStack.last() }
			//{ noteStack.remove(note) }
		}

		{ (seqState == 0) || (seqState == 1)}{
			case
			// we released the one and last note on stack
			{ noteStack.size == 0 }{
				//noteStack.remove(note);
				if(hold)
				{ noteStack.add( -1*abs(note) ) }
				{ this.noteOff(note) };
			}

			// More than one on stack and letting go of one playing now
			{ note == currentlyPlayingNote }{
				this.noteOff(note);
				//noteStack.remove(note);
				this.noteOn(noteStack.last())
			}

			// More than one on stack and not letting go of playing note
			//{ note != noteStack.last() }
			//{ noteStack.remove(note) }

			//{ true }
			//{
			//	"entered case in free and record release function".postln;
			//	noteStack.postln;
			//	note.postln;
			//}
		}
	}

	/*
	 * Other Functions
	 */

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

	playSeqButtonPress {
		if(seqState == 2){ // Already on, turn off
			seqState = 0;
			bridge.sendMsg( lPath+/+"row",0, 3, 80, 171);
			bridge.sendMsg( lPath+/+"row",0, 4, 80, 171);
			this.killNotesOn();
			if(noteStack.size > 0){ this.noteOn(abs(noteStack.last())) }
		}{
			// was off or recording, turn on
			seqState = 2;
			bridge.sendMsg( lPath+/+"row",0,3, 92, 171);
			bridge.sendMsg( lPath+/+"row",0,4, 92, 171);
		};
		seqPos = 0;
	}

	recSeqButtonPress {
		if(seqState == 1){ // Seq is in record mode
			// Pressing it again will put it in silent record mode
			if(recordSilently){ // turn recording off
				recordSilently = false;
				seqState = 0;
				bridge.sendMsg( lPath+/+"row",0,3, 80, 171);
				bridge.sendMsg( lPath+/+"row",0,4, 80, 171);
			}{
				recordSilently = true;
				bridge.sendMsg( lPath+/+"row",0,3, 81, 171);
				bridge.sendMsg( lPath+/+"row",0,4, 82, 171);
			}
		}{
			// Seq is off or in play mode
			if( seqState > 1){
				this.killNotesOn();
				if(noteStack.size > 0){ this.noteOn(abs(noteStack.last())) };
			};

			seqState = 1; // turn on seq in record mode
			recordSilently = false; // first press keeps it audible
			bridge.sendMsg( lPath+/+"row",0,3, 83, 171);
			bridge.sendMsg( lPath+/+"row",0,4, 83, 171);
		};
		seqPos = 0;
	}

	selectPattern {|xPos|
		if((xPos >= 0 ) && ( xPos <= 3)){
			var newSelected = xPos;
			if( selectedSeq != newSelected ){
				selectedSeq = newSelected;
				bridge.sendMsg( lPath+/+"row",0,5, 80 + (2**selectedSeq), 171);
			}
		};
		seqPos = 0;
	}

	setSeqPlaySpeed {|xPos|
		if((xPos >=0) && (xPos <= 3)){
			seqPlaySpeed = xPos;
			bridge.sendMsg( lPath+/+"row", 0,6, 2**xPos, 0);
		}
	}

	transToggle {
		if( ~trans.on ){
			~trans.stop;
			bridge.sendMsg(lPath +/+ "set", 2,0,0);
			bridge.sendMsg(lPath +/+ "set", 3,0,0);
			bridge.sendMsg(lPath +/+ "row",0,1, 80,171);
			bridge.sendMsg(lPath +/+ "row",0,2, 0, 0);
			if(seqState == 2){ this.killNotesOn() };
		}{
			~trans.start;
			bridge.sendMsg(lPath +/+ "set", 2,0,1);
			bridge.sendMsg(lPath +/+ "set", 3,0,1);
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

	holdToggle {
		if(hold == true) {
			if( noteStack.size == 1 ){
				if(noteStack.last() < 0){ //case where there is a held note
					noteStack.pop();
					this.killNotesOn();
				}
			};
			//{ noteStack.size == 0 }
			//{ this.killNotesOn() };
			hold = false;
		}
		{ hold = true };
		bridge.sendMsg(lPath +/+ "row", 0,7,hold.if{12}{3}+80, 171);
	}

		tickResponder {|tick|
		var speed = 12; //(3 * (2**(4-seqPlaySpeed)));
		case
		{ seqPlaySpeed == 0 }{ speed = 12 }
		{ seqPlaySpeed == 1 }{ speed = 6 }
		{ seqPlaySpeed == 2 }{ speed = 3 }
		{ seqPlaySpeed == 3 }{ speed = 8 };

		if(	(seqState > 1) && (noteStack.size > 0)){ //down beat or note on
			if((tick/speed)%4 == 0){
				var note = if(seqPos == 0)
					{abs(noteStack.last())}
					{abs(noteStack.last()) + seq[selectedSeq][seqPos]};
				this.killNotesOn();
				this.noteOn(note);
				seqPos = (seqPos + 1)%seq[selectedSeq].size;
			};
		};

		if((seqState > 1)){ // Up beat or note off
			if((tick/speed)%4 == 3){ this.killNotesOn() };
		};

		if(show_cnt > 0){
			case
			{tick%48 == 0}{
				bridge.sendMsg(lPath +/+ "row",0,1, 92,171);
				bridge.sendMsg(lPath +/+ "row",0,2, 12,0);
			}
			{tick%48 == 24}{
				bridge.sendMsg(lPath +/+ "row",0,1, 83,171);
				bridge.sendMsg(lPath +/+ "row",0,2, 3,0);
			}
		}
	}

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
	noteOn {|num|
		midiOut.noteOn(0, num);
		notesOn.add(num);
	}

	noteOff {|num|
		midiOut.noteOff(0, num);
		notesOn.remove(num);
		//notesOn.postln;
		//("sent note off: " +num).postln;
		//noteStack.remove(num);
	}

	killNotesOn {
		//notesOn.postln;
		//"killing".postln;
		notesOn.do({|note|
			this.noteOff(note);
			//("killing note: " + note).postln;
		});
		//notesOn.postln;
	}

	killallnotes {
		(1..150).do({|note| midiOut.noteOff(0, note) })
	}
}
