
MIDIClient.init(0,1);
MIDIOut.connect(0,2);


m = MIDIOut(0, MIDIClient.destinations[1].uid);

m.control(0, ctlNum: 104, val: 127);//Play
m.control(0, ctlNum: 103, val: 127);//New Loop
m.control(0, ctlNum: 102, val: 127);//Record

(
m.stop;
m.songPtr(10);
m.continue;
)

m.start;
m.stop;

m.program(127, num: 102);

SystemClock(0.25, {m.midiClock; 0.25});
SystemClock.clear;

m.start;
m.stop;

(
m.stop;
t = 1/ (140/60 * 24);
SystemClock.clear;
SystemClock.sched(0.0,{ arg time;
	m.midiClock;
	t
});
m.start;
)