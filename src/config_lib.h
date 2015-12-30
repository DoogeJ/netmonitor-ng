/*****************************************************************
 *config_lib.h: Provides funtion definitions for the configuration
 *              subprogram for netmonitor
 *
 *Author:    Dwayne Hoy / DoogeJ
 *
 *Date:      September 20 2005
 *
 *Version:   0.5.2
 *
 *Modified:  December 30 2015
 *****************************************************************/
#ifndef _CONFIG_LIB_H
#define _CONFIG_LIB_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ctype.h>
#include <fcntl.h>

typedef struct config_dev{
  char dev[7];
  int added;
}conf_t;

/******************************************************************
 *void config(): Runs the config subprogram to configure users
 *               data files.
 *
 *Recieves:      char* to the dev file path
 *               char* to the users home path
 *
 *Returns:       NA
 */
void config(char* dev_path, char* home_path);

/*****************************************************************
 *void display_main_config(): Displays the main menu to the screen
 *
 *Recieves: NA
 *
 *Returns: NA
 */
void display_main_config();

/****************************************************************
 *int vercheck(): Checks the version of the /proc/net/dev file
 *
 *Recieves: File pointer to the /proc/net/dev file
 *
 *Returns: An int value 1 for correct version -1 for incorrect
 */
int vercheck(FILE *dev_file);

/****************************************************************
 *void conf_add(): Adds devices to the configuration buffer for
 *                 saving.
 *
 *Recieves: A conf_t pointer holding the buffer, and an int
 *          value holding the size of the buffer;
 *
 *Returns: NA
 */
void conf_add(conf_t *dev, int size);

/***************************************************************
 *void conf_remove: Removes devices from the configuration
 *                  buffer;
 *
 *Recieves: A conf_t pointer holding the buffer, and an int
 *          value holding the size of the buffer;
 *
 *Returns: NA
 */
void conf_remove(conf_t *dev, int size);

/**************************************************************
 *int conf_save(): Saves configeration buffer to the
 *                 appropriate files.
 *
 *Recieves: A conf_t pointer holding the buffer, an int value
 *          holding the value of the buffer, and the path
 *          of the directory to save the files in.
 *
 *Returns: A int value 1 if save succeeded -1 if it fails.
 */
int conf_save(conf_t *dev, int size, char* homepath);

/************************************************************
 *void clrscrn(): Completely clears all text from a console.
 *
 *Recieves: NA
 *
 *Returns: NA
 */
void clrscrn();
#endif
