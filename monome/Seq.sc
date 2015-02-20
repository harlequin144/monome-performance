Seq {
	const path = "/sc/seq";
	const lPath = "/sc/seq/grid/led";

	// State
	var showCnt = 0;
	var controlPressCount = 0;
	var pressStack;

	var transOn = false;
	var downTick = 0;
	var lastTick = 0;

	//Sequencer
	var seq;
	var mode = 0; //0 - normal play, 1 - recording, 2 - rec silently
	var selectedSeq = 0; // 0 - 7 inclusive
	var seqPos = 0;
	var seqSpeed = 0;

	// Comms
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

		transOn = false;
		pressStack = List[];

 		seq = Array[
			LinkedList[60],LinkedList[60],LinkedList[60],LinkedList[60],
			LinkedList[60],LinkedList[60],LinkedList[60],LinkedList[60] ];
		seq[0] = LinkedList[0,1,2,3,4,5,6,7];

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
				transOn = false;
				this.showLeft;
			}, '/transport/stop');

		OSCdef(\seq_hide,
			{
				showCnt = showCnt - 1;
				//"show decreased".postln; showCnt.postln;
				if( showCnt < 0 ) { showCnt = 0 }
			}, path+/+'hide');

		OSCdef(\seq_show,
			{
				showCnt = showCnt + 1;
				//"show increased".postln; showCnt.postln;
				if( showCnt > 0 ) { this.show; };
			}, path+/+'show');

		this.show();
	}


	/*
	 * Press Functions
	 */

	pressResponder {|xPos, yPos, time|
		showCnt = 1;

		case
		{ (xPos >= 4) && (xPos <= 15) }{
			if( (yPos == 0) || (yPos == 1) )
			{ 
				if( xPos >= 8 )
				{ this.setSeq( xPos - 8 ); }
				{ this.setSpeed( xPos - 4 ); }
			}
			{ this.notePress(xPos, yPos) }
		}
		{ (xPos < 4) && ( xPos >= 0) }{
			controlPressCount = controlPressCount + 1;
			if( controlPressCount == 1){
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
			controlPressCount = controlPressCount - 1;
			if(controlPressCount < 0){ controlPressCount = 0 }
		}
	}

	notePress { |xPos, yPos| //|time|
		var note = (12*yPos) + (xPos-4);
		//pressStack.removeEvery( [note] );
		//pressStack.add( note );

		case
		{ mode == 0 }{
			downTick = lastTick;
			if( transOn){ seqPos = 0; };

			midiOut.currentNoteOff;
			pressStack.removeEvery( [note] );
			pressStack.add( note );

			if(seqPos == 0)
			{ midiOut.noteOn( note ); }
			{ 
				midiOut.noteOn( 
					pressStack.last() + seq[selectedSeq][seqPos]);
			};

			seqPos = (seqPos + 1)%seq[selectedSeq].size;
		}

		{ (mode == 1) || (mode == 2) }{ // record mode
			midiOut.currentNoteOff;
			pressStack.removeEvery( [note] );
			pressStack.add( note );
			if( mode == 1 ){ midiOut.noteOn( note ) };
			//currentNote = note;

			case
			{ seqPos == 0 }{
				seq[selectedSeq] = LinkedList[ note ];
				seqPos = seqPos + 1;
			}
			{ seqPos < 60 } {
				seq[selectedSeq].add(note - seq[selectedSeq].first);
				seqPos = seqPos + 1;
			};
		}

	}

	noteRelease {|xPos, yPos|
		var note = (12*yPos) + (xPos-4);

		if( note == pressStack.last ){
			case
			{ mode == 0 }{
				pressStack.removeEvery( [note] );
				if( transOn ) 
				{ if( pressStack.size <= 0 ) { midiOut.currentNoteOff; } }
				{ midiOut.currentNoteOff; }
			}
	
			{ (mode == 1) || (mode == 2) }{ // record mode
				midiOut.currentNoteOff;
				pressStack.removeEvery( [note] );
			}
		}
		{ pressStack.removeEvery( [note] ); };
	}

	setSeq {|seqNum|
		if( (seqNum >= 0) && (seqNum <= 7) ){
			selectedSeq = seqNum;
			this.showRight;
		};
		seqPos = 0;
	}

	setSpeed {|speed|
		if( (speed >=0) && (speed <= 3) ){
			seqSpeed = speed;
			this.showLeft;
		}
	}

	modeToggle {
		mode = (mode + 1) % 3;
		if( mode != 2 )
		{ seqPos = 0; };
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
		var tickOn = 0, tickOff = 1;

		lastTick = tick;
		
		case
		{ tick == 0 } { this.transLightDn; }
		{ tick == 72 } { this.transLightUp; };

		if( transOn == false ){
			bridge.sendMsg(lPath +/+ "set",0,2,1);
			bridge.sendMsg(lPath +/+ "set",0,5,1);
			transOn = true;
		};

		case
		{ seqSpeed == 0 }{ speed = 144 }
		{ seqSpeed == 1 }{ speed = 72 }
		{ seqSpeed == 2 }{ speed = 36 }
		{ seqSpeed == 3 }{ speed = 18 };

		if( (mode == 0) && (pressStack.size > 0) ) {
			case
			{ (tick%speed) == (downTick%speed) } {
				var note = 
				if(seqPos == 0)
				{ pressStack.last() }
				{ pressStack.last() + seq[selectedSeq][seqPos] };

				midiOut.noteOn(note);
				seqPos = (seqPos + 1)%seq[selectedSeq].size;
			}

			{ tick%speed == ( ( (downTick%speed) + ((speed/4)*3) )%speed) }
			{ midiOut.currentNoteOff; };
		}
	}

	transLightUp{
		if(showCnt > 0){
			bridge.sendMsg(lPath +/+ "row",0,3, 13,0);
			bridge.sendMsg(lPath +/+ "row",0,4, 93,171);
		}
	}

	transLightDn{
		if(showCnt > 0){
			bridge.sendMsg(lPath +/+ "row",0,3, 03,0);
			bridge.sendMsg(lPath +/+ "row",0,4, 83,171);
		}
	}

	show {
		this.showLeft;
		this.showRight;
	}

	showLeft {
		if(showCnt > 0){
			var ledOut = 0, ledIn = 0;
			case
			{ mode == 0 } {ledOut = 0; ledIn = 4;}
			{ mode == 1 } {ledOut = 14; ledIn = 10;}
			{ mode == 2 } {ledOut = 4; ledIn = 10;};

			bridge.sendMsg(lPath +/+ "map", 0,0,
				1 + ledOut + ( 2**(4+seqSpeed) ),
				ledIn + ( 2**(4+seqSpeed) ),
				80 + ledOut + if( transOn ){1}{0},
	
				1, 81, 

				80 + if(midiOut.sustaining){14}{0} + if( transOn ){1}{0}, 
				if(midiOut.sustaining){10}{4},
				81 + if(midiOut.sustaining){14}{0}
			);
		};
	}

	showRight {
		if(showCnt > 0) { 
			bridge.sendMsg( lPath +/+ "map", 8,0, 
				2**selectedSeq,
				2**selectedSeq,
				171,0, 171,171,0,171 
			); 
		};
	}

	hide{
		showCnt = 0;
		bridge.sendMsg(path+/+"hide"); //sending a request to bridge to hide
		controlPressCount = 0;
		if( midiOut.sustaining == false){
			midiOut.currentNoteOff;
			pressStack = List[];
		};
	}

}
