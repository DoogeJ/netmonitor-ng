/************************************************************
 *net_data.c: Functions for the functions and struct in 
 *            net_data.h, to be used with netmon.c
 *
 *Author: Dwayne Hoy
 *
 *Date: Friday, September 09 2005
 *
 *Version: 0.5
 *
 *Modified: Sunday, October 09 2005
 ************************************************************/
#include "net_data.h"

void init_net_t(net_t *Ndata){
  Ndata->gig_rx = 0;
  Ndata->pre_rx = 0;
  Ndata->byte_rx = 0;
  Ndata->lval_rx = 0;
  Ndata->gig_tx = 0;
  Ndata->pre_tx = 0;
  Ndata->byte_tx = 0;
  Ndata->lval_tx = 0;
}
/**********************SET FUNCTIONS************************/
void set_rx(net_t *Ndata, long long devval ,long long bval, 
	    long long gval){
  Ndata->pre_rx = bval;
  Ndata->byte_rx = bval;
  Ndata->gig_rx = gval;
  Ndata->lval_rx = devval;
}

void set_tx(net_t *Ndata, long long devval ,long long bval, 
	    long long gval){
  Ndata->pre_tx = bval;
  Ndata->byte_tx = bval;
  Ndata->gig_tx = gval;
  Ndata->lval_tx = devval;
}

/**********************GET FUNCTIONS**************************/
long long get_rxRate(net_t *Ndata, double elapse){
  if(elapse == 0){
    elapse = 1;
  }
  return (Ndata->byte_rx - Ndata->pre_rx)/elapse;
}

long long get_txRate(net_t *Ndata, double elapse){
  if(elapse == 0){
    elapse = 1;
  }
  return (Ndata->byte_tx - Ndata->pre_tx)/elapse;
}

long long get_rxbtotal(net_t *Ndata){
  return Ndata->byte_rx;
}

long long get_txbtotal(net_t *Ndata){
  return Ndata->byte_tx;
}

long long get_rxgtotal(net_t *Ndata){
  return Ndata->gig_rx;
}

long long get_txgtotal(net_t *Ndata){
  return Ndata->gig_tx;
}
/**********************UPDATE FUNCTIONS***********************/
void update_tx(net_t *Ndata, long long val){
  // Check if new value is less than old value
  long long tval;
  if(val < Ndata->lval_tx){
    tval = val + (long long)858993459 * (long long)5;
  }
  else{
    tval = val;
  }
  //Set tval
  tval = tval - Ndata->lval_tx;
  //reset lval
  Ndata->lval_tx = val;
  //Update values
  Ndata->pre_tx = Ndata->byte_tx;
  Ndata->byte_tx = Ndata->byte_tx + tval;
  //Check if pre is 1 gig yet
  //Normalizes 1 gig each cycle so no need to loop
  if(Ndata->pre_tx > ND_ONEGIG){
    Ndata->pre_tx = Ndata->pre_tx - ND_ONEGIG;
    Ndata->byte_tx = Ndata->byte_tx - ND_ONEGIG;
    Ndata->gig_tx++;
  }
}

void update_rx(net_t *Ndata, long long val){
  // Check if new value is less than old value
  long long tval;
  if(val < Ndata->lval_rx){
    tval = val + (long long)858993459 * (long long)5;
  }
  else{
    tval = val;
  }
  //Set tval
  tval = tval - Ndata->lval_rx;
  //reset lval
  Ndata->lval_rx = val;
  //Update values
  Ndata->pre_rx = Ndata->byte_rx;
  Ndata->byte_rx = Ndata->byte_rx + tval;
  //Check if pre is 1 gig yet
  //Normaizes 1 gig each cycle so no need to loop
  if(Ndata->pre_rx > ND_ONEGIG){
    Ndata->pre_rx = Ndata->pre_rx - ND_ONEGIG;
    Ndata->byte_rx = Ndata->byte_rx - ND_ONEGIG;
    Ndata->gig_rx++;
  }
}
