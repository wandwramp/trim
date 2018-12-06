#!/usr/bin/python3

# Daniel Oosterwijk, University of Waikato 2018
# Turns an srec file, as given by wlink, into raw opcodes to be understood
# by the Vivado synthesis tool.

import argparse
import os

parser = argparse.ArgumentParser(description='Translate an .srec file into its raw instruction data. This drops all information about where a program is stored in memory, and outputs as a contiguous stream of instructions.')
parser.add_argument('input', help='Specify input file')
parser.add_argument('-o', '--output', help='Specify output file. Defaults to the name of the input file with its extension changed to .mem')
args = parser.parse_args()

if args.output is None:
    base = os.path.splitext(args.input)[0]
    args.output = base + ".mem"

with open(args.input, 'r') as input:
    content = input.readlines()
with open(args.output, 'w') as output:
    for line in content:
        loc = line[4:11]
        data = line[12:-3]
        if len(data) == 0:
            continue

        words = [data[i:i+8] for i in range(0, len(line), 8)]
        for w in words:
            if len(w) == 0:
                continue
            output.write(w + "\n")
