/*******************************************************************
 *net_lib.h: Function definitions to be used with netmon.c
 *
 *Author:    Dwayne Hoy / DoogeJ
 *
 *Date:      September 09 2005
 *
 *Version:   0.5.2
 *
 *Modified:  December 30 2015
 *******************************************************************/
#ifndef _NET_LIB
#define _NET_LIB

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/file.h>
#include <time.h>
#include <pthread.h>

#include "net_data.h"
#include "config_lib.h"

#define DEBUG 0

/*******************************************************************
 *void display(): Prints values stored in Ndata to the screen with
 *                a table like format, before each print to screen
 *                it flushes the console.
 *
 *Recieves:       net_t array, int size of the array, and a double
 *                time value
 *Returns:        NA
 */
void display(net_t *Ndata, int size ,double time);

/*******************************************************************
 *void clearsrceen(): Clears previous text from the console screen
 *
 *Recieves:           NA
 *
 *Returns:            NA
 */
void clearscreen();
/*******************************************************************
 *int parse_args(): Parses the command line arguments and
 *                  performs the appropriate operations for the
 *                  arguments.
 *
 *Recieves:         int number of arguments, char pointer to
 *                  argument vector.
 *
 *Returns:          A int value which tells the caller what
 *                  argument was found
 */
int parse_args(int argc, char* argv[]);

/*******************************************************************
 *long double to_unit(): Modifies a long number to a larger unit
 *                       and changes the it's unit value to that
 *                       unit
 *
 *Recieves:              char* to a string holding the units
 *                       long long value to be modified
 *
 *Returns:               long double value that is the modified
 *                       long long value
 */
long double to_unit(char *unit, long double bval, long double gval ,
		    int mode);

/*******************************************************************
 *int checkfile(): Checks that a file isn't a symbolic link for
 *                 security reasons
 *
 *Recieves:        char* holding the path to the file to be checked
 *
 *Returns:         int value -1 if check failed, 0 if check passed
 */
int checkfile(char *path);

/*******************************************************************
 *int ver_check(): Checks that the dev file is the correct version.
 *
 *Recieves:        FILE* to the dev file stream.
 *
 *Returns:         int value -1 if version was incorrect, 0
 *                 otherwise.
 */
int ver_check(FILE *dev_file);

/*******************************************************************
 *void remove_newline(): Removes first new line from a string
 *
 *Recieves:              char* to the string to be modified
 *
 *Returns:               NA
 */
void remove_newline(char* string);

/*******************************************************************
 *int write_to_log(): Writes data from net_t array to a file
 *
 *Recieves:           net_t array holding the data to be written
 *                    char* to the path of the file
 *                    int size of the array
 *
 *Returns:            int value -1 if write failed, 0 otherwise
 */
int write_to_log(net_t *Ndata, char* path, int size);

/******************************************************************
 *void display_help(): Displays the help screen.
 *
 *Recieves:            NA
 *
 *Returns:             NA
 */
void display_help();

#endif
#include <time.h>
