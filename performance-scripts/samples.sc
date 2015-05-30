
Server.local.makeGui
// Three kinds of samplers:
// - triggers
// - loopers
// - pitched
// ????? build/loop sampler



do



p = {Out.ar(0,Pan2.ar(PlayBuf.ar(1, ~t, BufRateScale.kr(~t), doneAction: 2)))}.play
p.free

s.boot
~t = Buffer.read(Server.local, "/home/dylan/music/samples/g-nome-samples/shield-killer.wav");
~p = TriggerSynth(List[~t])
~p.pull

(
var drum_set_path = "/home/dylan/music/samples/VintageDrumSamples24bit/Roland TR-808";

var snare = Buffer.read(s, drum_set_path +/+ "TR-808Snare01.wav");
var kick = Buffer.read(s, drum_set_path +/+ "TR-808Kick03.wav");
var hato = Buffer.read(s, drum_set_path +/+ "TR-808Hat_O01.wav");
var hatc = Buffer.read(s, drum_set_path +/+ "TR-808Hat_C01.wav");
var ride = Buffer.read(s, drum_set_path +/+ "TR-808Ride01.wav");

~trigger = Trigger("/sc/trigger");

~trigger.addTrigger(\snare, [snare]);
~trigger.addTrigger(\kick, [kick])
~trigger.addTrigger(\hato, [hato])
//~trigger.addTrigger(\kick, quantize, interval)
//~trigger.addTrigger(\hato, quantize, interval)


//"/sc/trigger/trig", "name"
//"/sc/trigger/kill", "name"
)


~trigger.pull(\snare)

