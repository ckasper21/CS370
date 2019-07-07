README
Chris Kasper
CS 370-001
Programming Assignment 2
-------------------------
Files:

interp.h (from inferno-os/include)
	- added line 266
		- new priority value for Prog struct

xec.c (from inferno-os/libinterp)
	- added lines 1700-1706
		- detect if process got blocked during time slice

dis.c (from inferno-os/emu/port)
	- added highest priority function line 90
		- finds highest priority process in ready list
	- added line 181
		- populated new priority value to Prog struct
		- modified quanta value above too
	- added lines 725-731
		- detect if process got blocked
	- added lines 1113-1122
		- priority scheduler code