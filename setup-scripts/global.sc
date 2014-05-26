(
var midiClient = 2;

MIDIClient.init;
//MIDIIn.connectAll;

~trans = Transport(MIDIClient.destinations.at(1).uid, 8000, 8001, 8002);

Micronome(MIDIClient.destinations.at(1).uid);

//Trigger();

// set up the trigger buffers (these serve both trigger and dseq)

)


// is the transport still busy with sending the midi tempo messages?1