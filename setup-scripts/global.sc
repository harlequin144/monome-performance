
(
MIDIClient.init;
MIDIIn.connectAll;

~trans = Transport(MIDIClient.destinations.at(2).uid, 8000, 8001, 8002);

~mNome = Micronome(MIDIClient.destinations.at(2).uid);

)