# SGControl

SGControl contains various C-based tools for operating the Keysight/Agilent
E4438C Signal Generator via LAN, but it should in theory work with any instrument
that supports SCPI (Standard Commands for Programmable Instruments). This set of
utilities is not intended to be compatible with Windows operating systems at the
moment; however, the legacy utilities (in ```ext/keysight```) were designed with
the support for both Unix- and Windows-based platforms (the latter has not been
tested).

Note that the directory ```ext/keysight``` contains reference copies of the
original code snippets from various Keysight documents (such as document #
E4400-90505), which have been reformatted and tested to confirm functionality. I
make no claim to credit for the content of those files, although they have been
used as inspiration for much of the rest of the project's content.

### Prerequisites and Deployment

SGControl should be compatible with any system supporting recent versions of
the gcc compiler and the C standard libraries.

The code in SGControl has been tested and run from two desktop machines and one
laptop, all of which are/were running Fedora 26 and/or Fedora 27 64-bit at the
time of development.

In order to use the sg_sequence tool, the target device must be provided a known
IP on the same network as the controlling machine. A sample signal generator IP
is currently hard-coded in the ```sg_sequence.c``` file, so the IP provided must
be entered in its place before use.

### Installation

All binaries are written to the project directory by default. There are several
options available for compilation.


To compile the main signal generator control tool (command sequencer):

```
make sgseq
```

To compile the original Keysight utilities:

```
make legacy
```

To compile everything:

```
make all
```

To compile the echo server (debug utility):

```
make echo
```

The option ```make clean``` is also available, as is ```make directories``` if
the user wants to create the build directories independently for some reason.
Further options will be added in the future to support encoding I/Q data in the
E4438C's preferred format (16-bit, Big-Endian signed integers). Note that
there is a phony rule in the Makefile that creates build and lib directories as
necessary before compilation.

### Usage

A sample sequence file querying the instrument for status, ```sg_test_commands.txt```,
is contained in the ```test/``` folder. This can be executed by the main program by
typing ```./sg_sequence test/sg_test_commands.txt```. Examples of good practices
for creating other sequence files are found in the ```doc/``` directory.

### Future Iterations

Pending features to the SGControl repository include:

* Finishing the ```sg_binenc.c``` and related programs to use for encoding I/Q
data in the E4438C's binary format
* A higher-level (possibly Python) script that can be set to loop command sequences,
enabling more complex command timings (and an accompanying move of executables to a
```bin/``` directory
* A GUI variant

### Authors

* **Trevor Gamblin** - [threexc](https://github.com/threexc)
