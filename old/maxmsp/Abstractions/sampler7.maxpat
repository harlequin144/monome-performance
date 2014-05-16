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
		"title" : "Sampler7",
		"boxes" : [ 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "route 0 1 2 3 4 5 6",
					"fontname" : "Arial",
					"numoutlets" : 8,
					"id" : "obj-5",
					"outlettype" : [ "", "", "", "", "", "", "", "" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 630.0, 60.0, 274.5, 20.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"numoutlets" : 0,
					"id" : "obj-10",
					"bgcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"border" : 1,
					"patching_rect" : [ 2445.0, 615.0, 382.003998, 4.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"numoutlets" : 0,
					"id" : "obj-30",
					"bgcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"border" : 1,
					"patching_rect" : [ 2040.0, 615.0, 382.003998, 4.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"numoutlets" : 0,
					"id" : "obj-33",
					"bgcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"border" : 1,
					"patching_rect" : [ 1635.0, 615.0, 382.003998, 4.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "* -1.",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-270",
					"outlettype" : [ "float" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 2475.0, 345.0, 33.0, 20.0 ],
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"fontname" : "Arial",
					"numoutlets" : 2,
					"id" : "obj-271",
					"outlettype" : [ "float", "bang" ],
					"bordercolor" : [ 1.0, 1.0, 1.0, 1.0 ],
					"minimum" : 0.0,
					"presentation_rect" : [ 72.299995, 187.199997, 52.396095, 20.0 ],
					"fontsize" : 12.0,
					"maximum" : 8.0,
					"patching_rect" : [ 2490.0, 300.0, 36.0, 20.0 ],
					"presentation" : 1,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "ubutton",
					"numoutlets" : 4,
					"id" : "obj-273",
					"outlettype" : [ "bang", "bang", "", "int" ],
					"handoff" : "",
					"presentation_rect" : [ 9.0, 185.0, 40.900002, 22.9 ],
					"patching_rect" : [ 2445.0, 300.0, 39.0, 19.0 ],
					"presentation" : 1,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Rev",
					"fontname" : "Arial",
					"numoutlets" : 0,
					"id" : "obj-274",
					"presentation_rect" : [ 9.0, 185.0, 33.200001, 21.0 ],
					"fontsize" : 13.0,
					"patching_rect" : [ 2445.0, 270.0, 42.0, 21.0 ],
					"presentation" : 1,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "* -1.",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-275",
					"outlettype" : [ "float" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 2070.0, 345.0, 33.0, 20.0 ],
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"fontname" : "Arial",
					"numoutlets" : 2,
					"id" : "obj-276",
					"outlettype" : [ "float", "bang" ],
					"bordercolor" : [ 1.0, 1.0, 1.0, 1.0 ],
					"minimum" : 0.0,
					"presentation_rect" : [ 72.299995, 157.199997, 52.396095, 20.0 ],
					"fontsize" : 12.0,
					"maximum" : 8.0,
					"patching_rect" : [ 2085.0, 300.0, 36.0, 20.0 ],
					"presentation" : 1,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "ubutton",
					"numoutlets" : 4,
					"id" : "obj-278",
					"outlettype" : [ "bang", "bang", "", "int" ],
					"handoff" : "",
					"presentation_rect" : [ 9.0, 155.0, 40.900002, 22.9 ],
					"patching_rect" : [ 2040.0, 300.0, 39.0, 19.0 ],
					"presentation" : 1,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Rev",
					"fontname" : "Arial",
					"numoutlets" : 0,
					"id" : "obj-279",
					"presentation_rect" : [ 9.0, 155.0, 33.200001, 21.0 ],
					"fontsize" : 13.0,
					"patching_rect" : [ 2040.0, 270.0, 42.0, 21.0 ],
					"presentation" : 1,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "* -1.",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-280",
					"outlettype" : [ "float" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 1665.0, 345.0, 33.0, 20.0 ],
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"fontname" : "Arial",
					"numoutlets" : 2,
					"id" : "obj-281",
					"outlettype" : [ "float", "bang" ],
					"bordercolor" : [ 1.0, 1.0, 1.0, 1.0 ],
					"minimum" : 0.0,
					"presentation_rect" : [ 72.299995, 127.199997, 52.396095, 20.0 ],
					"fontsize" : 12.0,
					"maximum" : 8.0,
					"patching_rect" : [ 1680.0, 300.0, 36.0, 20.0 ],
					"presentation" : 1,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "ubutton",
					"numoutlets" : 4,
					"id" : "obj-283",
					"outlettype" : [ "bang", "bang", "", "int" ],
					"handoff" : "",
					"presentation_rect" : [ 9.0, 125.0, 40.900002, 22.9 ],
					"patching_rect" : [ 1635.0, 300.0, 39.0, 19.0 ],
					"presentation" : 1,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Rev",
					"fontname" : "Arial",
					"numoutlets" : 0,
					"id" : "obj-284",
					"presentation_rect" : [ 9.0, 125.0, 33.200001, 21.0 ],
					"fontsize" : 13.0,
					"patching_rect" : [ 1635.0, 270.0, 42.0, 21.0 ],
					"presentation" : 1,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "* -1.",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-265",
					"outlettype" : [ "float" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 1260.0, 345.0, 33.0, 20.0 ],
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"fontname" : "Arial",
					"numoutlets" : 2,
					"id" : "obj-266",
					"outlettype" : [ "float", "bang" ],
					"bordercolor" : [ 1.0, 1.0, 1.0, 1.0 ],
					"minimum" : 0.0,
					"presentation_rect" : [ 72.299995, 97.199997, 52.396095, 20.0 ],
					"fontsize" : 12.0,
					"maximum" : 8.0,
					"patching_rect" : [ 1275.0, 300.0, 36.0, 20.0 ],
					"presentation" : 1,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "ubutton",
					"numoutlets" : 4,
					"id" : "obj-268",
					"outlettype" : [ "bang", "bang", "", "int" ],
					"handoff" : "",
					"presentation_rect" : [ 9.0, 95.0, 40.900002, 22.9 ],
					"patching_rect" : [ 1230.0, 300.0, 39.0, 19.0 ],
					"presentation" : 1,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Rev",
					"fontname" : "Arial",
					"numoutlets" : 0,
					"id" : "obj-269",
					"presentation_rect" : [ 9.0, 95.0, 33.200001, 21.0 ],
					"fontsize" : 13.0,
					"patching_rect" : [ 1230.0, 270.0, 42.0, 21.0 ],
					"presentation" : 1,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "* -1.",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-260",
					"outlettype" : [ "float" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 854.999939, 345.0, 33.0, 20.0 ],
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"fontname" : "Arial",
					"numoutlets" : 2,
					"id" : "obj-261",
					"outlettype" : [ "float", "bang" ],
					"bordercolor" : [ 1.0, 1.0, 1.0, 1.0 ],
					"minimum" : 0.0,
					"presentation_rect" : [ 72.299995, 67.199997, 52.396095, 20.0 ],
					"fontsize" : 12.0,
					"maximum" : 8.0,
					"patching_rect" : [ 869.999939, 300.0, 36.0, 20.0 ],
					"presentation" : 1,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "ubutton",
					"numoutlets" : 4,
					"id" : "obj-263",
					"outlettype" : [ "bang", "bang", "", "int" ],
					"handoff" : "",
					"presentation_rect" : [ 9.0, 65.0, 40.900002, 22.9 ],
					"patching_rect" : [ 824.999939, 300.0, 39.0, 19.0 ],
					"presentation" : 1,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Rev",
					"fontname" : "Arial",
					"numoutlets" : 0,
					"id" : "obj-264",
					"presentation_rect" : [ 9.0, 65.0, 33.200001, 21.0 ],
					"fontsize" : 13.0,
					"patching_rect" : [ 824.999939, 270.0, 42.0, 21.0 ],
					"presentation" : 1,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "* -1.",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-255",
					"outlettype" : [ "float" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 449.999939, 345.0, 33.0, 20.0 ],
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"fontname" : "Arial",
					"numoutlets" : 2,
					"id" : "obj-256",
					"outlettype" : [ "float", "bang" ],
					"bordercolor" : [ 1.0, 1.0, 1.0, 1.0 ],
					"minimum" : 0.0,
					"presentation_rect" : [ 72.299995, 37.200001, 52.396095, 20.0 ],
					"fontsize" : 12.0,
					"maximum" : 8.0,
					"patching_rect" : [ 464.999939, 300.0, 36.0, 20.0 ],
					"presentation" : 1,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "ubutton",
					"numoutlets" : 4,
					"id" : "obj-258",
					"outlettype" : [ "bang", "bang", "", "int" ],
					"handoff" : "",
					"presentation_rect" : [ 9.0, 35.0, 40.900002, 22.9 ],
					"patching_rect" : [ 419.999939, 300.0, 39.0, 19.0 ],
					"presentation" : 1,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Rev",
					"fontname" : "Arial",
					"numoutlets" : 0,
					"id" : "obj-259",
					"presentation_rect" : [ 9.0, 35.0, 33.200001, 21.0 ],
					"fontsize" : 13.0,
					"patching_rect" : [ 419.999939, 270.0, 42.0, 21.0 ],
					"presentation" : 1,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "* -1.",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-254",
					"outlettype" : [ "float" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 45.0, 345.0, 33.0, 20.0 ],
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "4",
					"fontname" : "Arial",
					"numoutlets" : 0,
					"id" : "obj-251",
					"underline" : 1,
					"fontface" : 1,
					"fontsize" : 36.0,
					"patching_rect" : [ 1965.0, 165.000015, 42.434006, 48.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "6",
					"fontname" : "Arial",
					"numoutlets" : 0,
					"id" : "obj-252",
					"underline" : 1,
					"fontface" : 1,
					"fontsize" : 36.0,
					"patching_rect" : [ 2775.0, 165.000015, 42.434006, 48.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "5",
					"fontname" : "Arial",
					"numoutlets" : 0,
					"id" : "obj-253",
					"underline" : 1,
					"fontface" : 1,
					"fontsize" : 36.0,
					"patching_rect" : [ 2370.0, 165.000015, 41.103004, 48.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "0",
					"fontname" : "Arial",
					"numoutlets" : 0,
					"id" : "obj-249",
					"underline" : 1,
					"fontface" : 1,
					"fontsize" : 36.0,
					"patching_rect" : [ 345.0, 165.000015, 42.434006, 48.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "3",
					"fontname" : "Arial",
					"numoutlets" : 0,
					"id" : "obj-248",
					"underline" : 1,
					"fontface" : 1,
					"fontsize" : 36.0,
					"patching_rect" : [ 1560.0, 165.000015, 42.434006, 48.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "2",
					"fontname" : "Arial",
					"numoutlets" : 0,
					"id" : "obj-247",
					"underline" : 1,
					"fontface" : 1,
					"fontsize" : 36.0,
					"patching_rect" : [ 1155.0, 165.000015, 42.434006, 48.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "1",
					"fontname" : "Arial",
					"numoutlets" : 0,
					"id" : "obj-246",
					"underline" : 1,
					"fontface" : 1,
					"fontsize" : 36.0,
					"patching_rect" : [ 750.0, 165.000015, 42.434006, 48.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"numoutlets" : 0,
					"id" : "obj-244",
					"bgcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"border" : 1,
					"patching_rect" : [ 1230.0, 615.0, 382.003998, 4.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"numoutlets" : 0,
					"id" : "obj-243",
					"bgcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"border" : 1,
					"patching_rect" : [ 825.0, 615.0, 382.003998, 4.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"numoutlets" : 0,
					"id" : "obj-242",
					"bgcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"border" : 1,
					"patching_rect" : [ 420.0, 615.0, 382.003998, 4.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"numoutlets" : 0,
					"id" : "obj-241",
					"bgcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"border" : 1,
					"patching_rect" : [ 15.0, 615.0, 382.003998, 4.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"numoutlets" : 0,
					"id" : "obj-151",
					"bgcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"border" : 1,
					"patching_rect" : [ 2835.0, 149.999969, 4.0, 443.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "min 0, max 8.",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-152",
					"outlettype" : [ "" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 2445.0, 210.0, 83.0, 18.0 ],
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "loadbang",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-153",
					"outlettype" : [ "bang" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 2445.0, 180.000015, 60.0, 20.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "prepend import",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-156",
					"outlettype" : [ "" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 2685.0, 315.0, 91.0, 20.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "load input\n",
					"fontname" : "Arial",
					"numoutlets" : 0,
					"id" : "obj-157",
					"fontsize" : 12.0,
					"patching_rect" : [ 2700.0, 285.0, 67.0, 20.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Control input",
					"fontname" : "Arial",
					"numoutlets" : 0,
					"id" : "obj-158",
					"fontsize" : 12.0,
					"patching_rect" : [ 2592.0, 178.000015, 150.0, 20.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "output\n",
					"fontname" : "Arial",
					"numoutlets" : 0,
					"id" : "obj-159",
					"fontsize" : 12.0,
					"patching_rect" : [ 2550.0, 570.0, 47.0, 20.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "goto",
					"fontname" : "Arial",
					"numoutlets" : 0,
					"id" : "obj-160",
					"fontsize" : 12.0,
					"patching_rect" : [ 2640.0, 240.0, 33.0, 20.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "sync",
					"fontname" : "Arial",
					"numoutlets" : 0,
					"id" : "obj-161",
					"fontsize" : 12.0,
					"patching_rect" : [ 2685.0, 540.0, 40.0, 20.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "info~ soundfile6",
					"fontname" : "Arial",
					"numoutlets" : 8,
					"id" : "obj-162",
					"outlettype" : [ "float", "list", "float", "float", "float", "float", "float", "" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 2685.0, 375.0, 113.5, 20.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "loop 1",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-163",
					"outlettype" : [ "" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 2475.0, 450.0, 43.0, 18.0 ],
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "loadbang",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-164",
					"outlettype" : [ "bang" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 2475.0, 420.0, 60.0, 20.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "* 10000.",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-165",
					"outlettype" : [ "float" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 2625.0, 315.0, 55.0, 20.0 ],
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "route stop play goto load",
					"fontname" : "Arial",
					"numoutlets" : 5,
					"id" : "obj-166",
					"outlettype" : [ "", "", "", "", "" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 2535.0, 210.0, 197.0, 20.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "play",
					"fontname" : "Arial",
					"numoutlets" : 0,
					"id" : "obj-167",
					"fontsize" : 12.0,
					"patching_rect" : [ 2595.0, 240.0, 34.0, 20.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "stop",
					"fontname" : "Arial",
					"numoutlets" : 0,
					"id" : "obj-168",
					"fontsize" : 12.0,
					"patching_rect" : [ 2550.0, 240.0, 35.0, 20.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "groove~ soundfile6",
					"fontname" : "Arial",
					"numoutlets" : 2,
					"id" : "obj-169",
					"outlettype" : [ "signal", "signal" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 2535.0, 480.0, 112.0, 20.0 ],
					"numinlets" : 3
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "sig~",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-170",
					"outlettype" : [ "signal" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 2535.0, 375.0, 33.0, 20.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "1",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-171",
					"outlettype" : [ "" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 2580.0, 270.0, 32.5, 18.0 ],
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "0",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-172",
					"outlettype" : [ "" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 2535.0, 270.0, 32.5, 18.0 ],
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "inlet",
					"numoutlets" : 1,
					"id" : "obj-173",
					"outlettype" : [ "" ],
					"patching_rect" : [ 630.0, 0.0, 25.0, 25.0 ],
					"numinlets" : 0,
					"comment" : "Channel, command"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "outlet",
					"numoutlets" : 0,
					"id" : "obj-174",
					"patching_rect" : [ 2655.0, 540.0, 25.0, 25.0 ],
					"numinlets" : 1,
					"comment" : "Sync 6"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "outlet",
					"numoutlets" : 0,
					"id" : "obj-176",
					"patching_rect" : [ 2535.0, 540.0, 25.0, 25.0 ],
					"numinlets" : 1,
					"comment" : "Output 6"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "buffer~ soundfile6 10000",
					"fontname" : "Arial",
					"numoutlets" : 2,
					"id" : "obj-177",
					"outlettype" : [ "float", "bang" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 2685.0, 345.0, 143.0, 20.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"numoutlets" : 0,
					"id" : "obj-181",
					"bgcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"border" : 1,
					"patching_rect" : [ 2430.0, 150.000015, 4.0, 443.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "min 0, max 8.",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-182",
					"outlettype" : [ "" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 2040.0, 210.0, 83.0, 18.0 ],
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "loadbang",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-183",
					"outlettype" : [ "bang" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 2040.0, 180.000015, 60.0, 20.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "prepend import",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-186",
					"outlettype" : [ "" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 2280.0, 315.0, 91.0, 20.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "load input\n",
					"fontname" : "Arial",
					"numoutlets" : 0,
					"id" : "obj-187",
					"fontsize" : 12.0,
					"patching_rect" : [ 2295.0, 285.0, 67.0, 20.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Control input",
					"fontname" : "Arial",
					"numoutlets" : 0,
					"id" : "obj-188",
					"fontsize" : 12.0,
					"patching_rect" : [ 2187.0, 178.000015, 150.0, 20.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "output\n",
					"fontname" : "Arial",
					"numoutlets" : 0,
					"id" : "obj-189",
					"fontsize" : 12.0,
					"patching_rect" : [ 2145.0, 570.0, 47.0, 20.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "goto",
					"fontname" : "Arial",
					"numoutlets" : 0,
					"id" : "obj-190",
					"fontsize" : 12.0,
					"patching_rect" : [ 2235.0, 240.0, 33.0, 20.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "sync",
					"fontname" : "Arial",
					"numoutlets" : 0,
					"id" : "obj-191",
					"fontsize" : 12.0,
					"patching_rect" : [ 2280.0, 540.0, 40.0, 20.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "info~ soundfile5",
					"fontname" : "Arial",
					"numoutlets" : 8,
					"id" : "obj-192",
					"outlettype" : [ "float", "list", "float", "float", "float", "float", "float", "" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 2280.0, 375.0, 113.5, 20.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "loop 1",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-193",
					"outlettype" : [ "" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 2070.0, 450.0, 43.0, 18.0 ],
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "loadbang",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-194",
					"outlettype" : [ "bang" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 2070.0, 420.0, 60.0, 20.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "* 10000.",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-195",
					"outlettype" : [ "float" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 2220.0, 315.0, 55.0, 20.0 ],
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "route stop play goto load",
					"fontname" : "Arial",
					"numoutlets" : 5,
					"id" : "obj-196",
					"outlettype" : [ "", "", "", "", "" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 2130.0, 210.0, 197.0, 20.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "play",
					"fontname" : "Arial",
					"numoutlets" : 0,
					"id" : "obj-197",
					"fontsize" : 12.0,
					"patching_rect" : [ 2190.0, 240.0, 34.0, 20.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "stop",
					"fontname" : "Arial",
					"numoutlets" : 0,
					"id" : "obj-198",
					"fontsize" : 12.0,
					"patching_rect" : [ 2145.0, 240.0, 35.0, 20.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "groove~ soundfile5",
					"fontname" : "Arial",
					"numoutlets" : 2,
					"id" : "obj-199",
					"outlettype" : [ "signal", "signal" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 2130.0, 480.0, 112.0, 20.0 ],
					"numinlets" : 3
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "sig~",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-200",
					"outlettype" : [ "signal" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 2130.0, 375.0, 33.0, 20.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "1",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-201",
					"outlettype" : [ "" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 2175.0, 270.0, 32.5, 18.0 ],
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "0",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-202",
					"outlettype" : [ "" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 2130.0, 270.0, 32.5, 18.0 ],
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "outlet",
					"numoutlets" : 0,
					"id" : "obj-204",
					"patching_rect" : [ 2250.0, 540.0, 25.0, 25.0 ],
					"numinlets" : 1,
					"comment" : "Sync 5"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "outlet",
					"numoutlets" : 0,
					"id" : "obj-206",
					"patching_rect" : [ 2130.0, 540.0, 25.0, 25.0 ],
					"numinlets" : 1,
					"comment" : "Output 5"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "buffer~ soundfile5 10000",
					"fontname" : "Arial",
					"numoutlets" : 2,
					"id" : "obj-207",
					"outlettype" : [ "float", "bang" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 2280.0, 345.0, 143.0, 20.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"numoutlets" : 0,
					"id" : "obj-211",
					"bgcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"border" : 1,
					"patching_rect" : [ 2025.0, 150.000015, 4.0, 443.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "min 0, max 8.",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-212",
					"outlettype" : [ "" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 1635.0, 210.0, 83.0, 18.0 ],
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "loadbang",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-213",
					"outlettype" : [ "bang" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 1635.0, 180.000015, 60.0, 20.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "prepend import",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-216",
					"outlettype" : [ "" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 1875.0, 315.0, 91.0, 20.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "load input\n",
					"fontname" : "Arial",
					"numoutlets" : 0,
					"id" : "obj-217",
					"fontsize" : 12.0,
					"patching_rect" : [ 1890.0, 285.0, 67.0, 20.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Control input",
					"fontname" : "Arial",
					"numoutlets" : 0,
					"id" : "obj-218",
					"fontsize" : 12.0,
					"patching_rect" : [ 1782.0, 178.000015, 150.0, 20.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "output\n",
					"fontname" : "Arial",
					"numoutlets" : 0,
					"id" : "obj-219",
					"fontsize" : 12.0,
					"patching_rect" : [ 1740.0, 570.0, 47.0, 20.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "goto",
					"fontname" : "Arial",
					"numoutlets" : 0,
					"id" : "obj-220",
					"fontsize" : 12.0,
					"patching_rect" : [ 1830.0, 240.0, 33.0, 20.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "sync",
					"fontname" : "Arial",
					"numoutlets" : 0,
					"id" : "obj-221",
					"fontsize" : 12.0,
					"patching_rect" : [ 1875.0, 540.0, 40.0, 20.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "info~ soundfile4",
					"fontname" : "Arial",
					"numoutlets" : 8,
					"id" : "obj-222",
					"outlettype" : [ "float", "list", "float", "float", "float", "float", "float", "" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 1875.0, 375.0, 113.5, 20.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "loop 1",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-223",
					"outlettype" : [ "" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 1665.0, 450.0, 43.0, 18.0 ],
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "loadbang",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-224",
					"outlettype" : [ "bang" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 1665.0, 420.0, 60.0, 20.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "* 10000.",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-225",
					"outlettype" : [ "float" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 1815.0, 315.0, 55.0, 20.0 ],
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "route stop play goto load",
					"fontname" : "Arial",
					"numoutlets" : 5,
					"id" : "obj-226",
					"outlettype" : [ "", "", "", "", "" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 1725.0, 210.0, 197.0, 20.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "play",
					"fontname" : "Arial",
					"numoutlets" : 0,
					"id" : "obj-227",
					"fontsize" : 12.0,
					"patching_rect" : [ 1785.0, 240.0, 34.0, 20.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "stop",
					"fontname" : "Arial",
					"numoutlets" : 0,
					"id" : "obj-228",
					"fontsize" : 12.0,
					"patching_rect" : [ 1740.0, 240.0, 35.0, 20.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "groove~ soundfile4",
					"fontname" : "Arial",
					"numoutlets" : 2,
					"id" : "obj-229",
					"outlettype" : [ "signal", "signal" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 1725.0, 480.0, 112.0, 20.0 ],
					"numinlets" : 3
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "sig~",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-230",
					"outlettype" : [ "signal" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 1725.0, 375.0, 33.0, 20.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "1",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-231",
					"outlettype" : [ "" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 1770.0, 270.0, 32.5, 18.0 ],
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "0",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-232",
					"outlettype" : [ "" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 1725.0, 270.0, 32.5, 18.0 ],
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "outlet",
					"numoutlets" : 0,
					"id" : "obj-234",
					"patching_rect" : [ 1845.0, 540.0, 25.0, 25.0 ],
					"numinlets" : 1,
					"comment" : "Sync 4"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "outlet",
					"numoutlets" : 0,
					"id" : "obj-236",
					"patching_rect" : [ 1725.0, 540.0, 25.0, 25.0 ],
					"numinlets" : 1,
					"comment" : "Output 4"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "buffer~ soundfile4 10000",
					"fontname" : "Arial",
					"numoutlets" : 2,
					"id" : "obj-237",
					"outlettype" : [ "float", "bang" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 1875.0, 345.0, 143.0, 20.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"numoutlets" : 0,
					"id" : "obj-61",
					"bgcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"border" : 1,
					"patching_rect" : [ 1620.0, 150.0, 4.0, 443.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "min 0, max 8.",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-62",
					"outlettype" : [ "" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 1230.0, 210.0, 83.0, 18.0 ],
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "loadbang",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-63",
					"outlettype" : [ "bang" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 1230.0, 180.000015, 60.0, 20.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "prepend import",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-66",
					"outlettype" : [ "" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 1470.0, 315.0, 91.0, 20.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "load input\n",
					"fontname" : "Arial",
					"numoutlets" : 0,
					"id" : "obj-67",
					"fontsize" : 12.0,
					"patching_rect" : [ 1485.0, 285.0, 67.0, 20.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Control input",
					"fontname" : "Arial",
					"numoutlets" : 0,
					"id" : "obj-68",
					"fontsize" : 12.0,
					"patching_rect" : [ 1377.0, 178.0, 150.0, 20.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "output\n",
					"fontname" : "Arial",
					"numoutlets" : 0,
					"id" : "obj-69",
					"fontsize" : 12.0,
					"patching_rect" : [ 1335.0, 570.0, 47.0, 20.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "goto",
					"fontname" : "Arial",
					"numoutlets" : 0,
					"id" : "obj-70",
					"fontsize" : 12.0,
					"patching_rect" : [ 1425.0, 240.0, 33.0, 20.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "sync",
					"fontname" : "Arial",
					"numoutlets" : 0,
					"id" : "obj-71",
					"fontsize" : 12.0,
					"patching_rect" : [ 1470.0, 540.0, 40.0, 20.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "info~ soundfile3",
					"fontname" : "Arial",
					"numoutlets" : 8,
					"id" : "obj-72",
					"outlettype" : [ "float", "list", "float", "float", "float", "float", "float", "" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 1470.0, 375.0, 113.5, 20.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "loop 1",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-73",
					"outlettype" : [ "" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 1260.0, 450.0, 43.0, 18.0 ],
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "loadbang",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-74",
					"outlettype" : [ "bang" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 1260.0, 420.0, 60.0, 20.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "* 10000.",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-75",
					"outlettype" : [ "float" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 1410.0, 315.0, 55.0, 20.0 ],
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "route stop play goto load",
					"fontname" : "Arial",
					"numoutlets" : 5,
					"id" : "obj-76",
					"outlettype" : [ "", "", "", "", "" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 1320.0, 210.0, 197.0, 20.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "play",
					"fontname" : "Arial",
					"numoutlets" : 0,
					"id" : "obj-77",
					"fontsize" : 12.0,
					"patching_rect" : [ 1380.0, 240.0, 34.0, 20.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "stop",
					"fontname" : "Arial",
					"numoutlets" : 0,
					"id" : "obj-78",
					"fontsize" : 12.0,
					"patching_rect" : [ 1335.0, 240.0, 35.0, 20.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "groove~ soundfile3",
					"fontname" : "Arial",
					"numoutlets" : 2,
					"id" : "obj-79",
					"outlettype" : [ "signal", "signal" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 1320.0, 480.0, 112.0, 20.0 ],
					"numinlets" : 3
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "sig~",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-80",
					"outlettype" : [ "signal" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 1320.0, 375.0, 33.0, 20.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "1",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-81",
					"outlettype" : [ "" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 1365.0, 270.0, 32.5, 18.0 ],
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "0",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-82",
					"outlettype" : [ "" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 1320.0, 270.0, 32.5, 18.0 ],
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "outlet",
					"numoutlets" : 0,
					"id" : "obj-84",
					"patching_rect" : [ 1440.0, 540.0, 25.0, 25.0 ],
					"numinlets" : 1,
					"comment" : "Sync 3"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "outlet",
					"numoutlets" : 0,
					"id" : "obj-86",
					"patching_rect" : [ 1320.0, 540.0, 25.0, 25.0 ],
					"numinlets" : 1,
					"comment" : "Output 3"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "buffer~ soundfile3 10000",
					"fontname" : "Arial",
					"numoutlets" : 2,
					"id" : "obj-87",
					"outlettype" : [ "float", "bang" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 1470.0, 345.0, 143.0, 20.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"numoutlets" : 0,
					"id" : "obj-91",
					"bgcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"border" : 1,
					"patching_rect" : [ 1215.0, 150.0, 4.0, 443.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "min 0, max 8.",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-92",
					"outlettype" : [ "" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 824.999939, 210.0, 83.0, 18.0 ],
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "loadbang",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-93",
					"outlettype" : [ "bang" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 824.999939, 180.000015, 60.0, 20.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "prepend import",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-96",
					"outlettype" : [ "" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 1065.0, 315.0, 91.0, 20.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "load input\n",
					"fontname" : "Arial",
					"numoutlets" : 0,
					"id" : "obj-97",
					"fontsize" : 12.0,
					"patching_rect" : [ 1080.0, 285.0, 67.0, 20.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Control input",
					"fontname" : "Arial",
					"numoutlets" : 0,
					"id" : "obj-98",
					"fontsize" : 12.0,
					"patching_rect" : [ 972.0, 178.0, 150.0, 20.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "output\n",
					"fontname" : "Arial",
					"numoutlets" : 0,
					"id" : "obj-99",
					"fontsize" : 12.0,
					"patching_rect" : [ 930.0, 570.0, 47.0, 20.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "goto",
					"fontname" : "Arial",
					"numoutlets" : 0,
					"id" : "obj-100",
					"fontsize" : 12.0,
					"patching_rect" : [ 1020.0, 240.0, 33.0, 20.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "sync",
					"fontname" : "Arial",
					"numoutlets" : 0,
					"id" : "obj-101",
					"fontsize" : 12.0,
					"patching_rect" : [ 1065.0, 540.0, 40.0, 20.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "info~ soundfile2",
					"fontname" : "Arial",
					"numoutlets" : 8,
					"id" : "obj-102",
					"outlettype" : [ "float", "list", "float", "float", "float", "float", "float", "" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 1065.0, 375.0, 113.5, 20.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "loop 1",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-103",
					"outlettype" : [ "" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 855.0, 450.0, 43.0, 18.0 ],
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "loadbang",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-104",
					"outlettype" : [ "bang" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 855.0, 420.0, 60.0, 20.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "* 10000.",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-105",
					"outlettype" : [ "float" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 1005.0, 315.0, 55.0, 20.0 ],
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "route stop play goto load",
					"fontname" : "Arial",
					"numoutlets" : 5,
					"id" : "obj-106",
					"outlettype" : [ "", "", "", "", "" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 915.0, 210.0, 197.0, 20.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "play",
					"fontname" : "Arial",
					"numoutlets" : 0,
					"id" : "obj-107",
					"fontsize" : 12.0,
					"patching_rect" : [ 975.0, 240.0, 34.0, 20.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "stop",
					"fontname" : "Arial",
					"numoutlets" : 0,
					"id" : "obj-108",
					"fontsize" : 12.0,
					"patching_rect" : [ 930.0, 240.0, 35.0, 20.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "groove~ soundfile2",
					"fontname" : "Arial",
					"numoutlets" : 2,
					"id" : "obj-109",
					"outlettype" : [ "signal", "signal" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 915.0, 480.0, 112.0, 20.0 ],
					"numinlets" : 3
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "sig~",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-110",
					"outlettype" : [ "signal" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 915.0, 375.0, 33.0, 20.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "1",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-111",
					"outlettype" : [ "" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 960.0, 270.0, 32.5, 18.0 ],
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "0",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-112",
					"outlettype" : [ "" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 915.0, 270.0, 32.5, 18.0 ],
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "outlet",
					"numoutlets" : 0,
					"id" : "obj-114",
					"patching_rect" : [ 1035.0, 540.0, 25.0, 25.0 ],
					"numinlets" : 1,
					"comment" : "Sync 2"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "outlet",
					"numoutlets" : 0,
					"id" : "obj-116",
					"patching_rect" : [ 915.0, 540.0, 25.0, 25.0 ],
					"numinlets" : 1,
					"comment" : "Output 2"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "buffer~ soundfile2 10000",
					"fontname" : "Arial",
					"numoutlets" : 2,
					"id" : "obj-117",
					"outlettype" : [ "float", "bang" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 1065.0, 345.0, 143.0, 20.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"numoutlets" : 0,
					"id" : "obj-13",
					"bgcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"border" : 1,
					"patching_rect" : [ 810.0, 150.000015, 4.0, 443.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "min 0, max 8.",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-24",
					"outlettype" : [ "" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 419.999939, 210.0, 83.0, 18.0 ],
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "loadbang",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-26",
					"outlettype" : [ "bang" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 419.999939, 180.000015, 60.0, 20.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "prepend import",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-35",
					"outlettype" : [ "" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 660.0, 315.0, 91.0, 20.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "load input\n",
					"fontname" : "Arial",
					"numoutlets" : 0,
					"id" : "obj-36",
					"fontsize" : 12.0,
					"patching_rect" : [ 675.0, 285.0, 67.0, 20.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Control input",
					"fontname" : "Arial",
					"numoutlets" : 0,
					"id" : "obj-37",
					"fontsize" : 12.0,
					"patching_rect" : [ 567.0, 178.000015, 150.0, 20.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "output\n",
					"fontname" : "Arial",
					"numoutlets" : 0,
					"id" : "obj-39",
					"fontsize" : 12.0,
					"patching_rect" : [ 525.0, 570.0, 47.0, 20.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "goto",
					"fontname" : "Arial",
					"numoutlets" : 0,
					"id" : "obj-40",
					"fontsize" : 12.0,
					"patching_rect" : [ 615.0, 240.0, 33.0, 20.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "sync",
					"fontname" : "Arial",
					"numoutlets" : 0,
					"id" : "obj-41",
					"fontsize" : 12.0,
					"patching_rect" : [ 660.0, 540.0, 40.0, 20.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "info~ soundfile1",
					"fontname" : "Arial",
					"numoutlets" : 8,
					"id" : "obj-42",
					"outlettype" : [ "float", "list", "float", "float", "float", "float", "float", "" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 660.0, 375.0, 113.5, 20.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "loop 1",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-43",
					"outlettype" : [ "" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 450.0, 450.0, 43.0, 18.0 ],
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "loadbang",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-44",
					"outlettype" : [ "bang" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 450.0, 420.0, 60.0, 20.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "* 10000.",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-45",
					"outlettype" : [ "float" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 600.0, 315.0, 55.0, 20.0 ],
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "route stop play goto load",
					"fontname" : "Arial",
					"numoutlets" : 5,
					"id" : "obj-46",
					"outlettype" : [ "", "", "", "", "" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 510.0, 210.0, 197.0, 20.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "play",
					"fontname" : "Arial",
					"numoutlets" : 0,
					"id" : "obj-47",
					"fontsize" : 12.0,
					"patching_rect" : [ 570.0, 240.0, 34.0, 20.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "stop",
					"fontname" : "Arial",
					"numoutlets" : 0,
					"id" : "obj-48",
					"fontsize" : 12.0,
					"patching_rect" : [ 525.0, 240.0, 35.0, 20.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "groove~ soundfile1",
					"fontname" : "Arial",
					"numoutlets" : 2,
					"id" : "obj-49",
					"outlettype" : [ "signal", "signal" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 510.0, 480.0, 112.0, 20.0 ],
					"numinlets" : 3
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "sig~",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-50",
					"outlettype" : [ "signal" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 510.0, 375.0, 33.0, 20.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "1",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-51",
					"outlettype" : [ "" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 555.0, 270.0, 32.5, 18.0 ],
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "0",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-52",
					"outlettype" : [ "" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 510.0, 270.0, 32.5, 18.0 ],
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "outlet",
					"numoutlets" : 0,
					"id" : "obj-54",
					"patching_rect" : [ 630.0, 540.0, 25.0, 25.0 ],
					"numinlets" : 1,
					"comment" : "Sync 1"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "outlet",
					"numoutlets" : 0,
					"id" : "obj-56",
					"patching_rect" : [ 510.0, 540.0, 25.0, 25.0 ],
					"numinlets" : 1,
					"comment" : "Output 1"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "buffer~ soundfile1 10000",
					"fontname" : "Arial",
					"numoutlets" : 2,
					"id" : "obj-57",
					"outlettype" : [ "float", "bang" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 660.0, 345.0, 143.0, 20.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"numoutlets" : 0,
					"id" : "obj-2",
					"bgcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"border" : 1,
					"patching_rect" : [ 405.0, 150.000015, 4.0, 443.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "min 0, max 8.",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-25",
					"outlettype" : [ "" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 15.0, 210.0, 83.0, 18.0 ],
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "loadbang",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-23",
					"outlettype" : [ "bang" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 15.0, 180.0, 60.0, 20.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"fontname" : "Arial",
					"numoutlets" : 2,
					"id" : "obj-21",
					"outlettype" : [ "float", "bang" ],
					"bordercolor" : [ 1.0, 1.0, 1.0, 1.0 ],
					"minimum" : 0.0,
					"presentation_rect" : [ 72.299995, 7.200001, 52.396095, 20.0 ],
					"fontsize" : 12.0,
					"maximum" : 8.0,
					"patching_rect" : [ 60.0, 300.0, 36.0, 20.0 ],
					"presentation" : 1,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "prepend import",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-1",
					"outlettype" : [ "" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 255.000015, 315.0, 91.0, 20.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "load input\n",
					"fontname" : "Arial",
					"numoutlets" : 0,
					"id" : "obj-12",
					"fontsize" : 12.0,
					"patching_rect" : [ 270.0, 285.0, 67.0, 20.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Control input",
					"fontname" : "Arial",
					"numoutlets" : 0,
					"id" : "obj-7",
					"fontsize" : 12.0,
					"patching_rect" : [ 162.0, 178.000015, 150.0, 20.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "output\n",
					"fontname" : "Arial",
					"numoutlets" : 0,
					"id" : "obj-8",
					"fontsize" : 12.0,
					"patching_rect" : [ 120.0, 570.0, 47.0, 20.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "goto",
					"fontname" : "Arial",
					"numoutlets" : 0,
					"id" : "obj-38",
					"fontsize" : 12.0,
					"patching_rect" : [ 210.0, 240.0, 33.0, 20.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "sync",
					"fontname" : "Arial",
					"numoutlets" : 0,
					"id" : "obj-34",
					"fontsize" : 12.0,
					"patching_rect" : [ 255.000015, 540.0, 40.0, 20.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "info~ soundfile0",
					"fontname" : "Arial",
					"numoutlets" : 8,
					"id" : "obj-32",
					"outlettype" : [ "float", "list", "float", "float", "float", "float", "float", "" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 255.000015, 375.0, 113.5, 20.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "loop 1",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-31",
					"outlettype" : [ "" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 45.0, 450.0, 43.0, 18.0 ],
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "loadbang",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-29",
					"outlettype" : [ "bang" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 45.0, 420.0, 60.0, 20.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "* 10000.",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-28",
					"outlettype" : [ "float" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 195.0, 315.0, 55.0, 20.0 ],
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "route stop play goto load",
					"fontname" : "Arial",
					"numoutlets" : 5,
					"id" : "obj-27",
					"outlettype" : [ "", "", "", "", "" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 105.0, 210.0, 197.0, 20.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "play",
					"fontname" : "Arial",
					"numoutlets" : 0,
					"id" : "obj-20",
					"fontsize" : 12.0,
					"patching_rect" : [ 165.0, 240.0, 34.0, 20.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "stop",
					"fontname" : "Arial",
					"numoutlets" : 0,
					"id" : "obj-15",
					"fontsize" : 12.0,
					"patching_rect" : [ 120.0, 240.0, 35.0, 20.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "groove~ soundfile0",
					"fontname" : "Arial",
					"numoutlets" : 2,
					"id" : "obj-18",
					"outlettype" : [ "signal", "signal" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 105.0, 480.0, 112.0, 20.0 ],
					"numinlets" : 3
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "sig~",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-17",
					"outlettype" : [ "signal" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 105.0, 375.0, 33.0, 20.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "1",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-16",
					"outlettype" : [ "" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 150.0, 270.0, 32.5, 18.0 ],
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "0",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-14",
					"outlettype" : [ "" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 105.0, 270.0, 32.5, 18.0 ],
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "outlet",
					"numoutlets" : 0,
					"id" : "obj-6",
					"patching_rect" : [ 225.0, 540.0, 25.0, 25.0 ],
					"numinlets" : 1,
					"comment" : ""
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "outlet",
					"numoutlets" : 0,
					"id" : "obj-4",
					"patching_rect" : [ 105.0, 540.0, 25.0, 25.0 ],
					"numinlets" : 1,
					"comment" : "Output 0"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "buffer~ soundfile0 10000",
					"fontname" : "Arial",
					"numoutlets" : 2,
					"id" : "obj-3",
					"outlettype" : [ "float", "bang" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 255.000015, 345.0, 143.0, 20.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "ubutton",
					"numoutlets" : 4,
					"id" : "obj-11",
					"outlettype" : [ "bang", "bang", "", "int" ],
					"handoff" : "",
					"presentation_rect" : [ 9.0, 5.0, 40.900002, 22.9 ],
					"patching_rect" : [ 15.0, 300.0, 39.0, 19.0 ],
					"presentation" : 1,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"numoutlets" : 0,
					"id" : "obj-272",
					"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
					"presentation_rect" : [ 70.700005, 36.099998, 62.900005, 22.9 ],
					"border" : 1,
					"patching_rect" : [ 2490.0, 240.0, 39.0, 19.0 ],
					"presentation" : 1,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"numoutlets" : 0,
					"id" : "obj-277",
					"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
					"presentation_rect" : [ 70.700005, 66.099998, 62.900005, 22.9 ],
					"border" : 1,
					"patching_rect" : [ 2085.0, 240.0, 39.0, 19.0 ],
					"presentation" : 1,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"numoutlets" : 0,
					"id" : "obj-282",
					"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
					"presentation_rect" : [ 70.700005, 96.099998, 62.900005, 22.9 ],
					"border" : 1,
					"patching_rect" : [ 1680.0, 240.0, 39.0, 19.0 ],
					"presentation" : 1,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"numoutlets" : 0,
					"id" : "obj-267",
					"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
					"presentation_rect" : [ 70.700005, 126.100006, 62.900005, 22.9 ],
					"border" : 1,
					"patching_rect" : [ 1275.0, 240.0, 39.0, 19.0 ],
					"presentation" : 1,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"numoutlets" : 0,
					"id" : "obj-262",
					"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
					"presentation_rect" : [ 70.700005, 156.100006, 62.900005, 22.9 ],
					"border" : 1,
					"patching_rect" : [ 869.999939, 240.0, 39.0, 19.0 ],
					"presentation" : 1,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"numoutlets" : 0,
					"id" : "obj-257",
					"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
					"presentation_rect" : [ 70.700005, 186.100006, 62.900005, 22.9 ],
					"border" : 1,
					"patching_rect" : [ 464.999939, 240.0, 39.0, 19.0 ],
					"presentation" : 1,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"numoutlets" : 0,
					"id" : "obj-22",
					"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
					"presentation_rect" : [ 70.700005, 6.1, 62.900005, 22.9 ],
					"border" : 1,
					"patching_rect" : [ 60.0, 240.0, 39.0, 19.0 ],
					"presentation" : 1,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Rev",
					"fontname" : "Arial",
					"numoutlets" : 0,
					"id" : "obj-19",
					"presentation_rect" : [ 9.0, 5.0, 33.200001, 21.0 ],
					"fontsize" : 13.0,
					"patching_rect" : [ 15.0, 270.0, 42.0, 21.0 ],
					"presentation" : 1,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"numoutlets" : 0,
					"id" : "obj-287",
					"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
					"presentation_rect" : [ 9.0, 35.0, 48.600006, 22.9 ],
					"border" : 1,
					"patching_rect" : [ 30.100008, 196.100006, 39.0, 19.0 ],
					"presentation" : 1,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"numoutlets" : 0,
					"id" : "obj-288",
					"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
					"presentation_rect" : [ 9.0, 65.0, 48.600006, 22.9 ],
					"border" : 1,
					"patching_rect" : [ 30.100008, 226.100006, 39.0, 19.0 ],
					"presentation" : 1,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"numoutlets" : 0,
					"id" : "obj-289",
					"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
					"presentation_rect" : [ 9.0, 95.0, 48.600006, 22.9 ],
					"border" : 1,
					"patching_rect" : [ 30.100008, 256.100006, 39.0, 19.0 ],
					"presentation" : 1,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"numoutlets" : 0,
					"id" : "obj-290",
					"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
					"presentation_rect" : [ 9.0, 125.0, 48.600006, 22.9 ],
					"border" : 1,
					"patching_rect" : [ 30.100008, 286.100037, 39.0, 19.0 ],
					"presentation" : 1,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"numoutlets" : 0,
					"id" : "obj-291",
					"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
					"presentation_rect" : [ 9.0, 155.0, 48.600006, 22.9 ],
					"border" : 1,
					"patching_rect" : [ 30.100008, 316.100037, 39.0, 19.0 ],
					"presentation" : 1,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"numoutlets" : 0,
					"id" : "obj-292",
					"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
					"presentation_rect" : [ 9.0, 185.0, 48.600006, 22.9 ],
					"border" : 1,
					"patching_rect" : [ 30.100008, 346.100037, 39.0, 19.0 ],
					"presentation" : 1,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"numoutlets" : 0,
					"id" : "obj-293",
					"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
					"presentation_rect" : [ 9.0, 5.0, 48.600006, 22.9 ],
					"border" : 1,
					"patching_rect" : [ 30.100008, 166.100006, 39.0, 19.0 ],
					"presentation" : 1,
					"numinlets" : 1
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-109", 1 ],
					"destination" : [ "obj-114", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-49", 1 ],
					"destination" : [ "obj-54", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-169", 1 ],
					"destination" : [ "obj-174", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-199", 1 ],
					"destination" : [ "obj-204", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-229", 1 ],
					"destination" : [ "obj-234", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-79", 1 ],
					"destination" : [ "obj-84", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-165", 0 ],
					"destination" : [ "obj-169", 0 ],
					"hidden" : 0,
					"midpoints" : [ 2634.5, 431.0, 2544.5, 431.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-163", 0 ],
					"destination" : [ "obj-169", 0 ],
					"hidden" : 0,
					"midpoints" : [ 2484.5, 476.0, 2544.5, 476.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-170", 0 ],
					"destination" : [ "obj-169", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-169", 0 ],
					"destination" : [ "obj-176", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-195", 0 ],
					"destination" : [ "obj-199", 0 ],
					"hidden" : 0,
					"midpoints" : [ 2229.5, 431.0, 2139.5, 431.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-193", 0 ],
					"destination" : [ "obj-199", 0 ],
					"hidden" : 0,
					"midpoints" : [ 2079.5, 476.0, 2139.5, 476.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-200", 0 ],
					"destination" : [ "obj-199", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-199", 0 ],
					"destination" : [ "obj-206", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-229", 0 ],
					"destination" : [ "obj-236", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-230", 0 ],
					"destination" : [ "obj-229", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-223", 0 ],
					"destination" : [ "obj-229", 0 ],
					"hidden" : 0,
					"midpoints" : [ 1674.5, 476.0, 1734.5, 476.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-225", 0 ],
					"destination" : [ "obj-229", 0 ],
					"hidden" : 0,
					"midpoints" : [ 1824.5, 431.0, 1734.5, 431.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-75", 0 ],
					"destination" : [ "obj-79", 0 ],
					"hidden" : 0,
					"midpoints" : [ 1419.5, 431.0, 1329.5, 431.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-73", 0 ],
					"destination" : [ "obj-79", 0 ],
					"hidden" : 0,
					"midpoints" : [ 1269.5, 476.0, 1329.5, 476.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-80", 0 ],
					"destination" : [ "obj-79", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-79", 0 ],
					"destination" : [ "obj-86", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-109", 0 ],
					"destination" : [ "obj-116", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-110", 0 ],
					"destination" : [ "obj-109", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-103", 0 ],
					"destination" : [ "obj-109", 0 ],
					"hidden" : 0,
					"midpoints" : [ 864.5, 476.0, 924.5, 476.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-105", 0 ],
					"destination" : [ "obj-109", 0 ],
					"hidden" : 0,
					"midpoints" : [ 1014.5, 431.0, 924.5, 431.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-49", 0 ],
					"destination" : [ "obj-56", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-50", 0 ],
					"destination" : [ "obj-49", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-43", 0 ],
					"destination" : [ "obj-49", 0 ],
					"hidden" : 0,
					"midpoints" : [ 459.5, 476.0, 519.5, 476.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-45", 0 ],
					"destination" : [ "obj-49", 0 ],
					"hidden" : 0,
					"midpoints" : [ 609.5, 431.0, 519.5, 431.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-18", 1 ],
					"destination" : [ "obj-6", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-28", 0 ],
					"destination" : [ "obj-18", 0 ],
					"hidden" : 0,
					"midpoints" : [ 204.5, 431.0, 114.5, 431.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-31", 0 ],
					"destination" : [ "obj-18", 0 ],
					"hidden" : 0,
					"midpoints" : [ 54.5, 476.0, 114.5, 476.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-17", 0 ],
					"destination" : [ "obj-18", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-18", 0 ],
					"destination" : [ "obj-4", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-25", 0 ],
					"destination" : [ "obj-21", 0 ],
					"hidden" : 0,
					"midpoints" : [ 24.5, 255.0, 57.0, 255.0, 57.0, 294.0, 69.5, 294.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-23", 0 ],
					"destination" : [ "obj-25", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-21", 0 ],
					"destination" : [ "obj-17", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-27", 3 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-16", 0 ],
					"destination" : [ "obj-17", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-29", 0 ],
					"destination" : [ "obj-31", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-14", 0 ],
					"destination" : [ "obj-17", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-27", 2 ],
					"destination" : [ "obj-28", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-27", 1 ],
					"destination" : [ "obj-16", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-27", 0 ],
					"destination" : [ "obj-14", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-46", 0 ],
					"destination" : [ "obj-52", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-46", 1 ],
					"destination" : [ "obj-51", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-46", 2 ],
					"destination" : [ "obj-45", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-52", 0 ],
					"destination" : [ "obj-50", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-44", 0 ],
					"destination" : [ "obj-43", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-51", 0 ],
					"destination" : [ "obj-50", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-46", 3 ],
					"destination" : [ "obj-35", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-26", 0 ],
					"destination" : [ "obj-24", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-63", 0 ],
					"destination" : [ "obj-62", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-76", 3 ],
					"destination" : [ "obj-66", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-81", 0 ],
					"destination" : [ "obj-80", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-74", 0 ],
					"destination" : [ "obj-73", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-82", 0 ],
					"destination" : [ "obj-80", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-76", 2 ],
					"destination" : [ "obj-75", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-76", 1 ],
					"destination" : [ "obj-81", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-76", 0 ],
					"destination" : [ "obj-82", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-106", 0 ],
					"destination" : [ "obj-112", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-106", 1 ],
					"destination" : [ "obj-111", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-106", 2 ],
					"destination" : [ "obj-105", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-112", 0 ],
					"destination" : [ "obj-110", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-104", 0 ],
					"destination" : [ "obj-103", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-111", 0 ],
					"destination" : [ "obj-110", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-106", 3 ],
					"destination" : [ "obj-96", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-93", 0 ],
					"destination" : [ "obj-92", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-153", 0 ],
					"destination" : [ "obj-152", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-166", 3 ],
					"destination" : [ "obj-156", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-171", 0 ],
					"destination" : [ "obj-170", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-164", 0 ],
					"destination" : [ "obj-163", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-172", 0 ],
					"destination" : [ "obj-170", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-166", 2 ],
					"destination" : [ "obj-165", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-166", 1 ],
					"destination" : [ "obj-171", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-166", 0 ],
					"destination" : [ "obj-172", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-183", 0 ],
					"destination" : [ "obj-182", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-196", 3 ],
					"destination" : [ "obj-186", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-201", 0 ],
					"destination" : [ "obj-200", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-194", 0 ],
					"destination" : [ "obj-193", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-202", 0 ],
					"destination" : [ "obj-200", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-196", 2 ],
					"destination" : [ "obj-195", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-196", 1 ],
					"destination" : [ "obj-201", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-196", 0 ],
					"destination" : [ "obj-202", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-226", 0 ],
					"destination" : [ "obj-232", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-226", 1 ],
					"destination" : [ "obj-231", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-226", 2 ],
					"destination" : [ "obj-225", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-232", 0 ],
					"destination" : [ "obj-230", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-224", 0 ],
					"destination" : [ "obj-223", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-231", 0 ],
					"destination" : [ "obj-230", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-226", 3 ],
					"destination" : [ "obj-216", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-213", 0 ],
					"destination" : [ "obj-212", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-1", 0 ],
					"destination" : [ "obj-3", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-35", 0 ],
					"destination" : [ "obj-57", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-57", 1 ],
					"destination" : [ "obj-42", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-42", 6 ],
					"destination" : [ "obj-45", 1 ],
					"hidden" : 0,
					"midpoints" : [ 750.5, 405.0, 657.0, 405.0, 657.0, 312.0, 645.5, 312.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-3", 1 ],
					"destination" : [ "obj-32", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-32", 6 ],
					"destination" : [ "obj-28", 1 ],
					"hidden" : 0,
					"midpoints" : [ 345.5, 405.0, 252.0, 405.0, 252.0, 312.0, 240.5, 312.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-96", 0 ],
					"destination" : [ "obj-117", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-117", 1 ],
					"destination" : [ "obj-102", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-102", 6 ],
					"destination" : [ "obj-105", 1 ],
					"hidden" : 0,
					"midpoints" : [ 1155.5, 405.0, 1062.0, 405.0, 1062.0, 312.0, 1050.5, 312.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-66", 0 ],
					"destination" : [ "obj-87", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-87", 1 ],
					"destination" : [ "obj-72", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-72", 6 ],
					"destination" : [ "obj-75", 1 ],
					"hidden" : 0,
					"midpoints" : [ 1560.5, 405.0, 1467.0, 405.0, 1467.0, 312.0, 1455.5, 312.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-216", 0 ],
					"destination" : [ "obj-237", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-237", 1 ],
					"destination" : [ "obj-222", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-222", 6 ],
					"destination" : [ "obj-225", 1 ],
					"hidden" : 0,
					"midpoints" : [ 1965.5, 405.0, 1872.0, 405.0, 1872.0, 312.0, 1860.5, 312.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-186", 0 ],
					"destination" : [ "obj-207", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-207", 1 ],
					"destination" : [ "obj-192", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-192", 6 ],
					"destination" : [ "obj-195", 1 ],
					"hidden" : 0,
					"midpoints" : [ 2370.5, 405.0, 2277.0, 405.0, 2277.0, 312.0, 2265.5, 312.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-156", 0 ],
					"destination" : [ "obj-177", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-177", 1 ],
					"destination" : [ "obj-162", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-162", 6 ],
					"destination" : [ "obj-165", 1 ],
					"hidden" : 0,
					"midpoints" : [ 2775.5, 405.0, 2682.0, 405.0, 2682.0, 312.0, 2670.5, 312.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-21", 0 ],
					"destination" : [ "obj-254", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-11", 1 ],
					"destination" : [ "obj-254", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-254", 0 ],
					"destination" : [ "obj-17", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-258", 1 ],
					"destination" : [ "obj-255", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-256", 0 ],
					"destination" : [ "obj-255", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-261", 0 ],
					"destination" : [ "obj-260", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-263", 1 ],
					"destination" : [ "obj-260", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-268", 1 ],
					"destination" : [ "obj-265", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-266", 0 ],
					"destination" : [ "obj-265", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-265", 0 ],
					"destination" : [ "obj-80", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-266", 0 ],
					"destination" : [ "obj-80", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-260", 0 ],
					"destination" : [ "obj-110", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-261", 0 ],
					"destination" : [ "obj-110", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-255", 0 ],
					"destination" : [ "obj-50", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-256", 0 ],
					"destination" : [ "obj-50", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-24", 0 ],
					"destination" : [ "obj-256", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-92", 0 ],
					"destination" : [ "obj-261", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-62", 0 ],
					"destination" : [ "obj-266", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-273", 1 ],
					"destination" : [ "obj-270", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-271", 0 ],
					"destination" : [ "obj-270", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-276", 0 ],
					"destination" : [ "obj-275", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-278", 1 ],
					"destination" : [ "obj-275", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-283", 1 ],
					"destination" : [ "obj-280", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-281", 0 ],
					"destination" : [ "obj-280", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-280", 0 ],
					"destination" : [ "obj-230", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-281", 0 ],
					"destination" : [ "obj-230", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-212", 0 ],
					"destination" : [ "obj-281", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-182", 0 ],
					"destination" : [ "obj-276", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-275", 0 ],
					"destination" : [ "obj-200", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-276", 0 ],
					"destination" : [ "obj-200", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-152", 0 ],
					"destination" : [ "obj-271", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-270", 0 ],
					"destination" : [ "obj-170", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-271", 0 ],
					"destination" : [ "obj-170", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-5", 0 ],
					"destination" : [ "obj-27", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-5", 1 ],
					"destination" : [ "obj-46", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-5", 2 ],
					"destination" : [ "obj-106", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-5", 3 ],
					"destination" : [ "obj-76", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-5", 4 ],
					"destination" : [ "obj-226", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-5", 5 ],
					"destination" : [ "obj-196", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-5", 6 ],
					"destination" : [ "obj-166", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-173", 0 ],
					"destination" : [ "obj-5", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
 ]
	}

}
