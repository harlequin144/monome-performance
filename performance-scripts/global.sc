(
MIDIClient.init;

// Servers
//~dserv = Dserver();


// Synth Controllers
~mono = Micronome(bridgePortNum: 8000, transportPortNum: 8001, uid: 1);
//~mono = Mono(8000, 1);
	//MIDIOut.findPort("USB2.0-MIDI-USB2.0-MIDI MIDI 2",
	//	"USB2.0-MIDI-USB2.0-MIDI MIDI 2").uid);

//the sequencer sems to be broken.... last note is not getting killed.

//Poly(8000, 7);o
//	MIDIOut.findPort("ZynAddSubFX-ZynAddSubFX", "ZynAddSubFX-ZynAddSubFX").uid);

// Need to make the source device in these a parameter

// Samplers
//Choppa();

)

~mono.show()
~mono.killallnotes

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