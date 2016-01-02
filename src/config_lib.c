/******************************************************************
 *config_lib.c : Provides function definitions for the header file
 *               config_lib.c. It is the functions for the
 *               configuration subprogram for netmonitor.
 *
 *Author:    Dwayne Hoy / DoogeJ
 *
 *Date:      September 20 2005
 *
 *Version:   0.5.2
 *
 *Modified:  December 30 2015
 ******************************************************************/
#include "config_lib.h"

static int is_dev = 0;

void config(char* dev_path, char* home_path){
  FILE *dev;
  char t_buff[150];
  conf_t devs[7];
  int num_dev;        /*Total number of devices*/
  char c_buff;
  int i, j, iput, is_dev = 0;

  if((dev = fopen(dev_path, "r")) == NULL){
    fprintf(stderr, "Error: Unable to open device file for reading\n");
    exit(EXIT_FAILURE);
  }
  if(vercheck(dev) != 0){
    fprintf(stderr, "Error: Device file is wrong version\n");
    exit(EXIT_FAILURE);
  }
  /*Remove header*/
  for(i = 0; i < 1; i++){
    if( !fgets(t_buff, sizeof(t_buff), dev)){
      fprintf(stderr, "Error: Unable to read device file\n");
      exit(EXIT_FAILURE);
    }
  }
  /*Read in values*/
  i = j = num_dev = 0;
  while(fgets(t_buff, sizeof(t_buff), dev)){
    j = i = 0;
    while(t_buff[i] != ':'){
      if(t_buff[i] != ' '){
	devs[num_dev].dev[j] = t_buff[i];
	j++;
      }
      i++;
    }
    devs[num_dev].added = 0;
    devs[num_dev].dev[j] = '\0';
    num_dev ++;
  }

  /*Main interface loop*/
  while(1){
    clrscrn();
    display_main_config();
    c_buff = getc(stdin);

    while(getc(stdin) != '\n');
    if(isdigit(c_buff)){
      iput = atoi(&c_buff);

      switch(iput){
      case 1:
	conf_add(devs, num_dev);
	break;
      case 2:
	conf_remove(devs, num_dev);
	break;
      case 3:
	i = conf_save(devs, num_dev, home_path);
	if(i == 1){
	  exit(EXIT_SUCCESS);
	}
	else if(i == -1){
	  exit(EXIT_FAILURE);
	}
	break;
      case 4:
	fprintf(stdout, "Are you sure you want to exit without saving [y|N] : ");

	c_buff = getc(stdin);
	while(getc(stdin) != '\n');

	if(c_buff == 'y')
	  exit(EXIT_SUCCESS);
	else
	  break;
      }

    }
  }
}

void display_main_config(){
  fprintf(stdout, "Netmonitor main configuration menu\n\n");
  fprintf(stdout, "1 - Add device\n");
  fprintf(stdout, "2 - Remove device\n");
  fprintf(stdout, "3 - Save configuration file and exit\n");
  fprintf(stdout, "4 - Exit without saving\n");
  fprintf(stdout, "Enter choice: ");

}

void conf_add(conf_t *dev, int size){
  char c_buff[3];
  int i, val;
  while(c_buff[0] != 'e'){
    clrscrn();
    fprintf(stdout,"Add device\n");
    fprintf(stdout, "\nAvailable devices\n");
    for(i = 0; i < size; i++){
      if(dev[i].added == 0)
	fprintf(stdout, "%d - %s\n", i , dev[i].dev);
    }
    fprintf(stdout, "\nSelected devices\n");
    for(i = 0; i < size; i++){
      if(dev[i].added == 1)
	fprintf(stdout, "%s\n", dev[i].dev);
    }
    fprintf(stderr, "\nPlease select a device to add (type 'e' when finished): ");
    fprintf(stdout, " ");
    read(STDIN_FILENO, c_buff, sizeof(c_buff));

    if(isdigit(c_buff[0])){
      val = atoi(c_buff);
      if(val < size){
	if(dev[val].added == 0){
	  dev[val].added = 1;
	  is_dev ++;
	}
      }
    }
  }
}

void conf_remove(conf_t *dev, int size){
  char c_buff[3];
  int i, val;
  while(c_buff[0] != 'e'){
    clrscrn();
    fprintf(stdout,"Remove device\n");
    fprintf(stdout, "\nAvailable devices\n");
    for(i = 0; i < size; i++){
      if(dev[i].added == 0)
	fprintf(stdout, "%s\n", dev[i].dev);
    }
    fprintf(stdout, "\nSelected devices\n");
    for(i = 0; i < size; i++){
      if(dev[i].added == 1)
	fprintf(stdout, "%d - %s\n", i, dev[i].dev);
    }
    fprintf(stderr, "\nPlease select a device to remove (type 'e' when finished): ");
    fprintf(stdout, " ");
    read(STDIN_FILENO, c_buff, sizeof(c_buff));

    if(isdigit(c_buff[0])){
      val = atoi(c_buff);
      if(val < size){
	if(dev[val].added != 0){
	  dev[val].added = 0;
	  is_dev --;
	}
      }
    }
  }
}


int conf_save(conf_t *dev, int size, char* homepath){
  FILE *dev_conf;
  FILE *data_dat;
  int i, dev_count = 0;
  char c_buff;
  char device_path[80], data_path[80], netmon_path[80];
  char version[] = "<0.5>";
  if(is_dev == 0){
    fprintf(stderr, "Error: Nothing to save\n");
    return 1;
  }
  fprintf(stdout, "This will erase all previous data. Are you sure ? [y|N]");
  c_buff = getc(stdin);
  while(getc(stdin) != '\n');

  if(c_buff == 'y'){
    /*Creates Paths*/
    strcpy(device_path, homepath);
    strcpy(data_path, homepath);
    strcpy(netmon_path, homepath);
    strcat(device_path, "/.netmonitor/device.conf");
    strcat(data_path, "/.netmonitor/data.dat");
    strcat(netmon_path, "/.netmonitor");
    /*checks and creates directory*/
    if(access(netmon_path, F_OK) == -1)
      mkdir(netmon_path, S_IRWXU);

    if((dev_conf = fopen(device_path, "w+")) == NULL){
      fprintf(stderr, "Error: Could not create device file\n");
      return -1;
    }

    if((data_dat = fopen(data_path, "w+")) == NULL){
      fprintf(stderr, "Error: Cound not create data file\n") ;
      return -1;
    }
    /*Prints version number*/
    fprintf(data_dat, "%s\n", version);
    /*Print to data file*/
    for(i = 0; i < size; i++){
      if(dev[i].added == 1){
	dev_count++;
	fprintf(data_dat,"[%s]\n0\n0\n0\n0\n", dev[i].dev);
      }
    }
    /*Print to device.conf*/
    fprintf(dev_conf, "%d\n", dev_count);
    for(i = 0; i < size; i++){
      if(dev[i].added == 1){
	fprintf(dev_conf,"%s\n", dev[i].dev);
      }
    }
  }
  else
    return 2;

  return 1;
}

int vercheck(FILE *dev_file){
  char buff[80];
  int i = 0;
  for(i = 0; i < 2; i++){
    if(!fgets(buff, 80, dev_file)){
      return -1;
    }
  }
  if((strstr(buff ,"compressed")) == NULL){
    return -2;
  }
  return 0;
}

void clrscrn(){
  char clear[3];
  clear[0] = 033; clear[1] = 'c'; clear[2]='\0';
  fprintf(stdout, "%s", clear);
}
