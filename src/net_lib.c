/***********************************************************************
 *net_lib.c : Implements the function definitions of net_lib.h, which
 *            are for use with netmon.c
 *
 *Author:    Dwayne Hoy / DoogeJ
 *
 *Date:      September 09 2005
 *
 *Version:   0.5.2
 *
 *Modified:  December 30 2015
 **********************************************************************/

#include "net_lib.h"


void display(net_t *Ndata, int size ,double time){
  int i;
  long double tx;          //Stores the tx total
  long double rx;          //Stores the rx total
  long double rx_trans;    //Stores the rx rate
  long double tx_trans;    //Stores the tx rate
  char tx_unit[4];         //Unit to display tx in
  char rx_unit[4];         //Unit to display rx in
  char rx_trans_unit[4];   //Unit to display rx rate in
  char tx_trans_unit[4];   //Unit to display tx rate in
  if(!DEBUG)
    clearscreen();
  /***********Prints title header*********/
  fprintf(stdout, "%s %16s %14s %11s %14s\n", "Device", "Downloaded",
	  "Rate" ,"Uploaded", "Rate");

  for(i = 0; i < size; i++){
    tx = to_unit(tx_unit, get_txbtotal(&Ndata[i]),
		 get_txgtotal(&Ndata[i]), 1);
    rx = to_unit(rx_unit, get_rxbtotal(&Ndata[i]),
		 get_rxgtotal(&Ndata[i]), 1);

    tx_trans = get_txRate(&Ndata[i], time);
    tx_trans = to_unit(tx_trans_unit, tx_trans, 0, 0);
    rx_trans = get_rxRate(&Ndata[i], time);
    rx_trans = to_unit(rx_trans_unit, rx_trans, 0, 0);

    fprintf(stdout, "%4s %15.2Lf %2s %9.2Lf %2s/s %8.2Lf %2s %9.2Lf %2s/s\n",
	    Ndata[i].device , rx, rx_unit, rx_trans, rx_trans_unit, tx,
	    tx_unit, tx_trans, tx_trans_unit);

  }
}

void clearscreen(){
  char clear[3];
  clear[0] = 033; clear[1] = 'c'; clear[2]='\0';
  fprintf(stdout, "%s",clear);
}



long double to_unit(char *unit, long double bval, long double gval,
		    int mode){
  long double tval;
  if(bval < 1024 && gval == 0){
    strcpy(unit, "B");
    return bval;
  }
  else if(bval < 1048576 && gval == 0){
    strcpy(unit, "KB");
    tval = (bval / 1024);
    return tval;
  }
  else if(bval < 1073741824 && gval == 0){
    strcpy(unit, "MB");
    tval = (bval / 1048576);
    return tval;
  }
  else{
    strcpy(unit, "GB");
    tval = gval;
    tval += (bval / (long double)1073741824);
    return (tval);
  }
}



int checkfile(char *path){
  struct stat attrib;
  if((lstat(path, &attrib)) != 0){
    if(DEBUG)
      fprintf(stderr, "checkfile: Error: Unable to get file statistics\n");
    return -1;
  }
  if(S_ISLNK(attrib.st_mode)){
    if(DEBUG)
      fprintf(stderr, "checkfile: Error: File is a symbolic link\n");
    return -2;
  }
  if(DEBUG)
    fprintf(stderr, "checkfile: file security check has passed\n");
  return 0;
}

int ver_check(FILE *dev_file){
  char buff[80];
  int i = 0;

  for(i = 0; i < 2; i++){
    if(!fgets(buff, 80, dev_file)){
      if(DEBUG)
        fprintf(stderr, "ver_check: Error: Unable to read dev file\n");
      return -1;
    }
  }

  if((strstr(buff ,"compressed")) == NULL){
    if(DEBUG)
      fprintf(stderr, "ver_check: Error: Incorrect dev version\n");
    return -2;
  }
  if(DEBUG)
    fprintf(stderr, "ver_check: dev version is correct\n");
  return 0;
}

void remove_newline(char* string){
  int i = 0;
  for(i = 0; i < strlen(string); i++){
    if(string[i] == '\n'){
      string[i] = '\0';
      if(DEBUG)
	fprintf(stderr, "remove_newline: Successfully removed a new line\n");
      return;
    }
  }
}

int write_to_log(net_t *Ndata, char* path, int size){
  FILE *log;
  char temp_string[300];
  int i;
  if(((log = fopen(path, "w")) == NULL))
    return -1;
  if(DEBUG)
    fprintf(stderr, "write_to_log: Successfully opened file for writting\n");
  if(!fputs("<0.5>\n", log)){
    fprintf(stderr, "write_to_log: Error writting to log\n");
    return -1;
  }

  for(i = 0; i < size; i++){
    if((snprintf(temp_string, 300, "[%s]\n%Lu\n%Lu\n%Lu\n%Lu\n",
		 Ndata[i].device,
		 get_rxbtotal(&Ndata[i]),
		 get_rxgtotal(&Ndata[i]),
		 get_txbtotal(&Ndata[i]),
		 get_txgtotal(&Ndata[i]))) < 0 ){
      fprintf(stderr, "write_to_log: Error creating string");
      return -1;
    }

    if(DEBUG)
      fprintf(stderr, "write_to_log: Successfully created string\n");

    if(!fputs(temp_string, log)){
      fprintf(stderr, "write_to_log: Error writting to log\n");
      return -1;
    }

    if(DEBUG)
      fprintf(stderr, "write_to_log: Successfully wrote to log\n");
  }
  fclose(log);
  return 0;
}


int parse_args(int argc, char *argv[]){
  int i;
  if(argc == 1)
    return 0;
  for(i = 1; i < argc; i++){
    if(strcmp(argv[i], "--config") == 0)
      return 1;
    else if(strcmp(argv[i], "--help") == 0){
      display_help();
      return -1;
    }
    else{
      fprintf(stderr, "Invalid argument please use --help flag for more details\n");
      return -1;
    }

  }
  return 0;
}

void display_help(){
  fprintf(stdout, "Usage netmonitor [OPTION]\n");
  fprintf(stdout, "A network monitoring console application.\n\n");
  fprintf(stdout, "  --help         Displays this text\n");
  fprintf(stdout, "  --config       Runs the configuration wizard\n\n");
  fprintf(stdout, "Project page: https://github.com/DoogeJ/netmonitor-ng\n\n");
}

