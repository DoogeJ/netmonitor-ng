/***********************************************************
 *net_data.h: Contains the net_t struct and it's functions
 *            for use with netmon.c
 *
 *Author:    Dwayne Hoy / DoogeJ
 *
 *Date:      September 09 2005
 *
 *Version:   0.5.2
 *
 *Modified:  December 30 2015
 ***********************************************************/
#ifndef NET_DATA
#define NET_DATA
#define ND_ONEGIG 1073741824

#include <sys/types.h>
#include <string.h>
typedef struct net_data{
  char device[6];
  long long byte_rx; //Holds bytes rx value
  long long gig_rx;  //Holds the gigabyte rx value
  long long pre_rx;  //Holds the last rx bytes count
  long long lval_rx; //Holds the last dev rx value
  long long byte_tx; //Holds bytes tx value
  long long gig_tx;  //Holds the gigabyte tx count
  long long pre_tx;  //Holds the last tx bytes count
  long long lval_tx; //Last dev tx value
} net_t;

/************************************************************
 *void init_net_t(): Initiates all values of a net_t
 *                   structure
 *
 *Recieves:          net_t* to the structure to be set
 *
 *Returns:           NA
 */
void init_net_t(net_t *Ndata);

/************************************************************
 *void set_tx:  Sets the rx value of the net_t structure
 *
 *Recieves:     *Ndata structure
 *              *devval: initial value from the dev file
 *              *bval: byte value from log file
 *              *gval: gig value from log file
 *
 *Returns:      NA
 */
void set_rx(net_t *Ndata, long long devval,  long long bval,
	    long long gval);

/************************************************************
 *void set_tx:  Sets the tx value of the net_t structure
 *
 *Recieves:     *Ndata structure
 *              *devval: initial value from the dev file
 *              *bval: byte value from log file
 *              *gval: gig value from log file
 *
 *Returns:      NA
 */
void set_tx(net_t *Ndata, long long devval, long long bval,
	    long long gval);
/***********************************************************
 *long long get_rxbtotal(): Returns the total rx bytes
 *
 *Recieves:   Ndata structure
 *
 *Returns:   long long value of rx bytes
 */
long long get_rxbtotal(net_t *Ndata);

/***********************************************************
 *long long get_txbtotal(): Returns the total tx bytes
 *
 *Recieves:   Ndata structure
 *
 *Returns:    long long value of tx bytes
 */
long long get_txbtotal(net_t *Ndata);

/***********************************************************
 *long long get_rxgtotal(): Returns the total rx gigs
 *
 *Recieves:   Ndata structure
 *
 *Returns:    long long value of rx gigs
 */
long long get_rxgtotal(net_t *Ndata);

/***********************************************************
 *long long get_txgtotal(): Returns the total tx gigs
 *
 *Recieves:   Ndata structure
 *
 *Returns:    long long value of tx gigs
 */
long long get_txgtotal(net_t *Ndata);

/***********************************************************
 *long long get_rxRate(): Gets the rate of change in two
 *                        updated set_rx() calls per second
 *
 *Recieves:               net_t structure to get value from.
 *                        double elapsed time between the
 *                        two set_rx() calls
 *
 *Returns:                The rate of change per second
 */
long long get_rxRate(net_t *Ndata, double elapse);

/***********************************************************
 *long long get_txRate(): Gets the rate of change in two
 *                        updated set_tx() calls per second
 *
 *Recieves:               net_t structure to get value from.
 *                        double elapsed time between the
 *                        two set_tx() calls
 *
 *Returns:                The rate of change per second.
 */
long long get_txRate(net_t *Ndata, double elapse);

/**********************************************************
 *void update_rx(): Updates the total data counters for rx,
 *                  contains logic for detecting dev file
 *                  overflow
 *
 *Recieves:         * Ndata structure
 *                  * val: The data value from dev file
 *
 *Returns:          NA
 */
void update_rx(net_t *Ndata, long long val);

/**********************************************************
 *void update_rx(): Updates the total data counters for rx,
 *                  contains logic for detecting dev file
 *                  overflow
 *
 *Recieves:         * Ndata structure
 *                  * val: The data value from dev file
 *
 *Returns:          NA
 */
void update_tx(net_t *Ndata, long long val);
#endif
