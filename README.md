# Examples-CPP-Makefile
# Example to understand C++ program with makefile



# Compiling Steps------------------------------------------------
[examples@infonoodle SimpleMakeFileProgram]$ gmake <label_name>
[examples@infonoodle SimpleMakeFileProgram]$ gmake
g++ -g -Wall -o sample src/sample.cc -I.
[examples@infonoodle SimpleMakeFileProgram]$ gmake clean
rm -f sample
[examples@infonoodle SimpleMakeFileProgram]$ gmake all
g++ -g -Wall -o sample src/sample.cc -I.

# Output --------------------------------------------------------
[examples@infonoodle SimpleMakeFileProgramm]$ ./sample
This is sample program with generic make file
[examples@infonoodle SimpleMakeFileProgramm]$
