(
MIDIClient.init;

// Servers
//~dserv = Dserver();
~trans = Transport(8000, 8001, 8002);
// transport cant keep track of all the prefixs, so it cant send the stop message properly.

// Synth Controllers
~mono = Micronome(8000, 1);
//~mono = Mono(8000, 1);
	//MIDIOut.findPort("USB2.0-MIDI-USB2.0-MIDI MIDI 2",
	//	"USB2.0-MIDI-USB2.0-MIDI MIDI 2").uid);

//the sequencer sems to be broken.... last note is not getting killed.

//Poly(8000, 7);
//	MIDIOut.findPort("ZynAddSubFX-ZynAddSubFX", "ZynAddSubFX-ZynAddSubFX").uid);

// Need to make the source device in these a parameter

// Samplers
//Choppa();

)

~mono.show()
~mono.killallnotes
Set

MIDIOut.findPort("ZynAddSubFX-ZynAddSubFX", "ZynAddSubFX-ZynAddSubFX").uid

~dserv.trigger(\sn1);

// The dserver accomodates multipule ways to trigger something. The clients decide how the will be triggered. Any Sample can be triggered in one of the possible ways

// - shot
// - loop
// - walk - progress and loop only as pressed
//
//
//

MIDIClient.destinations;

m.control(0, 109,0)

Poly()