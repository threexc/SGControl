# Sample Status Sequence

The following is a sample **status** sequence for querying the instrument about
its current state. It assumes that the user has previously loaded a BIN file
into volatile memory in the instrument, and has turnd on modulation and the RF
output. While there is no formal distinction between status sequences and
control sequences, it is good practice to avoid including control messages in
the former. Copy and paste it into another text file of your choice in order to
target it with ```sg_sequence```.

```
:RAD:CUST:DATA?
:RAD:CUST:STAT?
:FREQ?
:POW?
:OUTP:MOD:STAT?
:OUTP:STAT?
```
