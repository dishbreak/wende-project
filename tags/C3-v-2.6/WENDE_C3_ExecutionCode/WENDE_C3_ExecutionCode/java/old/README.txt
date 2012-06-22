------------------------------------------------
DEMO INSTRUCTIONS
------------------------------------------------
copy java.zip to your computer

extract zip archive

open two command prompt windows (start->run 'cmd' <enter>) x2

navigate to the directory the archive was extracted to using 'cd' command in both windows

run the following two commands - one in each cmd window order doesn't matter:
	java GetMsg
	java SendMsg

------------------------------------------------
CONFIGURATION
------------------------------------------------
To change configuration settings, open the ./config/net_config.txt file

to see text output change
verbose = true

the server (SendMsg) always needs to have localhost, the client host can point to another computer by changing the host parameter.

mainRunTime determines how long the program runs for in milliseconds