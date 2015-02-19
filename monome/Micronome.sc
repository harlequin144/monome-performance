Micronome {

	//classvar numMicronomes;
	const path = "/sc/micronome";
	const lPath = "/sc/micronome/grid/led";

	// State
	var show_cnt = 0;
	var sustain = false;
	var ctrl_press_cnt = 0;
	//var down_tick = 0;
	var trans_on = false;

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
	var transport;
	var midiOut; // Keeps track of what notes are on microbrute-side



	*new {|bridgePortNum = 8000, midi|
		^super.new.init(bridgePortNum, midi)
	}

	init {|bridgePortNum = 8000, midi|
		bridge = NetAddr.new("localhost", bridgePortNum);

		midiOut = midi;

		trans_on = false;

		// Data Structure Initialization
		noteStack = LinkedList[];

 		seq = Array[
			LinkedList[60],LinkedList[60],LinkedList[60],LinkedList[60] ];

		midiOut.control(0,105, 127);  // Seq Play hold to off
		midiOut.control(0, 109,0);
		midiOut.sysex(Int8Array[16rB0, 16r65, 16r0, 16rB0, 16r64, 16r0,
			16rB0, 16r06, pitchRange]);


		// Osc Responder Registration
		OSCdef(\micronome_press,
			{|msg, time|
				case
				{ msg[3] == 1}{ this.pressResponder(msg[1], msg[2], time) }
				{ msg[3] == 0}{ this.releaseResponder(msg[1], msg[2]) }
			}, path +/+ 'grid/key');

		OSCdef.newMatching(\micronome_tick,
			{|msg|
				this.tickResponder(msg[1]);
			}, '/transport/tick');


		OSCdef(\micronome_transport_stop,
			{|msg|
				trans_on = false;
				bridge.sendMsg(lPath+/+"row", 0,1, 80, 171);
				bridge.sendMsg(lPath+/+"row", 0,2, 0, 0);
			}, '/transport/stop');

		OSCdef(\micronome_hide,
			{
				show_cnt = show_cnt - 1;
				//"show decreased".postln; show_cnt.postln;
				if(show_cnt < 0){show_cnt = 0}
			}, path+/+'hide');

		OSCdef(\micronome_show,
			{
				show_cnt = show_cnt + 1;
				//"show increased".postln; show_cnt.postln;
				this.show;
			}, path+/+'show');

		this.show();
	}


	/*
	 * Press Functions
	 */

	pressResponder {|xPos, yPos, time|
		//if( xPos < 8)
		show_cnt = 1;
		//{ show_cnt = 2 };

		if( (xPos >= 4) && (yPos > 0) ){
			this.notePress(xPos, yPos)
		}{
			ctrl_press_cnt = ctrl_press_cnt + 1;
			if( ctrl_press_cnt == 1){
				case
				{ yPos == 0 }{
					case
					{ xPos < 2 } { this.hide }
					{ xPos < 4 } { transport.sendMsg("/transport/toggle") }
					{ xPos >= 4 }{ this.setMod(xPos-3) }
				}

				{ (yPos == 1) || (yPos == 2) }{
					if (xPos < 2)
					{ transport.sendMsg("/transport/clear_tap") }
					{ transport.sendMsg("/transport/tap") }
				}

				// Record and Play
				{ (yPos == 3) || (yPos == 4) }{
					if(xPos < 2)
					{ this.recSeqButtonPress }
					{ this.playSeqButtonPress }
				}

				{ yPos == 5 }{ this.selectPattern(xPos) }
				{ yPos == 6 }{ this.setSeqPlaySpeed(xPos) }
				{ yPos == 7 }{ this.sustainToggle() }
			}
		};

		if(yPos >= 8)
		{ "got a press that is bigger than normal".postln; yPos.postln; }
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
		if(sustain && noteStack.size == 1){
			if(noteStack.last() < 0){ noteStack.pop() }
		};
		// This is the main purpose of this function, operations specific to
		// states are handled in the cases below.
		noteStack.add(note);

		case
		{ seqState == 0 }{ 	//free mode
			midiOut.killNotesOn();
			midiOut.noteOn(note);
		}

		{ seqState == 1 }{ // record mode
			midiOut.killNotesOn();
			if(recordSilently == false){ midiOut.noteOn(note) };
			if(seqPos == 0){
				seq[selectedSeq] = LinkedList[noteStack.last()];
				seqPos = seqPos + 1;
			}{
				seq[selectedSeq].add(noteStack.last()-seq[selectedSeq].first);
				seqPos = seqPos + 1;
			};
		}

		{ seqState > 1 }{ //Sequencer play mode
			if(trans_on == false){
				midiOut.killNotesOn();
				if(seqPos == 0)
				{ midiOut.noteOn(noteStack.last()) }
				{ midiOut.noteOn(noteStack.last() + seq[selectedSeq][seqPos]) };
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
				if( sustain )
				{ noteStack.add( -1*abs(note) ) }
				{ if(trans_on == false){ midiOut.killNotesOn() } };
			}

			// More than one on stack and letting go of one playing now
			{ note == currentlyPlayingNote }{
				//noteStack.remove(note);
				if( trans_on == false){
					midiOut.killNotesOn();
					midiOut.noteOn(noteStack.last()+seq[selectedSeq][seqPos]);
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
				if( sustain )
				{ noteStack.add( -1*abs(note) ) }
				{ 
					midiOut.noteOff(note);
					noteStack.remove(note);
				};
			}

			// More than one on stack and letting go of one playing now
			{ note == currentlyPlayingNote }{
				midiOut.noteOff(note);
				noteStack.remove(note);
				midiOut.noteOn(noteStack.last())
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

	playSeqButtonPress {
		if(seqState == 2){ // Already on, turn off
			seqState = 0;
			bridge.sendMsg( lPath+/+"row",0, 3, 80, 171);
			bridge.sendMsg( lPath+/+"row",0, 4, 80, 171);
			midiOut.killNotesOn();
			if(noteStack.size > 0){ midiOut.noteOn(abs(noteStack.last())) }
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
				midiOut.killNotesOn();
				if(noteStack.size > 0){ midiOut.noteOn(abs(noteStack.last())) };
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
		if( sustain  == true) {
			if( noteStack.size == 1 ){
				if(noteStack.last() < 0){ //case where there is a held note
					noteStack.pop();
					midiOut.killNotesOn();
				}
			};
			//{ noteStack.size == 0 }
			//{ this.killNotesOn() };
			sustain = false;
		}
		{  sustain  = true };
		bridge.sendMsg(lPath +/+ "row", 0,7, sustain.if{12}{3}+80, 171);
	}


	tickResponder {|tick|
		var speed = 12;

		case
		{ tick == 0 } { this.trans_light_dn; }
		{ tick == 72 } { this.trans_light_up; };

		trans_on = true;

		case
		{ seqPlaySpeed == 0 }{ speed = 12 }
		{ seqPlaySpeed == 1 }{ speed = 6 }
		{ seqPlaySpeed == 2 }{ speed = 3 }
		{ seqPlaySpeed == 3 }{ speed = 8 };

		if(	(seqState > 1) && (noteStack.size > 0)){ //down beat or note on
			if(((tick/speed)%4) == 0){
				var note = if(seqPos == 0)
					{abs(noteStack.last())}
					{abs(noteStack.last()) + seq[selectedSeq][seqPos]};
				midiOut.killNotesOn();
				midiOut.noteOn(note);
				seqPos = (seqPos + 1)%seq[selectedSeq].size;
			};
		};

		// Kill notes off in the seq
		if( (seqState > 1) && ((tick/speed)%4 == 3) )
		{ midiOut.killNotesOn() };
	}

	trans_light_up{
		if(show_cnt > 0){
			bridge.sendMsg(lPath +/+ "row",0,1, 92,171);
			bridge.sendMsg(lPath +/+ "row",0,2, 12,0);
		}
	}

	trans_light_dn{
		if(show_cnt > 0){
			bridge.sendMsg(lPath +/+ "row",0,1, 83,171);
			bridge.sendMsg(lPath +/+ "row",0,2, 3,0);
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
				( if(trans_on){12}{0} ),
				80, 0, // Tap and clear - let tick handle update this
				80 + (4*seqFact), // Play and Record
				80 + (4*seqFact),
				80 + (2** selectedSeq), // Select Seq
				2**seqPlaySpeed,//Playback speed
				83 + (9 * sustain.if{1}{0}) // Hold
			);
			// Right Side
			bridge.sendMsg(lPath +/+ "map",8,0,
				(pitchG4).if{ 2**(pitchRange-4)-1 }{0},
				171,0,171,171,171,0,171);
		};
	}

	hide{
		show_cnt = 0;
		bridge.sendMsg(path+/+"hide"); //sending a request to bridge to hide
		ctrl_press_cnt = 0;
		if( sustain == false){
			midiOut.killNotesOn();
			{noteStack.size > 0}.while({noteStack.pop()});
		};
	}

}
