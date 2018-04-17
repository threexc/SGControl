# SGControl

SGControl contains various C- and Python-based control scripts for the
Keysight/Agilent E4438C Signal Generator. These tools are for LAN-based control.
Note that the files getopt.c and lanio.c contained in ext/keysight are copies of
the code originally presented in the Keysight document number E4400-90505, and
I make no claim for credit for them other than their translation into a usable
format.

### Installation

All binaries are written to the bin directory by default.

To compile the lanio service, type:

```
make lanio
```

To compile the echo_server (for testing purposes), type:
```
make echo
```

The command "make clean" is also available to remove the binaries.
