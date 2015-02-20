
//MIDIDevice : AbstractDevice {
MonophonicMIDIDevice : MIDIOut {

	//var pitchRange = 12;

	var notesOnStack;
	var <sustaining;
	var <currentNote; //the note that is currently on.


	*new {|deviceName, portName|
		^super.new(0).init( deviceName, portName );
	}

	init {|deviceName, portName|

		if( portName == nil ){
			portName = deviceName;
		};
		
		super.connect( MIDIOut.findPort( deviceName, portName ).uid );
		super.latency = 0;

		notesOnStack = List[];
		sustaining = false;
		currentNote = 0;
	}

	sustainOn { sustaining = true;}
	sustainOff { 
		sustaining = false;
		this.killNotesSustaining;
	}

	noteOn {|note|
		super.noteOff( 0, currentNote );

		notesOnStack.removeEvery( [note] );
		notesOnStack.add(note);
		super.noteOn( 0, note );

		currentNote = note;
	}

	noteOff {|note|
		if( currentNote == note ){
			if( sustaining )
			{ notesOnStack.removeEvery([note]); } 
			{
				notesOnStack.removeEvery([note]);
				super.noteOff(0, note); 

				if( notesOnStack.last != nil ) { 
					super.noteOn(0, notesOnStack.last ); 
					currentNote = notesOnStack.last;
				}
			}
		}
		{ notesOnStack.removeEvery([note]); };
	}

	currentNoteOff {
		this.noteOff( currentNote );
	}

	killNotesOn {
		notesOnStack.do( {|note| super.noteOff(0, note) } );
		super.noteOff( 0, currentNote );
		notesOnStack = List[];
	}

	killNotesSustaining {
		super.noteOff( 0, currentNote );
	}

	killallnotes {
		(1..150).do({|note| super.noteOff(0, note) })
	}
}
