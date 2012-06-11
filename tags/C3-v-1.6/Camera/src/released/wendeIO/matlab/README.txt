To use this MATLAB driver for Camera Messages use the netRun.m file

This is broken into three sections.  The first section initializes the java code.  You will need to change the java path before running.

The second block is where everything happens.  Run this as many times as needed for your testing.  Make changes to timing etc.

The last block is supposed to close out the network connections.  THis seems to work, but MATLAB currently (6-10-2012) crashes when you re-run the initialize.
I am still working on correcting this.

Let me know if you have any suggestions/questions.

Thank you,

Jonathan Ford