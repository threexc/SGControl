## Generating a IQ Sine Waveform

The following is a series of tests with which a sine wave can be generated
using the E4438C signal generator.

### Generating the IQ data

From the parent directory (SGControl), type

```
make ks_iq
```

then navigate to the bin/ directory. Run the iq executable by typing

```
./iq
```

An iq.txt file should have been created in SGControl/ .

### Control commands

The following commands can be used to condition the E4438C before output begins
(these are not in a prescribed order):

1. :FREQ:MODE FIX - sets the frequency to fixed mode; alternative is a sweep
2. :FREQ:FIX 5.0GHz - specifies the output frequency to be centered at 5.0GHz
3. :FREQ:OFFS:STAT OFF - sets the frequency offset to 0
4. :POW:LEV -10.0DBM - controls the signal generator's attenuator
5. :OUTP:MOD:STAT ON - turns on the RF output
6. :MEM:DATA:IQ "<file_name>"
7. :IQ - initiates an IQ calibration
8. :MEM:CAT:BIN? - outputs a list of binary files on the generator
9. :MEM:CAT:IQ? - outputs a list of IQ files on the generator
10. :MEM:DATA:IQ "<file_name>" <offsetQ>, <num_states>, <i0>, <q0>, <i1>, <q1>, ...
<i(n)>, <q(n)>... - loads an IQ data file into NVWFM. /USER/IQ directory is implied.
Example: :MEM:DATA:IQ "Test_BPSK",1,2,1,0,0,0 loads a two-symbol IQ file named
TEST_BPSK with a Q offset.
11. :MEM:FREE? - returns the amount of free space (in bytes) in the filesystem
12. :MEM:DATA "<file_name>", <data_block> - loads <datablock> into the given filename
13. :SYST:ERR? - returns the most recent error message in the queue
14. :RAD:ARB:GEN:SINE "Sine_Wave",60,.5,IQ - generates I/Q sine wave and saves it to the "Sine Wave" file
