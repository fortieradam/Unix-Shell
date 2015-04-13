HAVE NOT IMPLEMENTED
---------------------

	1) Support of Multiple pipes
	2) Wildcard Matching
	3) Alias expansion
	4) Unset Environment Variable
	5) Environment Variable expansion
	6) Combination of piping and I/O.

HAVE IMPLEMENTED
-----------------

	1) Tokenized, parsed, and properly placed all commands, arguments, and operators in the respective data structures
	2) Implemented cd word_directory_name, printenv, alias name word, unalias name, bye
	3) Execution of any binary file existing in a directory on PATH can be executed including arguments
	4) Using a single pipe to link commands
	5) Input redirection from a file to a command
	6) Output redirection to an existing or unexistent file
	7) File input to exe to execute line by line


INSTRUCTIONS
-------------

	MAKEFILE:
	--------
				To compile: "make"

				To run: "./shell"

				To compile and run: "make run"

				To clean: "make clean"

				To zip: "make zip"

				File input: "./shell < file"


