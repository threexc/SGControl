# SGControl

SGControl contains various C-based control programs for the Keysight/Agilent
E4438C Signal Generator. These tools provide a means to perform LAN-based
control by sending SCPI (Standard Commands for Programmable Instruments). Note
that the directory "ext/keysight" contains reference copies of the original
code snippets from various Keysight documents (such as document #E4400-90505),
which have been reformatted and tested to confirm functionality. I make no
claim to ownership for the content of those files, although they have been used
as inspiration for much of the rest of the project's content.

This set of utilities is not intended to be compatible with Windows operating
systems at the moment; however, the legacy utilities (in "ext/keysight") in
theory support both Unix- and Windows-based platforms (the latter has not been
tested).

### Installation

All binaries are written to the project directory by default. There are several
options available for compilation.

To compile the original Keysight utilities:

```
make legacy
```

To compile the main signal generator control tool (command sequencer):

```
make sg_sequence
```

To compile everything:

```
make all
```

The option "make clean" is also available, and further options will be added
in the future to support encoding I/Q data in the E4438C's preferred format
(16-bit, Big-Endian signed integers).

### Prerequisites

SGControl should be compatible with any system supporting recent versions of
the gcc compiler and the C standard libraries.

### Authors

* **Trevor Gamblin** - [threexc](https://github.com/threexc)
