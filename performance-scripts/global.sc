(
MIDIClient.init;

//~trans = Transport(bridgePort: 8000, othPorts: 57120);

~microbrute = MonophonicMIDIDevice("USB2.0-MIDI-USB2.0-MIDI MIDI 2");
//~zyn = MIDIDevice("ZynAddSubFX-ZynAddSubFX");



//~mono = Micronome(bridgePortNum: 8000, transPortNum: 8001, midi: ~microbrute);
~mono = Mono(bridgePortNum: 8000, midi: ~microbrute);
~mono = Seq(bridgePortNum: 8000, midi: ~microbrute);
//~poly = Poly(8000, ~zyn);

)


~microbrute.killallnotes


MIDIClient.destinations;

