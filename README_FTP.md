## Instructions for using FTP to transfer files to the E4438C

These instructions are taken from Keysight document E4400-90627 with the intent of clarifying the process and reducing the learning curve

**Note 1:** This method may overwrite files if the filenames are not changed between successive transfers

**Note 2:** This will download to non-volatile memory by default (file path must be changed to download to volatile)

1. Type "ftp <device IP>" (you may need to install the ftp package locally first)
2. Press Enter at the "User:" and "Password:" prompts (there are no credentials)
3. To send a file to the E4438C:
  1. If it's a waveform file, type "put <filename.wfm> /user/waveform/<filename1>"
	2. If it's a marker file, type "put <filename.mkr> /user/marker/<filename1>"
	**Note:** This must be followed exactly in order for the files to be compatible and loaded correctly later on. *Do not* add .wfm or .mkr suffixes to the filename on the E4438C!
4. Similarly, to retrieve a file from the E4438C:
  1. If it's a waveform file, type "get /user/waveform/file file1.wfm", where "file1.wfm" is the name it will have once it is on your workstation
	2. If it's a marker file, type "get /user/marker/file file1.mkr"
5. Type "bye" followed by "exit" to end the FTP session with the E4438C
