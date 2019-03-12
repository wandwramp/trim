/*
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
*/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define EX_USAGE 		1
#define EX_NOINPUT		2
#define EX_CANTCREAT	3
#define EX_BADSTART		4
#define EX_BADRECORD	5

FILE* input;
FILE* output;

void error(int exitCode)
{
	if ( input != NULL )
		fclose(input);
	if ( output != NULL )
		fclose(output);
	exit(exitCode);
}

void usage(char* argv[])
{
	fprintf(stderr, "USAGE: %s [-o OUTPUT] input\n", argv[0]);
	error(EX_USAGE);
}

void parse_args(int argc, char* argv[])
{
	if ( argc == 1 )
	{
		usage(argv);
	}

	int opt;
	
	// Interpret -o option for output file
	while ( (opt = getopt(argc, argv, "o:")) != -1 )
	{
		if ( opt == 'o' )
		{
			output = fopen(optarg, "w");
			if ( output == NULL ) 
			{
				fprintf(stderr, "Could not open file: %s\n", optarg);
				error(EX_CANTCREAT);
			}
		}
	}

	// If an output file was not specified, open a default output file
	if ( output == NULL )
	{
		output = fopen("trim.mem", "w");
		if ( output == NULL )
		{
			fprintf(stderr, "Could not open file: %s\n", "trim.mem");
			error(EX_CANTCREAT);
		}
	}
	
	// Ensure an input file was specified
	if ( optind >= argc )
	{
		fprintf(stderr, "Please specify an input file.\n");
		error(EX_NOINPUT);
	}

	// Open the input file
	input = fopen(argv[optind], "r");
	if ( input == NULL )
	{
		fprintf(stderr, "Could not open file: %s\n", argv[optind]);
		error(EX_NOINPUT);
	}
}

int main(int argc, char* argv[])
{
	parse_args(argc, argv);

	ssize_t read;
	char* line = NULL;
	char* token;
	size_t len = 0;

	char length[3];
	length[2] = 0;
	int recordLength;

	char data[10];
	data[8] = '\n';
	data[9] = '\0';

	while ( (read = getline(&line, &len, input)) != -1 )
	{
		token = line;
		if ( *token++ != 'S' )
		{
			fprintf(stderr, "Invalid SREC line: Did not start with S.\n");
			error(EX_BADSTART);
		}
		switch ( *token++ )
		{
			case '3':
				// Get the record length
				length[0] = *token++;
				length[1] = *token++;
				recordLength = (int)strtoul(length, NULL, 16);
				// Ignore the length of the address field and checksum
				recordLength -= 5;
				// Skip the address field
				token += 8;

				for ( int i = 0; i < recordLength / 4; i++ )
				{
					for ( int j = 0; j < 8; j++ )
					{
						data[j] = *token++;
					}
					fputs(data, output); 
				}
				break;
			case '7':
				// S7 records set the entrypoint, which is dropped in
				// the output.
				continue;
			default:
				fprintf(stderr, "Invalid SREC line: Unknown record type.\n");
				error(EX_BADRECORD);
		}
	}

	fclose(input);
	fclose(output);
	return 0;
}
