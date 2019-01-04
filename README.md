# Diff Implementation

# About Author

* NAME: Shreyansh Gopawar
* MIS: 111508073
* BRANCH: IT

# How To Run DIFF:

Run "diff" as __./diff <options> <file1/dir1> <file2/dir2>__ where options are:
* treat all files as text (-a)
* ignore space change (-b)
* ignore blank lines (-B)
* ignore case (-i)
* show no details (-q)
* compares files found with similar names inside directory (-r)
* ignore all space (-w)
* ignore trailing and leading spaces, tabs and blank lines (-Z)
* ignore all tabs (-t)
* help (-h)
	
# Description:
![Datastructure](datastructure.png)

The diff command compares two file and tell the least change to be made to make first file exactly same as the second.
The project also compares name of files in Directories and compare the files with same names. The comparision of file names is only one layer which means it will not open another directory inside a directory.
The project also compares Binary files.
One can also compare a non-text file as text by using (-a) option provided.
Output without any details of changes can also be viewed using (-q) option.
There are couple of options provided to compare files by ignoring case changes(-i), space changes(-b), blank line changes(-B), etc.
(-h) option is provided for further help.

# Datastructure:

# Files Included:

* diff.c - Main code
* diff.h - header file
* queue.c - Queue Implementation
* queue.h - Queue header file
* Makefile - Make executable for the project
* README - This file
* test - Files on which code was tested
* screenshots - Results of testing
