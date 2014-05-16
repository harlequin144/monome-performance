{
	"patcher" : 	{
		"fileversion" : 1,
		"rect" : [ 8.0, 50.0, 1304.0, 700.0 ],
		"bglocked" : 0,
		"defrect" : [ 8.0, 50.0, 1304.0, 700.0 ],
		"openrect" : [ 0.0, 0.0, 0.0, 0.0 ],
		"openinpresentation" : 1,
		"default_fontsize" : 12.0,
		"default_fontface" : 0,
		"default_fontname" : "Arial",
		"gridonopen" : 0,
		"gridsize" : [ 15.0, 15.0 ],
		"gridsnaponopen" : 0,
		"toolbarvisible" : 1,
		"boxanimatetime" : 200,
		"imprint" : 0,
		"boxes" : [ 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "togedge",
					"outlettype" : [ "bang", "bang" ],
					"fontname" : "Arial",
					"id" : "obj-51",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 660.0, 435.0, 54.0, 20.0 ],
					"numoutlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "bangbang",
					"outlettype" : [ "bang", "bang" ],
					"fontname" : "Arial",
					"id" : "obj-50",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 780.0, 450.0, 64.0, 20.0 ],
					"numoutlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "onebang",
					"outlettype" : [ "bang", "bang" ],
					"fontname" : "Arial",
					"id" : "obj-49",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 840.0, 450.0, 57.0, 20.0 ],
					"numoutlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "metro",
					"outlettype" : [ "bang" ],
					"fontname" : "Arial",
					"id" : "obj-41",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 930.0, 555.0, 41.0, 20.0 ],
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "transport",
					"outlettype" : [ "int", "int", "float", "float", "float", "", "int", "float", "" ],
					"fontname" : "Arial",
					"id" : "obj-40",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 780.0, 555.0, 127.0, 20.0 ],
					"numoutlets" : 9
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "gswitch2",
					"outlettype" : [ "", "" ],
					"id" : "obj-38",
					"numinlets" : 2,
					"patching_rect" : [ 783.0, 645.0, 39.0, 32.0 ],
					"numoutlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "gswitch",
					"outlettype" : [ "" ],
					"id" : "obj-37",
					"numinlets" : 3,
					"patching_rect" : [ 708.0, 645.0, 41.0, 32.0 ],
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "seq",
					"outlettype" : [ "int", "bang" ],
					"fontname" : "Arial",
					"id" : "obj-36",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 690.0, 600.0, 32.5, 20.0 ],
					"numoutlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "follow",
					"outlettype" : [ "int", "int" ],
					"fontname" : "Arial",
					"id" : "obj-35",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 810.0, 600.0, 41.0, 20.0 ],
					"numoutlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "mtr",
					"outlettype" : [ "", "" ],
					"fontname" : "Arial",
					"id" : "obj-34",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 765.0, 600.0, 32.5, 20.0 ],
					"numoutlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "detonate",
					"outlettype" : [ "int", "int", "int", "int", "int", "int", "int", "int" ],
					"fontname" : "Arial",
					"id" : "obj-33",
					"fontsize" : 12.0,
					"numinlets" : 8,
					"patching_rect" : [ 915.0, 600.0, 113.5, 20.0 ],
					"numoutlets" : 8,
					"save" : [ "#N", "detonate", "", ";", "#X", "setparam", 0, "Time", 0, 0, 999999, 0, 1000, 200, 0, ";", "#X", "setparam", 1, "Pitch", 4, 0, 127, 60, 12, 4, 0, ";", "#X", "setparam", 2, "Vel", 2, 0, 127, 64, 12, 4, 0, ";", "#X", "setparam", 3, "Dur", 3, 1, 99999, 200, 800, 200, 0, ";", "#X", "setparam", 4, "Chan", 0, 1, 16, 1, 8, 1, 0, ";", "#X", "setparam", 5, "Track", 0, 1, 32, 1, 8, 1, 0, ";", "#X", "setparam", 6, "X1", 0, 0, 999, 0, 80, 20, 0, ";", "#X", "setparam", 7, "X2", 0, 0, 999, 0, 80, 20, 0, ";", "#X", "restore", ";", "#X", "stop", ";" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "tempo",
					"outlettype" : [ "int" ],
					"fontname" : "Arial",
					"id" : "obj-30",
					"fontsize" : 12.0,
					"numinlets" : 4,
					"patching_rect" : [ 690.0, 555.0, 59.5, 20.0 ],
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "Info",
					"outlettype" : [ "" ],
					"fontname" : "Arial",
					"id" : "obj-26",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 908.0, 166.0, 32.5, 18.0 ],
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "t open",
					"outlettype" : [ "open" ],
					"fontname" : "Arial",
					"id" : "obj-15",
					"hidden" : 1,
					"fontsize" : 9.0,
					"numinlets" : 1,
					"patching_rect" : [ 904.0, 191.0, 37.0, 17.0 ],
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "text SlicerInfo",
					"outlettype" : [ "", "bang", "int" ],
					"fontname" : "Arial",
					"id" : "obj-17",
					"hidden" : 1,
					"fontsize" : 9.0,
					"numinlets" : 1,
					"patching_rect" : [ 907.0, 232.0, 65.0, 17.0 ],
					"numoutlets" : 3
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "/sys/prefix /Slicer",
					"outlettype" : [ "" ],
					"fontname" : "Arial",
					"id" : "obj-70",
					"presentation_rect" : [ 1155.0, 90.0, 110.0, 16.0 ],
					"fontsize" : 10.0,
					"numinlets" : 2,
					"patching_rect" : [ 1350.0, 60.0, 86.0, 16.0 ],
					"numoutlets" : 1,
					"presentation" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "udpsend localhost 8080",
					"fontname" : "Arial",
					"id" : "obj-71",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 1352.0, 93.0, 150.0, 20.0 ],
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "bpatcher",
					"outlettype" : [ "" ],
					"args" : [  ],
					"id" : "obj-32",
					"presentation_rect" : [ 435.0, 30.0, 108.0, 200.0 ],
					"numinlets" : 1,
					"name" : "SlicerMuteGroups.maxpat",
					"patching_rect" : [ 75.0, 375.0, 129.0, 28.0 ],
					"numoutlets" : 1,
					"presentation" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "bpatcher",
					"outlettype" : [ "" ],
					"args" : [  ],
					"id" : "obj-18",
					"presentation_rect" : [ 345.0, 30.0, 54.189999, 200.600006 ],
					"numinlets" : 0,
					"name" : "SlicerButtons.maxpat",
					"patching_rect" : [ 62.0, 320.0, 128.0, 22.729996 ],
					"numoutlets" : 1,
					"presentation" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "SlicerLedChannel 6",
					"outlettype" : [ "" ],
					"fontname" : "Arial",
					"id" : "obj-14",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 300.0, 675.0, 208.0, 20.0 ],
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "SlicerLedChannel 5",
					"outlettype" : [ "" ],
					"fontname" : "Arial",
					"id" : "obj-12",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 300.0, 645.0, 194.0, 20.0 ],
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "SlicerLedChannel 4",
					"outlettype" : [ "" ],
					"fontname" : "Arial",
					"id" : "obj-13",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 300.0, 615.0, 180.0, 20.0 ],
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "SlicerLedChannel 3",
					"outlettype" : [ "" ],
					"fontname" : "Arial",
					"id" : "obj-10",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 300.0, 585.0, 166.0, 20.0 ],
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "SlicerLedChannel 2",
					"outlettype" : [ "" ],
					"fontname" : "Arial",
					"id" : "obj-11",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 300.0, 555.0, 152.0, 20.0 ],
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "SlicerLedChannel 1",
					"outlettype" : [ "" ],
					"fontname" : "Arial",
					"id" : "obj-7",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 300.0, 525.0, 137.0, 20.0 ],
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "print Channel7",
					"fontname" : "Arial",
					"id" : "obj-27",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 1080.0, 494.0, 89.0, 20.0 ],
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "print Channel6",
					"fontname" : "Arial",
					"id" : "obj-28",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 1080.0, 464.0, 89.0, 20.0 ],
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "print Channel5",
					"fontname" : "Arial",
					"id" : "obj-29",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 1080.0, 434.0, 89.0, 20.0 ],
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "print Channel4",
					"fontname" : "Arial",
					"id" : "obj-24",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 1080.0, 404.0, 89.0, 20.0 ],
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "print Channel3",
					"fontname" : "Arial",
					"id" : "obj-25",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 1080.0, 374.0, 89.0, 20.0 ],
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "print Channel2",
					"fontname" : "Arial",
					"id" : "obj-9",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 1080.0, 344.0, 89.0, 20.0 ],
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "print Channel1",
					"fontname" : "Arial",
					"id" : "obj-4",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 1080.0, 314.0, 89.0, 20.0 ],
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "bpatcher",
					"args" : [  ],
					"id" : "obj-21",
					"presentation_rect" : [ 15.0, 525.0, 701.0, 207.0 ],
					"numinlets" : 7,
					"name" : "mixer7.maxpat",
					"patching_rect" : [ 60.0, 585.0, 221.0, 44.0 ],
					"numoutlets" : 0,
					"presentation" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "bpatcher",
					"outlettype" : [ "signal", "signal", "signal", "signal", "signal", "signal", "signal", "signal", "signal", "signal", "signal", "signal", "signal", "signal" ],
					"args" : [ 0 ],
					"id" : "obj-19",
					"presentation_rect" : [ 30.0, 240.0, 238.0, 241.490005 ],
					"numinlets" : 1,
					"name" : "sampler7.maxpat",
					"patching_rect" : [ 60.0, 420.0, 238.0, 37.0 ],
					"numoutlets" : 14,
					"presentation" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "print loader",
					"fontname" : "Arial",
					"id" : "obj-6",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 60.0, 255.0, 71.0, 20.0 ],
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "route 0 1 2 3 4 5 6",
					"outlettype" : [ "", "", "", "", "", "", "", "" ],
					"fontname" : "Arial",
					"id" : "obj-5",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 405.0, 420.0, 117.5, 20.0 ],
					"numoutlets" : 8
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "SlicerLedChannel 0",
					"outlettype" : [ "" ],
					"fontname" : "Arial",
					"id" : "obj-3",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 300.0, 495.0, 124.0, 20.0 ],
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "bpatcher",
					"outlettype" : [ "" ],
					"args" : [  ],
					"id" : "obj-2",
					"presentation_rect" : [ 60.0, 0.0, 234.0, 230.0 ],
					"numinlets" : 0,
					"name" : "loader.maxpat",
					"patching_rect" : [ 15.0, 15.0, 219.0, 224.0 ],
					"numoutlets" : 1,
					"presentation" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "mute group control.",
					"fontname" : "Arial",
					"id" : "obj-16",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 90.0, 360.0, 114.909996, 20.0 ],
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "button control.",
					"fontname" : "Arial",
					"id" : "obj-44",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 60.0, 300.0, 150.0, 20.0 ],
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Mute Grouping.",
					"fontname" : "Arial",
					"id" : "obj-48",
					"presentation_rect" : [ 435.0, 0.0, 150.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 630.0, 75.0, 150.0, 20.0 ],
					"numoutlets" : 0,
					"presentation" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Loop Length",
					"fontname" : "Arial",
					"id" : "obj-46",
					"presentation_rect" : [ 345.0, 0.0, 77.399994, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 450.0, 150.0, 150.0, 20.0 ],
					"numoutlets" : 0,
					"presentation" : 1
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-32", 0 ],
					"destination" : [ "obj-5", 0 ],
					"hidden" : 0,
					"midpoints" : [ 84.5, 405.0, 414.5, 405.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-70", 0 ],
					"destination" : [ "obj-71", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-26", 0 ],
					"destination" : [ "obj-15", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-15", 0 ],
					"destination" : [ "obj-17", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-5", 0 ],
					"destination" : [ "obj-3", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-5", 1 ],
					"destination" : [ "obj-7", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-5", 2 ],
					"destination" : [ "obj-11", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-5", 3 ],
					"destination" : [ "obj-10", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-5", 4 ],
					"destination" : [ "obj-13", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-5", 5 ],
					"destination" : [ "obj-12", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-5", 6 ],
					"destination" : [ "obj-14", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-19", 13 ],
					"destination" : [ "obj-14", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-19", 11 ],
					"destination" : [ "obj-12", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-19", 9 ],
					"destination" : [ "obj-13", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-19", 7 ],
					"destination" : [ "obj-10", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-19", 5 ],
					"destination" : [ "obj-11", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-19", 3 ],
					"destination" : [ "obj-7", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-19", 1 ],
					"destination" : [ "obj-3", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-2", 0 ],
					"destination" : [ "obj-19", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-32", 0 ],
					"destination" : [ "obj-19", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-18", 0 ],
					"destination" : [ "obj-32", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-18", 0 ],
					"destination" : [ "obj-5", 0 ],
					"hidden" : 0,
					"midpoints" : [ 71.5, 345.480011, 414.5, 345.480011 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-18", 0 ],
					"destination" : [ "obj-19", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-19", 12 ],
					"destination" : [ "obj-21", 6 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-19", 10 ],
					"destination" : [ "obj-21", 5 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-19", 8 ],
					"destination" : [ "obj-21", 4 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-19", 6 ],
					"destination" : [ "obj-21", 3 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-19", 4 ],
					"destination" : [ "obj-21", 2 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-19", 2 ],
					"destination" : [ "obj-21", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-19", 0 ],
					"destination" : [ "obj-21", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
 ]
	}

}
