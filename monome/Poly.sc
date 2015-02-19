Poly {

	classvar num_polys = 0;

	const path = "/sc/poly";
	const lPath = "/sc/poly/grid/led";

	// State
	var show_cnt = 0;
	var ctrl_press_cnt = 0;
	var iso_mode = false;
	//var hold = false;

	// Notes
	var press_set;
	// This keeps track directly of what is being pressed on the monome. It
	// holds Pair objects that represent the button coordinate pairs. The set
	// is unordered and everything is only kept in there once. The
	// stack should be empty when the monome is not showing this program.

	//Sequencer
	//var seq;
	//var seqState = 0; //0 off, 1 recording, and else is playing
	//var selectedSeq = 0; // thru 3
	//var seqPos = 0;
	//var recordSilently = true;
	//var seqPlaySpeed = 0;

	// Osc/Midi
	var bridge;
	var midiOut; // Keeps track of what notes are on microbrute-side


	*new {|bridgePortNum = 8000, uid|
		num_polys = num_polys + 1;
		if(num_polys == 1)
		{ ^super.new.init(bridgePortNum, uid) }
		{ "only one poly can be made".postln }
	}

	init {|bridgePortNum = 8000, uid|
		bridge = NetAddr.new("localhost", bridgePortNum);
		bridge.sendMsg("/sc/poly/grid/led/set", 0,0, 1);

		// Midi
		midiOut = MIDIOut(2);
		midiOut.connect(uid);
		//midiOut = MIDIOut.findPort("ZynAddSubFX-ZynAddSubFX", "ZynAddSubFX-ZynAddSubFX").uid;

		midiOut.latency = 0;

		// Data Structure Initialization
		press_set = Set[];
 		//seq = Array[
			//LinkedList[60],LinkedList[60],LinkedList[60],LinkedList[60] ];

		// Osc Responder Registration
		OSCdef(\poly_press,
			{|msg, time|
				var xPos = msg[1]; var yPos = msg[2];

				if(msg[3] == 1)
				{
					if(xPos < 4)
					{ this.controlPress(msg[1], msg[2], time) }
					{ this.notePress(msg[1], msg[2]) }
				}{
					if(xPos < 4)
					{ this.controlRelease(msg[1], msg[2], time) }
					{ this.noteRelease(msg[1], msg[2]) }
				}
			},
			path +/+ 'grid/key');

		OSCdef(\poly_tick,
			{|msg|
				//this.tickResponder(msg[1]);
				case
				{ msg[1] == 0 }
				{ this.trans_light_dn; }
				{ msg[1] == 24 }
				{ this.trans_light_up; }
			}, '/transport/tick');


		OSCdef(\poly_hide, { show_cnt = 0 }, path+/+'hide');

		OSCdef(\poly_show,
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

	controlPress {|xPos, yPos, time|
		// arameter checking
		if( (xPos > 15) || (yPos >= 8))
		{
			"got a press that is bigger than normal".postln;
			yPos.postln;
			yPos.postln;
		}{
			if(show_cnt == 0)
			{ show_cnt = 1 }
		};

		ctrl_press_cnt = ctrl_press_cnt + 1;
		if( ctrl_press_cnt == 1){
			case
			// Exit
			{ (yPos <= 1) && (xPos <= 1) }
			{ this.hide }

			//toggle trans
			{ yPos <= 1 }{ this.transToggle } // On Off

			// Tap and Clear
			{ (yPos <= 3) && (xPos <= 1) }
			{ ~trans.clear_tap }

			{ yPos <= 3 }
			{ ~trans.tap(time) }

			// keyboard mode select
			{ (yPos == 4) || (yPos == 5) }
			{
				if(iso_mode)
				{
					iso_mode = false;
					press_set.do({|pair, i|
						var note = (4*(yPos-1)) + (3*(xPos-5));
						midiOut.noteOff(0, note);
					});
				}
				{
					iso_mode = true;
					press_set.do({|pair, i|
						var note = (12*pair[1]) + (pair[0]-4);
						midiOut.noteOff(0, note);
					});
				};
				press_set.do({|pair, i|
					var note = (12*pair[1]) + (pair[0]-4);
					midiOut.noteOff(0, note);
				});
				press_set.clear;
				this.show;

			}


			//// Record and Play
			//{ (yPos == 3) || (yPos == 4) }{
			//	if(xPos < 2)
			//	{ this.recSeqButtonPress }
			//	{ this.playSeqButtonPress }
			//}
			//// Pattern Select
			//{ yPos == 5 }{ this.selectPattern(xPos) }

			//// Pattern Note Play Speed
			//{ yPos == 6 }{ this.setSeqPlaySpeed(xPos) }
		};
	}

	transToggle {
		if( ~trans.on )
		{
			~trans.stop;
			bridge.sendMsg(lPath +/+ "set", 2,0,0);
			bridge.sendMsg(lPath +/+ "set", 3,0,0);
			bridge.sendMsg(lPath +/+ "row",0,1, 80,171);
			bridge.sendMsg(lPath +/+ "row",0,2, 0, 0);
		}
		{ ~trans.start }
	}

	controlRelease {|xPos, yPos|
		ctrl_press_cnt = ctrl_press_cnt - 1;
		if(ctrl_press_cnt < 0){ ctrl_press_cnt = 0 }
	}


	notePress { |xPos, yPos| //|time|
		var note = 0;

		// This is the main purpose of this function, operations specific to
		// states are handled in the cases below.
		press_set.add([xPos, yPos]);

		if(iso_mode)
		{ note = (4*(yPos-1)) + (3*(xPos-5)) + 36}
		{ note = (12*yPos) + (xPos-4) };

		midiOut.noteOn(0, note);


		//case
		//{ seqState == 0 }{ 	//free mode
		//	//this.killNotesOn();
		//	this.noteOn(note);
		//}

		//{ seqState == 1 }{ // record mode
		//	//this.killNotesOn();
		//	if(recordSilently == false){ this.noteOn(note) };
		//	if(seqPos == 0)
		//	{ seq[selectedSeq] = LinkedList[noteStack.last()]; }
		//	{ seq[selectedSeq].add(noteStack.last()-seq[selectedSeq].first); };
		//	seqPos = seqPos + 1;
		//}

		//{ seqState > 1 }{ //Sequencer play mode
		//	if(~trans.on == false){
		//		//this.killNotesOn();
		//		if(seqPos == 0)
		//		{ this.noteOn(noteStack.last()) }
		//		{ this.noteOn(noteStack.last() + seq[selectedSeq][seqPos]) };
		//		seqPos = (seqPos + 1)%seq[selectedSeq].size;
		//	}
		//}
	}

	noteRelease {|xPos, yPos|
		var note = 0;

		press_set.remove([xPos, yPos]);

		if(iso_mode)
		{ note = (4*(yPos-1)) + (3*(xPos-5)) + 36 }
		{ note = (12*yPos) + (xPos-4) };

		midiOut.noteOff(0, note);

		//case
		//{ seqState > 1}{
		//	case
		//	// Letting go of the one and last note on stack
		//	{ noteStack.size == 0 }{
		//		//noteStack.remove(note);
		//		if(hold)
		//		{ noteStack.add( -1*abs(note) ) }
		//		{ if(~trans.on == false){ this.killNotesOn() } };
		//	}

		//	// More than one on stack and letting go of one playing now
		//	{ note == currentlyPlayingNote }{
		//		//noteStack.remove(note);
		//		if( ~trans.on == false){
		//			this.killNotesOn();
		//			this.noteOn(noteStack.last()+seq[selectedSeq][seqPos]);
		//			seqPos = (seqPos + 1)%seq[selectedSeq].size;
		//		}
		//	}

		//	// More than one on stack and not letting go of playing note
		//	//{ note != noteStack.last() }
		//	//{ noteStack.remove(note) }
		//}

		//{ (seqState == 0) || (seqState == 1)}{
		//	case
		//	// we released the one and last note on stack
		//	{ noteStack.size == 0 }{
		//		//noteStack.remove(note);
		//		if(hold)
		//		{ noteStack.add( -1*abs(note) ) }
		//		{ this.noteOff(note) };
		//	}

		//	// More than one on stack and letting go of one playing now
		//	{ note == currentlyPlayingNote }{
		//		this.noteOff(note);
		//		//noteStack.remove(note);
		//		this.noteOn(noteStack.last())
		//	}

		//}
	}

	/*
	 * Other Functions
	 */


	//playSeqButtonPress {
	//	if(seqState == 2){ // Already on, turn off
	//		seqState = 0;
	//		bridge.sendMsg( lPath+/+"row",0, 3, 80, 171);
	//		bridge.sendMsg( lPath+/+"row",0, 4, 80, 171);
	//		this.killNotesOn();
	//		if(noteStack.size > 0){ this.noteOn(abs(noteStack.last())) }
	//	}{
	//		// was off or recording, turn on
	//		seqState = 2;
	//		bridge.sendMsg( lPath+/+"row",0,3, 92, 171);
	//		bridge.sendMsg( lPath+/+"row",0,4, 92, 171);
	//	};
	//	seqPos = 0;
	//}

	//recSeqButtonPress {
	//	if(seqState == 1){ // Seq is in record mode
	//		// Pressing it again will put it in silent record mode
	//		if(recordSilently){ // turn recording off
	//			recordSilently = false;
	//			seqState = 0;
	//			bridge.sendMsg( lPath+/+"row",0,3, 80, 171);
	//			bridge.sendMsg( lPath+/+"row",0,4, 80, 171);
	//		}{
	//			recordSilently = true;
	//			bridge.sendMsg( lPath+/+"row",0,3, 81, 171);
	//			bridge.sendMsg( lPath+/+"row",0,4, 82, 171);
	//		}
	//	}{
	//		// Seq is off or in play mode
	//		if( seqState > 1){
	//			this.killNotesOn();
	//			if(noteStack.size > 0){ this.noteOn(abs(noteStack.last())) };
	//		};

	//		seqState = 1; // turn on seq in record mode
	//		recordSilently = false; // first press keeps it audible
	//		bridge.sendMsg( lPath+/+"row",0,3, 83, 171);
	//		bridge.sendMsg( lPath+/+"row",0,4, 83, 171);
	//	};
	//	seqPos = 0;
	//}

	//selectPattern {|xPos|
	//	if((xPos >= 0 ) && ( xPos <= 3)){
	//		var newSelected = xPos;
	//		if( selectedSeq != newSelected ){
	//			selectedSeq = newSelected;
	//			bridge.sendMsg( lPath+/+"row",0,5, 80 + (2**selectedSeq), 171);
	//		}
	//	};
	//	seqPos = 0;
	//}

	//setSeqPlaySpeed {|xPos|
	//	if((xPos >=0) && (xPos <= 3)){
	//		seqPlaySpeed = xPos;
	//		bridge.sendMsg( lPath+/+"row", 0,6, 2**xPos, 0);
	//	}
	//}


//	tickResponder {|tick|
//		var speed = 12;
//		case
//		{ seqPlaySpeed == 0 }{ speed = 12 }
//		{ seqPlaySpeed == 1 }{ speed = 6 }
//		{ seqPlaySpeed == 2 }{ speed = 3 }
//		{ seqPlaySpeed == 3 }{ speed = 8 };
//
//		if(	(seqState > 1) && (noteStack.size > 0)){ //down beat or note on
//			if(((tick/speed)%4) == 0){
//				var note = if(seqPos == 0)
//					{abs(noteStack.last())}
//					{abs(noteStack.last()) + seq[selectedSeq][seqPos]};
//				this.killNotesOn();
//				this.noteOn(note);
//				seqPos = (seqPos + 1)%seq[selectedSeq].size;
//			};
//		};
//
//		// Kill notes off in the seq
//		if( (seqState > 1) && ((tick/speed)%4 == 3) )
//		{ this.killNotesOn() };
//	}

	trans_light_up{
		if(show_cnt > 0){
			bridge.sendMsg(lPath +/+ "row",0,0, 15,0);
			bridge.sendMsg(lPath +/+ "row",0,1, 15,0);
			bridge.sendMsg(lPath +/+ "row",0,2, 3,0);
			bridge.sendMsg(lPath +/+ "row",0,3, 83,171);
		}
	}

	trans_light_dn{
		if(show_cnt > 0){
			bridge.sendMsg(lPath +/+ "row",0,2, 12,0);
			bridge.sendMsg(lPath +/+ "row",0,3, 92,171);
		}
	}

	/*
	 * Hide n show
	 */
	show { // Light up the monome
		if( show_cnt > 0){
			if(iso_mode){
				// Left side
				bridge.sendMsg(lPath +/+ "map",0,0,
					3,35 ,0,0,44, 12, 0,32);
				// Right Side
				bridge.sendMsg(lPath +/+ "map",8,0,
					0,34,0,0,34,0,0,34);
			}{
				// Left side
				bridge.sendMsg(lPath +/+ "map",0,0, 3,83,0,80,83,83,0,80);
				// Right Side
				bridge.sendMsg(lPath +/+ "map",8,0, 0,171,0,171,171,171,0,171);
			}
		};
	}

	hide{
		show_cnt = 0;
		bridge.sendMsg(path+/+"hide"); //sending a request to bridge to hide
		ctrl_press_cnt = 0;

		if(iso_mode){
			press_set.do({|pair, i|
				var note = (4*(pair[1]-1)) + (3*(pair[0]-5)) + 36;

				midiOut.noteOff(0, note);
			});
		}{
			press_set.do({|pair, i|
				var note = (12*pair[1]) + (pair[0]-4);
				midiOut.noteOff(0, note);
			});
		};
		press_set.clear;
	}

}
