Seq {
	const path = "/sc/seq";
	const lPath = "/sc/seq/grid/led";

	// State
	var show_cnt = 0;
	var ctrl_press_cnt = 0;
	//var down_tick = 0;
	var trans_on = false;
	var pressStack;


	//Sequencer
	var seq;
	var mode = 0; //0 - normal play, 1 - recording, 2 - rec silently

	var selectedSeq = 0; // 0 - 7 inclusive
	var seqPos = 0;
	var seqSpeed = 0;

	// Osc/Midi
	var bridge;
	var transport;
	var midiOut; // Keeps track of what notes are on microbrute-side



	*new {|bridgePortNum = 8000, transPortNum = 8001, midi|
		^super.new.init(bridgePortNum, transPortNum, midi)
	}

	init {|bridgePortNum = 8000, transPortNum = 8001, midi|

		bridge = NetAddr.new("localhost", bridgePortNum);
		transport = NetAddr.new("localhost", transPortNum);
		midiOut = midi;

		trans_on = false;
		pressStack = List[];

 		seq = Array[
			LinkedList[60],LinkedList[60],LinkedList[60],LinkedList[60],
			LinkedList[60],LinkedList[60],LinkedList[60],LinkedList[60] ];

		// Osc Responder Registration
		OSCdef(\seq_press,
			{|msg, time|
				case
				{ msg[3] == 1}{ this.pressResponder(msg[1], msg[2], time) }
				{ msg[3] == 0}{ this.releaseResponder(msg[1], msg[2]) }
			}, path +/+ 'grid/key');

		OSCdef.newMatching(\seq_tick,
			{|msg|
				this.tickResponder(msg[1]);
			}, '/transport/tick');


		OSCdef(\seq_transport_stop,
			{|msg|
				trans_on = false;
				this.showLeft;
			}, '/transport/stop');

		OSCdef(\seq_hide,
			{
				show_cnt = show_cnt - 1;
				//"show decreased".postln; show_cnt.postln;
				if( show_cnt < 0 ) { show_cnt = 0 }
			}, path+/+'hide');

		OSCdef(\seq_show,
			{
				show_cnt = show_cnt + 1;
				//"show increased".postln; show_cnt.postln;
				if( show_cnt > 0 ) { this.show; };
			}, path+/+'show');

		this.show();
	}


	/*
	 * Press Functions
	 */

	pressResponder {|xPos, yPos, time|
		show_cnt = 1;

		case
		{ (xPos >= 4) && (xPos <= 15) }{
			if( (yPos == 0) || (yPos == 1) )
			{ 
				if( xPos >= 8 )
				{ this.setSeq( xPos - 8 ); }
				{ this.setSpeed( xPos - 4 ); }
			}
			{ } //this.notePress(xPos, yPos) }
		}
		{ (xPos < 4) && ( xPos >= 0) }{
			ctrl_press_cnt = ctrl_press_cnt + 1;
			if( ctrl_press_cnt == 1){
				case
				{ xPos == 0} {
					case
					{ yPos == 0 } { this.hide }
					{ yPos == 7 } {} // switch to the mono

					{ (yPos >= 2) && (yPos <= 5) }
					{ transport.sendMsg("/transport/toggle") }
				}

				{ yPos <= 2 } { this.modeToggle; }

				{ yPos <= 4 } { 
					if( xPos == 1 )
					{ transport.sendMsg("/transport/clear_tap") }
					{ transport.sendMsg("/transport/tap") }
				}

				{ yPos <= 7} { this.sustainToggle; }
			}
		};

		if( (yPos >= 8) || (yPos < 0) || (xPos < 0) || (xPos >= 16) )
		{ "got a weird press".postln; xPos.postln; yPos.postln; }
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
		//pressStack.removeEvery( [note] );
		//pressStack.add( note );

		case
		{ mode == 0 }{
			if( trans_on ) {

			}{
				midiOut.killNotesOn();
				////if(seqPos == 0)
				//{ midiOut.noteOn(noteStack.last()) }
				//{ midiOut.noteOn(noteStack.last() + seq[selectedSeq][seqPos]) };
				//seqPos = (seqPos + 1)%seq[selectedSeq].size;
			}
		}

		{ (mode == 1) || (mode == 2) }{ // record mode
		//	midiOut.killNotesOn();
		//	if(recordSilently == false){ midiOut.noteOn(note) };
		//	if(seqPos == 0){
		//		//seq[selectedSeq] = LinkedList[noteStack.last()];
		//		seqPos = seqPos + 1;
		//	}{
		//		//seq[selectedSeq].add(noteStack.last()-seq[selectedSeq].first);
		//		seqPos = seqPos + 1;
		//	};
		}

	}

	noteRelease {|xPos, yPos|
		var note = (12*yPos) + (xPos-4);
		//var currentlyPlayingNote = noteStack.last();
		//noteStack.remove(note);

		//case
		//{ seqState > 1}{
		//	//case
		//	// Letting go of the one and last note on stack
		//	//{ noteStack.size == 0 }{
		//		//noteStack.remove(note);
		//		//if( sustain )
		//		//{ noteStack.add( -1*abs(note) ) }
		//		//{ if(trans_on == false){ midiOut.killNotesOn() } };
		//	//}

		//	// More than one on stack and letting go of one playing now
		//	//{ note == currentlyPlayingNote }{
		//		////noteStack.remove(note);
		//		//if( trans_on == false){
		//			//midiOut.killNotesOn();
		//			//midiOut.noteOn(noteStack.last()+seq[selectedSeq][seqPos]);
		//			//seqPos = (seqPos + 1)%seq[selectedSeq].size;
		//		//}
		//	//}

		//	// More than one on stack and not letting go of playing note
		//	//{ note != noteStack.last() }
		//	//{ noteStack.remove(note) }
		//}

		//{ (seqState == 0) || (seqState == 1)}{
		//	//case
		//	// we released the one and last note on stack
		//	//{ noteStack.size == 0 }{
		//		//noteStack.remove(note);
		//		//if( sustain )
		//		//{ noteStack.add( -1*abs(note) ) }
		//		////{ 
		//			//midiOut.noteOff(note);
		//			//noteStack.remove(note);
		//		//};
		//	//}
//
		//	// More than one on stack and letting go of one playing now
		//	//{ note == currentlyPlayingNote }{
		//		//midiOut.noteOff(note);
		//		//noteStack.remove(note);
		//		////midiOut.noteOn(noteStack.last())
		//	//}

		//	// More than one on stack and not letting go of playing note
		//	//{ note != noteStack.last() }
		//	//{ noteStack.remove(note) }

		//	//{ true }
		//	//{
		//	//	"entered case in free and record release function".postln;
		//	//	noteStack.postln;
		//	//	note.postln;
		//	//}
		//}
	}

	/*
	 * Other Functions
	 */

	recSeqButtonPress {
		//if(seqState == 1){ // Seq is in record mode
			// Pressing it again will put it in silent record mode
			//if(recordSilently){ // turn recording off
				//recordSilently = false;
				//seqState = 0;
				bridge.sendMsg( lPath+/+"row",0,3, 80, 171);
				bridge.sendMsg( lPath+/+"row",0,4, 80, 171);
			//}{
				//recordSilently = true;
				bridge.sendMsg( lPath+/+"row",0,3, 81, 171);
				bridge.sendMsg( lPath+/+"row",0,4, 82, 171);
			//}
		//}{
			// Seq is off or in play mode
			//if( seqState > 1){
				midiOut.killNotesOn();
				//if(noteStack.size > 0){ midiOut.noteOn(abs(noteStack.last())) };
			//};

			//seqState = 1; // turn on seq in record mode
			//recordSilently = false; // first press keeps it audible
			bridge.sendMsg( lPath+/+"row",0,3, 83, 171);
			bridge.sendMsg( lPath+/+"row",0,4, 83, 171);
		//};
		//seqPos = 0;
	}

	setSeq {|seqNum|
		if( (seqNum >= 0) && (seqNum <= 7) ){
			selectedSeq = seqNum;
			this.showRight;
		};
		//seqPos = 0;
	}

	setSpeed {|speed|
		if( (speed >=0) && (speed <= 3) ){
			seqSpeed = speed;
			this.showLeft;
		}
	}

	modeToggle {
		mode = (mode + 1) % 3;
		this.showLeft;
	}

	sustainToggle {
		if( midiOut.sustaining ) 
		{ midiOut.sustainOff; }
		{ midiOut.sustainOn; };
		this.showLeft;
	}


	tickResponder {|tick|
		var speed = 36;
		case
		{ tick == 0 } { this.trans_light_dn; }
		{ tick == 72 } { this.trans_light_up; };

		if( trans_on == false ){
			bridge.sendMsg(lPath +/+ "set",0,2,1);
			bridge.sendMsg(lPath +/+ "set",0,5,1);
			trans_on = true;
		};

		//case
		//{ seqPlaySpeed == 0 }{ speed = 12 }
		//{ seqPlaySpeed == 1 }{ speed = 6 }
		//{ seqPlaySpeed == 2 }{ speed = 3 }
		//{ seqPlaySpeed == 3 }{ speed = 8 };

		//if(	(seqState > 1) ) { //&& (noteStack.size > 0)){ //down beat or note on
			if(((tick/speed)%4) == 0){
				//var note = if(seqPos == 0)
					//{abs() { //noteStack.last())}
					//{abs(noteStack.last()) + seq[selectedSeq][seqPos]};
				midiOut.killNotesOn();
				//midiOut.noteOn(note);
				//seqPos = (seqPos + 1)%seq[selectedSeq].size;
			};
		//};

		// Kill notes off in the seq
		//if( (seqState > 1) && ((tick/speed)%4 == 3) )
		//{ midiOut.killNotesOn() };
	}

	trans_light_up{
		if(show_cnt > 0){
			bridge.sendMsg(lPath +/+ "row",0,3, 13,0);
			bridge.sendMsg(lPath +/+ "row",0,4, 93,171);
		}
	}

	trans_light_dn{
		if(show_cnt > 0){
			bridge.sendMsg(lPath +/+ "row",0,3, 03,0);
			bridge.sendMsg(lPath +/+ "row",0,4, 83,171);
		}
	}

	show {
		this.showLeft;
		this.showRight;
	}

	showLeft {
		if(show_cnt > 0){
			var ledOut = 0, ledIn = 0;
			case
			{ mode == 0 } {ledOut = 0; ledIn = 4;}
			{ mode == 1 } {ledOut = 14; ledIn = 10;}
			{ mode == 2 } {ledOut = 4; ledIn = 10;};

			bridge.sendMsg(lPath +/+ "map", 0,0,
				1 + ledOut + ( 2**(4+seqSpeed) ),
				ledIn + ( 2**(4+seqSpeed) ),
				80 + ledOut + if( trans_on ){1}{0},
	
				1, 81, 

				80 + if(midiOut.sustaining){14}{0} + if( trans_on ){1}{0}, 
				if(midiOut.sustaining){10}{4},
				81 + if(midiOut.sustaining){14}{0}
			);
		};
	}

	showRight {
		if(show_cnt > 0) { 
			bridge.sendMsg( lPath +/+ "map", 8,0, 
				2**selectedSeq,
				2**selectedSeq,
				171,0, 171,171,0,171 
			); 
		};
	}

	hide{
		show_cnt = 0;
		bridge.sendMsg(path+/+"hide"); //sending a request to bridge to hide
		ctrl_press_cnt = 0;
		//if( midiOut.sustaining == false){
			//midiOut.killNotesOn();
			//{noteStack.size > 0}.while({noteStack.pop()});
		//};
	}

}
