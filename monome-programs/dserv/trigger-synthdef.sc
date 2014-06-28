#!/usr/local/bin/sclang

/*
 * This is a synthdef that the dserver uses when triggering things server
 * side. It will be assumed that this is already loaded into the synthdefs
 * directory when dserv is started, so consider running this script and
 * loading this synth def into a directory that sc will see and use properly a
 * part of the build process for this program. i. e. look to see that this
 * synthdef is properly done when there are errors.
 *
 */


SynthDef(\dtrigger,
	{ |bufnum, t_trig = 0, out = 0|
		Out.ar(out,
			PlayBuf.ar(1, bufnum, BufRateScale.kr(bufnum), t_trig, 0,
			doneAction: 0)
		)
	}
).load;

