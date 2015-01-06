~trans = Transport(8000, 8001, 8002);

// Devices
(
	MIDIClient.init;
	~microbrute = MidiDevice();
	~zyn = MidiDevice();
	~scat = PitchedSampler();
	//~scutter
	//~tcutter

	// AbstractDevice --> MidiDevice
	// 				  --> SCDevice
)


//Controllers
(
	~bruteMono = MMonoCtlr(~microbrute);
	~bruteSeq = MSeqCtlr(~microbrute);
	~zynPoly = MPolyCtlr(~zyn);
	~zynSeq = MSeqCtlr(~zyn);
	~scatPoly = MPolyCtlr(~scat);
	//~soundboard
	//~switch
	//~chopper
	//~transcontrol???
)


// change pitchsampler samples

// change triggered samples
// change other samples

// change the switch's control function




~microbrute.killall;


~dserv.trigger(\sn1);


