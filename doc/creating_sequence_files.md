# Creating Sequence Files

Creating a sequence file for commanding the signal generator to configure
various settings should follow three simple rules:

1. Structure the sequence file by entering one full command per line (this
	means that you should avoid piping BIN or BIT data directly to the control
	program), and ensure that there are no misspelled characters or extra
	whitespace
2. Follow the standard 

    ```:<SYS_CMD>:<PARAM_1>:<PARAM_2>:...:<PARAM_N> <VAL>``` 

	and/or
	
    ```:<SYS_CMD>:<PARAM>?```
	
	SCPL syntax convention **exactly**
3. Whenever possible, follow a command that changes a setting by a command that
	queries the instrument for that setting. This is to support debug, as the
	output from queries is stored in the log file time-stamped according to when
	you run a sequence file with the sg_sequence executable

### Example 1 (bad)

:RAD:CUST:STAT On\
:FREQ:FIX 2.5GHZ\
:POW:LEV -10.0DBM

and so on

### Example 2 (good)

:RAD:CUST:STAT On\
:RAD:CUST:STAT?\
:FREQ:FIX 2.5GHZ\
:FREQ?\
:POW:LEV -10.0DBM\
:POW?

etc.
