/* This is simlib.h. */

/* Include files. */

#include "simlib.h"             /* Required for use of simlib.c. */
#include <math.h>
#include <time.h>
#include <stdlib.h>

#define EVENT_RECEIVE_NEW				3
#define EVENT_RECEIVE   			    2  /* Event type for new call arrival. */
#define EVENT_RECEIVE_REQUEST			4
#define EVENT_SEND   					1  /* Event type for handoff call arrival. */
#define EVENT_END						5  /* Event type for departure. */
#define STREAM_NEW_INTERARRIVAL         1  /* Random-number stream for interarrivals. */
#define STREAM_REQUEST_INTERARRIVAL     2  /* Random-number stream for interarrivals. */
#define STREAM_NODE_SELECT     			3  /* Random-number stream for selecting node. */
#define STREAM_DATA_SELECT     			4  /* Random-number stream for selecting data. */
#define STREAM_TX_DELAY     			5  /* Random-number stream for transmission delay. */
#define STREAM_SELECT_CACHE     		6  /* Random-number stream for transmission delay. */

/* Declare data structure. */
struct Location
{
	float x;
	float y;
};

struct Data
{
	int id;
	struct Location location;
	float time;
};

struct Item
{
	float time;
	struct Data data;
	struct Item *next;
};

struct Node
{
	int id;
	struct Location location;
	struct Item *cache;
	struct Item *wait;
};

struct Event
{
	float time;
	int nodeId;
	int dataId;
	struct Event *next;
};

struct Neighbor
{
	struct Node node;
	struct Neighbor * next;
};

/* Declare non-simlib global variables. */
FILE *infile, *outfile;

struct Data *list_Data;
struct Node *list_Node;
struct Event *list_Receive;
struct Event *list_Send;

int n_Request_NodeId = -1, n_Request_DataId = -1;

// variables read from file
int n_Total_SimTime = -1, n_NodeNo = -1, n_DataNo = -1;
int n_Max_Cache_Size = -1;
float mean_data_interarrival, mean_request_interarrival;
float lambda;
int n_Tx_Range = -1;

/* Declare statistic variables */
int n_Total_Request = 0, n_Producer_Hit = 0, n_Cache_Hit = 0, n_Miss = 0, n_Insert_Cache = 0, n_Replacement = 0;
int n_Total_Overhead = 0;

/* Declare non-simlib functions. */
void init_model(void);
void report(void);

void receive_request(void);
void receive_new(void);
void receive(int);
void send(void);

void schedule_send(void);

double cal_distance(struct Location, struct Location);
struct Neighbor * get_neighbors(int);

int if_in_cache(int, int);
int if_in_wait(int, int);

void print_node_cache(int);
void print_node_wait(int);
void print_send();
void print_receive();
void print_neighbor_list(int);

int add_to_cache(int, int);
int add_to_wait(float, int, int);
int add_to_send(float, int, int);
int add_to_receive(float, int, int);

int remove_from_cache(int, int);
int remove_from_wait(int, int);

int get_random_int();
