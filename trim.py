#!/usr/bin/python3

########################################################################
# This file is part of trim, a tool for converting .srec to .mem
#
# Copyright (C) 2019 The University of Waikato, Hamilton, New Zealand.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.
########################################################################

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
