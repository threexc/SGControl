# Sample Control Sequence

The following is a sample **control** sequence for loading a BIN file that has
already been transferred to the instrument via FTP (see ```ftp_transfers.md```).
Copy and paste it into another text file of your choice in order to target it
with ```sg_sequence```. Note that, as per ```creating_sequence_files.md```,
each command message is followed by the corresponding status query.

```
:RAD:CUST:DATA "BIN:<file_name>"
:RAD:CUST:DATA?
:RAD:CUST:STAT On
:RAD:CUST:STAT?
:FREQ:FIX 5.765GHZ
:FREQ?
:POW:LEV -10.0DBM
:POW?
:OUTP:MOD:STAT ON
:OUTP:MOD:STAT?
:OUTP:STAT ON
:OUTP:STAT?
```

The explanations for each command are as follows (queries are self-explanatory):

1. ```:RAD:CUST:DATA``` sets the filename specified for use by the Custom
subsystem of the digital real-time testing component (Chapter 8 of E4400-90622)
2. ```:RAD:CUST:STAT``` enables or disables the Custom modulation
3. ```:FREQ:FIX``` sets the output center frequency
4. ```:POW:LEV``` sets the output power level
5. ```:OUTP:MOD:STAT``` enables or disables modulation of the RF output with the
currently active modulation types
6. ```:OUTP:STAT``` enables or disables the RF output

There also exists :RAD:CUST:MOD:TYPE to set the modulation type, which can be
selected from this list (see E4400-90622 page 570):

ASK|BPSK|QPSK|UQPSk|IS95QPSK|GRAYQPSK|OQPSK|IS95OQPSK|P4DQPSK|PSK8|PSK16|D8PSK|
HDQPSK|MSK|FSK2|FSK4|FSK8|FSK16|C4FM|HCPM|QAM4|QAM16|QAM32|QAM64|QAM128|QAM256|
16APSKCR23|16APSKCR34|16APSKCR45|16APSKCR56|16APSKCR89|16APSKCR910|32APSKCR34|
32APSKCR45|32APSKCR56|32APSKCR89|32APSKCR910|UIQ|UFSK
