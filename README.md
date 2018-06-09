# SGControl

SGControl contains various C-based tools for operating the Keysight/Agilent
E4438C Signal Generator via LAN, but it should in theory work with any instrument
that supports SCPI (Standard Commands for Programmable Instruments). This set of
utilities is not intended to be compatible with Windows operating systems at the
moment; however, the legacy utilities (in ```ext/keysight```) were designed with
the support for both Unix- and Windows-based platforms (the latter has not been
tested).

The objective of this project is to provide a means to automate the control of
the E4438C for use in a laboratory testing environment while providing usability
features such as command/response logging, support for creating and encoding
raw IQ data files for use as both modulation *and* data targets, and GUI-based 
sequence creation/queueing.


### Project Structure

The current list of subdirectories and their descriptions is as follows:

```doc``` - Guides for creation of control sequences and general signal generator 
usage. Does *not* contain usable sequence files - these are kept in ```test```

```ext``` - Directory for copies of code written by others (e.g. Keysight)

```include``` - Header files for project tools

```src``` - Corresponding source files for project tools

```test``` - Key control sequence files, IQ waveforms for testing (also where the
executable for the echo_server utility gets created)


Additionally, once the executables are built, the following directories may also be
found:

```bin``` - Unused placeholder (will contain executables once a master Python-based
program is implemented)

```lib``` - Placeholder for external libraries


The directory ```ext/keysight``` contains reference copies of the
original code snippets from various Keysight documents (such as document #
E4400-90505), which have been reformatted and tested to confirm functionality. I
make no claim to credit for the content of those files, although they have been
used as inspiration for much of the rest of the project's content.

### Prerequisites and Deployment

SGControl should be compatible with any system supporting recent versions of
the gcc compiler and the C standard libraries.

The code in SGControl has been tested and run from two desktop machines and one
laptop, all of which are/were running Fedora 26 and/or Fedora 27 64-bit at the
time of development. Code was developed in the [Atom](https://atom.io/) text editor,
with ```gcc``` and ```gdb``` used for building and debug.


### Installation

All binaries are written to the project directory by default. There are several
options available for compilation.

To compile everything:

```
make all
```

To compile the main signal generator control tool (command sequencer):

```
make sgseq
```

To compile the encoder/decoder tool (only encoding works at this time):

```
make sg_encdec
```

To compile the original Keysight utilities:

```
make legacy
```

To compile the echo server (debug utility):

```
make echo
```

The option ```make clean``` is also available, as is ```make directories``` if
the user wants to create the build directories independently for some reason.

### Usage

In order to use the sg_sequence tool, the target device must be provided a known
IP on the same network as the controlling machine. A sample signal generator IP
is currently hard-coded in the ```sg_sequence.c``` file, so the IP provided must
be entered in its place before use.

#### Sending Command Sequences

A sample sequence file querying the instrument for status, ```sg_test_commands.txt```,
is contained in the ```test/``` folder. This can be executed by the main program by
typing ```./sg_sequence test/sg_test_commands.txt```. Examples of good practices
for creating other sequence files, as well as sample sequences detailing how to select
and modulate simple data signals are found in the ```doc/``` directory.

#### Encoding and Decoding IQ Data

The encoder tool requires IQ data files be formatted with one IQ point per line,
with a comma and single space separating them like so:

```
1.000, -0.394
0.233, -0.998
0.000, 0.000
-1.000, 1.000

```

Using the ```sg_encdec``` tool on an IQ data file requires that the number of points
in the file be specified, followed by the filename, and finally the -e (encode) or -d
(decode) option. For example, if the above four points were contained in the file 
"random_four", the following would target said file and output a new file known as
"random_four_encoded":

```./sg_encdec 4 random_four -e```

#### Logging

The ```sg_sequence``` tool has command logging built-in, collecting responses received
from the instrument and storing them in a time-stamped file in the top-level directory
(this will change to a logs directory in future versions). This feature relies on use
of query statements (i.e. commands sent to the instrument requesting status of an 
interface, such as the RF output, where all of the commands end with a "?") following
any command sent to modify settings.

### Future Iterations

Development is still active and ongoing. Current priorities include:

* Finishing the ```sg_binenc.c``` and related programs to use for encoding I/Q
data in the E4438C's binary format (currently only supports encoding, not decoding)
* A high-level Python control framework that includes a GUI option

### Authors

* **Trevor Gamblin** - [threexc](https://github.com/threexc)
