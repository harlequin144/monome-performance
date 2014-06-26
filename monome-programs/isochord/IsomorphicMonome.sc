// This class is meant to encapsulate the process of translating the position
// of button presses on the monome into note numbers. This class will maintian
// state information that will affect how the press positions will be
// interpreted. 

// Note numbers will follow those of the midi note numbers


IsomorphicMonome {

	// State
	var base_note;

	*new {|base_note = 12|
		^super.new.init(base_note)
	}

	init {|base_note|

		base_note = 12;

	}

	note_from_pos {|xPos, yPos|
		var return_note = base_note + (xPos * 4) + (yPos * 3);
		^return_note
	}

	get_positions_of_note {|num|

	}





}
