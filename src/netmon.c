/*****************************************************************************
 *netmon.c:  Main program for netmonitor, netmonitor is a network monitoring  
 *           and logging program for linux. The aim is to provide a bandwidth
 *           meter type program that is small and simple
 *
 *Author:    Dwayne Hoy / DoogeJ
 *
 *Date:      Monday, September 05 2005
 *
 *Version:   0.5.1
 *
 *Mod date:  Tuesday, December 29 2015
 ****************************************************************************/


#include "net_lib.h"


int main(int argc, char *argv[]){

  /*File Paths*/
  char filepath[] = "/proc/net/dev";

  char *homepath = getenv("HOME");

  char logpath[80];
  strcpy(logpath, homepath);
  strcat(logpath, "/.netmon/data.dat");
 
  char confpath[80];
  strcpy(confpath, homepath);
  strcat(confpath, "/.netmon/device.conf");
  
  
  /*File pointers*/
  FILE *dev_file = NULL;              
  FILE *log_file = NULL;               
  FILE *conf_file = NULL;              
  
  net_t *data;
  int args;                       /*Arg return value*/
  char t_buff[160];               /*Temporary buffer*/
  char mod_dev[160];              /*Buffer for dev modifications*/
  int size = 0;                   /*Size of the net_t array*/
  int index = 0;                  /*Index to device array entry*/
  char* delim;                    /*Pointer to end of device*/
  long long temprx, temptx;       /*Stores dev values before update*/
  time_t start, finish;           /*Used for timing data*/
  int first_run = 1;              /*Check for program start*/

  time(&start);
  if((args = parse_args(argc, argv)) == 1){
    config(filepath, homepath);
    exit(EXIT_SUCCESS);
  }
  if(args == -1)
    exit(EXIT_SUCCESS);
  
  /***************************CONFIG FILE ACCESS***************************/
  
  if((conf_file = fopen(confpath, "r")) == NULL){
    fprintf(stderr, "Error: Unable to open config file. ");
    fprintf(stderr, "Please run with --config flag\n");
    exit(EXIT_FAILURE);
  }

 if(! fgets(t_buff, sizeof(t_buff), conf_file)){
    fprintf(stderr, "Error: Unable to read config file\n");
    exit(EXIT_FAILURE);
  }
int j, i = atoi(t_buff);
  if(DEBUG){
    fprintf(stderr, "main:CONFIG FILE ACCESS: number of devices in ");
    fprintf(stderr, "config file%d\n", i);
  }
  data = calloc(i, sizeof(net_t));
  for(j = 0; j < i; j++){
    if (!fgets(t_buff, sizeof(t_buff), conf_file)){
      fprintf(stderr, "Error: Recieved end of file before expected\n");
      exit(EXIT_FAILURE);
    }
    remove_newline(t_buff);
    strcpy(data[j].device, t_buff);
    if(DEBUG){
      fprintf(stderr, "main:CONFIG FILE ACCESS: device entered in");
      fprintf(stderr, " data[%i] is %s\n", j ,data[j].device);
    }
  }
  size = i;
  if(DEBUG)
    fprintf(stderr, "main:CONFIG FILE ACCESS: Size of array is %d\n", size);
  fclose(conf_file);

  /*****************************LOG FILE ACCESS V0.5***********************/
  long long rxbval[5];
  long long txbval[5];
  long long rxgval[5];
  long long txgval[5];
  long long clearrxval[5];
  long long cleartxval[5];
  
  /*Open log file for reading*/
  if((log_file = fopen(logpath, "r")) == NULL){
    fprintf(stderr, "Error: Unable to open log file. ");
    fprintf(stderr, "Please run with --config flag\n");
    exit(EXIT_FAILURE);
  }
  
  /*Checks log file version*/
  if(!fgets(t_buff, sizeof(t_buff), log_file)){
    fprintf(stderr, "Error: Unable to read log file\n");
  }
  if(strstr(t_buff, "<0.5>") == NULL){
    fprintf(stderr, "Error: Wrong log file version. Please update\n");
    exit(EXIT_FAILURE);
  }
  /*******LOG file loop*********/
  for(i = 0; i < size; i++){
    if(DEBUG)
      fprintf(stderr, "main:LOG FILE ACCESS: Attempting to read log file\n");
    
    if(!fgets(t_buff, sizeof(t_buff), log_file)){
      fprintf(stderr, "Error: Unable to read log file\n");
    }
    
    /*Find correct device*/
    for(index = 0; index < size; index ++){
      if(strstr(t_buff, data[index].device) != NULL){
	break;
      }
    }    
    if(DEBUG)
      fprintf(stderr, "main:LOG FILE ACCESS: Found device at position %d\n", 
	      index);
    /*Reads in rx byte value from log*/
    if(!fgets(t_buff, sizeof(t_buff), log_file)){
      fprintf(stderr, "Error: Unable to retrieve data values\n");
      exit(EXIT_FAILURE);
    }
    sscanf(t_buff, "%lld", &rxbval[index]);
    
    /*Reads in rx gig value from log*/
    if(!fgets(t_buff, sizeof(t_buff), log_file)){
      fprintf(stderr, "Error: Unable to retrieve data values\n");
      exit(EXIT_FAILURE);
    }
 
    sscanf(t_buff, "%lld", &rxgval[index]);
    
 
    
    /*Reads in tx byte value from log*/
    if(!fgets(t_buff, sizeof(t_buff), log_file)){
      fprintf(stderr, "Error: Unable to retrieve data values\n");
      exit(EXIT_FAILURE);
    }
    sscanf(t_buff, "%lld", &txbval[index]);

    /*Reads in tx gig value from log*/
    if(!fgets(t_buff, sizeof(t_buff), log_file)){
      fprintf(stderr, "Error: Unable to retrieve data values\n");
      exit(EXIT_FAILURE);
    }
    sscanf(t_buff, "%lld", &txgval[index]);
  }

  fclose(log_file);
    
    /***************************DEV FILE ACCESS****************************/
  /*Check dev file*/
  if((checkfile(filepath)) != 0){
    fprintf(stderr, "File security failed\n");
    exit(EXIT_FAILURE);
  }
  if(DEBUG)
    fprintf(stderr, "main:DEV FILE ACCESS:dev file passed security check\n");

  /*Open dev file for reading*/
  if((dev_file = fopen(filepath, "r")) == NULL){
    fprintf(stderr, "Error: Unable to open dev file\n");
    exit(EXIT_FAILURE);
  }
  
  /*Check dev file version*/
  if((ver_check(dev_file)) < 0){
    fprintf(stderr, "Error: Incorrect dev version\n");
    exit(EXIT_FAILURE);
  }
  if(DEBUG){
    fprintf(stderr, "main:DEV FILE ACCESS: Dev file is the correct ");
    fprintf(stderr, "version\n");
  }
  rewind(dev_file);
  if(DEBUG)
    fprintf(stderr, "main:DEV FILE ACCESS: Dev file is at the beginning\n");

  /*******************************UPDATE LOOP******************************/
  while(1){
    if(DEBUG)
      fprintf(stderr, "main:UPDATE LOOP: Entering update loop now\n");
    
    /*removes headings*/
    for(i = 0; i < 1; i++){
      if( !fgets(t_buff, sizeof(t_buff), dev_file)){
	fprintf(stderr, "Error:UPDATE LOOP Unable to read dev file\n");
	exit(EXIT_FAILURE);
      }
    }
    int found = 0;
    /*Checks that device is in data*/
    while(fgets(t_buff, sizeof(t_buff), dev_file)){
      found = 0;
      for(j = 0; j < size; j++){
	if(strstr(t_buff, data[j].device) != NULL){
	  found = 1;
	  if(DEBUG){
	    fprintf(stderr, "main:UPDATE LOOP:Found matching dev file ");
	    fprintf(stderr,"device at position %d\n",j);
	  }
	  break;
	}
      }
      if(DEBUG){
	fprintf(stderr, "main:UPDATE LOOP: Exited out of update loops ");
	fprintf(stderr, "device check\n");
      }      
      if(found){
	if(DEBUG)
	  fprintf(stderr, "main:UPDATE LOOP: Entered data update section\n");
        /*Retrieves values*/
	delim = strchr(t_buff, ':');

	if( *( delim + 1 ) == ' ' ){
	  sscanf(t_buff,"%*s %Lu %*d %*d %*d %*d %*d %*d %*d %Lu ", 
		 &temprx, &temptx);
	  if(first_run){
	      clearrxval[j] = temprx;
	      cleartxval[j] = temptx;
	    }
	}
	else{
	  if(DEBUG){
	    fprintf(stderr, "main:UPDATE LOOP: Found no space between ");
	    fprintf(stderr,"device and value\n");
	  }
	  strcpy(mod_dev ,delim + 1);
	  for(i = 0; i < sizeof(mod_dev); i++){
	    if(mod_dev[i] == ' '){
	      mod_dev[i] = '\0';
	      break;
	    } 
	  }
	  temprx = atoll(mod_dev);        
	  sscanf(t_buff,"%*s %*d %*d %*d %*d %*d %*d %*d %Lu ", &temptx);
	  if(first_run)
	    {
	      clearrxval[j] = temprx;
	      cleartxval[j] = temptx;
	    }
	}
	if(first_run){
	  set_tx(&data[j], cleartxval[j], txbval[j], txgval[j]);
	  set_rx(&data[j], clearrxval[j], rxbval[j], rxgval[j]);
	}
	else{
	  update_rx(&data[j], temprx);
	  update_tx(&data[j], temptx);
	}
      }
    }
    first_run = 0;
    rewind(dev_file);
    if(DEBUG)
      fprintf(stderr, "main:UPDATE LOOP: dev file rewinded\n");    
    time(&finish);    
    if(DEBUG)
      fprintf(stderr, "main:UPDATE LOOP: start time is %d seconds\n", (int)start);   
    if(DEBUG)
      fprintf(stderr, "main:UPDATE LOOP: finish time is %d seconds\n", (int)finish);
    time(&finish);
    display(data, size,(double)(finish - start));
    time(&start);
    if((write_to_log(data, logpath, size)) < 0){
      fprintf(stderr, "main:UPDATE LOOP: Error writing to log. Exiting!");
      exit(EXIT_FAILURE);
    }
    sleep(2);
  }
  exit(EXIT_SUCCESS);
}

