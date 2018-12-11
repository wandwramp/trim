# `trim`

`trim` is a tool to translate .srec files created by wcc or wlink into .mem
files which can be understood by the Vivado synthesis tool.
It was initially written in Python, but a C version is also available.

## Usage

Usage is the same for both versions of `trim`, though the default behaviour is slightly different.

` $ trim input.srec `

This will cause C `trim` to create or overwrite a file called `trim.mem` in 
the working directory. `trim.py` will instead output a file in the same
directory as the input file, with a name identical to the input file except
for the extension being replaced with `.mem`.

To specify an output file, pass the `-o` argument to either program.

` $ trim -o output.mem input.srec `

Both programs will create or overwrite the specified file.

## Building

C `trim` can be built by simply running `make`. Alternatively, give `trim.c`
to your favourite C compiler using your favourite flags.  
`trim.py` will run using either Python3 or Python2.
