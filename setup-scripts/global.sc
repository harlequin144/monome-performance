(
var midiClient = 2;

MIDIClient.init;
//MIDIIn.connectAll;

~trans = Transport(MIDIClient.destinations.at(1).uid, 8000, 8001, 8002);
// transport cant keep track of all the prefixs, so it cant send the stop message properly.
// also, there are still bugs noted in the file, search bug
Micronome(MIDIClient.destinations.at(1).uid);

~dserv = Dserver();

// set up the trigger buffers (these serve both trigger and dseq)

)


// is the transport still busy with sending the midi tempo messages?1
~dserv.trigger(\sn1);


// The dserver accomodates multipule ways to trigger something. The clients decide how the will be triggered. Any Sample can be triggered in one of the possible ways

// - shot
// - loop
// - walk - progress and loop only as pressed
//
//
//
