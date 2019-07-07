README
Chris Kasper
CS 370-001
Programming Assignment 3
-------------------------
Files:

alloc.c (from inferno-os/emu/port)
	- added lines 21-42
		- slab rounded for requests <= 4K bytes
	- added line 63
		- pool slab pointers
	- added lines 344-380
		- takes blocks from slab list
	- added lines 540-585
		- store blocks <= 4K bytes in our slab list
	- added line 1072
		- added slab block description for audit
	- added line 1141
		- makes sure slab blocks aren't treated as corrupt or "bad magic"
	