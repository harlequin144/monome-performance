AbstractDevice {

	var sustaining = false;

	var notesOn; // these corespond just with notes that are being pressed
	var notesSustaining; // not being pressed but still sustaining.


	//*new {
		//^super.new.init();
	//}

	//init {
		//notesOn = Set[];
		//notesSustaining = Set[];
	//}

	noteOn {|note|
		// send signal
		notesOn.add( note );
	}

	noteOff {|note|
		// send signal

		notesOn.remove( note );

		if( sustaining ) 
		{ notesSustaining.add( note ) }
	}

	sustainOn { this.sustaining = true; }
	sustainOff { this.sustaining = false; }


	killNotesOn {
		notesOn.do({|note|
			this.noteOff(note);
		});
	}
}
