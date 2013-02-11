cople
=====

COPLE a set based distributed language.


                                                COPLE
                                 Concurrent Objects Programming LanguagE

                                           by Peter Antoine.

                                       Copyright (c) 1999 P.Antoine.
                                           All rights reserved.


COPLE is an object based programming language. It is designed to work with as yet
non-existing operating system (COBOS) hence the rather silly name. The language is
used to both generate persistent objects, applications and system functions for the
COBOS operating system.

The language is designed for persistent objects that are distributed over more than
one system. These are controlled by virtual structure called a realm. A realm can
be distributed over several systems or nodes.

Anyway, this version is now complete as far as code generation. It is not correct. Code
generated for both the machine specific and the virtual processor have not been totally
tested. Plus, there is no loader/vp for the machine specific code. but, these are things
for the future.

To build this release:

	 	make

That simple!

#To Do:##

1.	 add "dynamic" keyword to the language.
	   To allow for system/application code to be able allocate dynamic memory. 

2.	 add "new", "delete", "renew" to the language.
	   These are as above, the operators for working with the dynamic memory functions. These
	   are used to create, delete and resize memory respectively.

3.	 get the code to be correct.
	   Obvious.

4.	 Floating point code.
	   Need to work this over. Not being generated correctly and the floating point numbers
	   need to be properly formatted. Not sure if to use the floating point instructions
	   yet, most likely will but depends on the design of COBOS version 2.

5.	 Generate the programmer API, and inc files.
	   Again this depends on the design of COBOS version 2.

6.	 Create the assembler for CASM code.
	   Again this depends on how the COBOS version 2 works. This will need to create code
	   for the VM for this OS.
	 

	 	Not much, aye?

##Project Status##

This project has long been abandoned. I have been meaning to revive it for the last 10+ years, but other things
(mostly that I need to build the OS for it - and the distributed processor and computer for it) have got in the
way. This is mostly put here are I am going to be doing some other compiler work and I want access to older code.


