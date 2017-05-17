// Meng Kuai
// Term Project CS 580 Spring 2015
// 4/17/2015
// Instruction: cache all received data

#include "cache.h"

int main() /* Main function. */
{
	/* Open input and output files. */
	// check opening file is successful or not
	if ((infile = fopen("input.txt", "r")) == NULL)
	{
		printf("Cannot open input.txt!\n");
		return -1;
	}
	if ((outfile = fopen("output.txt", "w")) == NULL)
	{
		printf("Cannot open output.txt!\n");
		return -1;
	}

	/* Read input parameters. */
	// check return value is correct or not !!!
	if (fscanf(infile, "%d\n%d\n%d\n%d\n%f\n%f\n%f\n%d", &n_Total_SimTime, &n_NodeNo, &n_DataNo, &n_Max_Cache_Size,
			&mean_data_interarrival, &mean_request_interarrival, &lambda, &n_Tx_Range) == 0)
	{
		printf("Input file is not in correct format!\n");
		return -1;
	}

	/* Initialize simlib */
	init_simlib();

	/* Set maxatr = max(maximum number of attributes per record, 4) */
	maxatr = 4; /* NEVER SET maxatr TO BE SMALLER THAN 4. */

	/* Initialize the model. */
	init_model();

	/* Run the simulation while more delays are still needed. */
	while (next_event_type != EVENT_END)
	{

		/* Determine the next event. */
		timing();

		/* Invoke the appropriate event function. */
		switch (next_event_type)
		{
			case EVENT_RECEIVE_NEW:
				receive_new();
				break;
			case EVENT_RECEIVE:
				receive(EVENT_RECEIVE);
				break;
			case EVENT_SEND:
				send();
				break;
			case EVENT_RECEIVE_REQUEST:
				receive_request();
				break;
		}
	}

	/* Invoke the report generator and end the simulation. */
	report();

	fclose(infile);
	fclose(outfile);

	return 0;
}

void init_model(void) /* Initialization function. */
{
	// initialize variables

	// load data info
	list_Data = (struct Data *) calloc(n_DataNo, sizeof(struct Data));
	list_Data[0] = (struct Data ) { 1, (struct Location ) { 0, 0 }, 10 };
	list_Data[1] = (struct Data ) { 2, (struct Location ) { 100, 0 }, 20 };
	list_Data[2] = (struct Data ) { 3, (struct Location ) { 200, 0 }, 30 };
	list_Data[3] = (struct Data ) { 4, (struct Location ) { 0, 100 }, 40 };
	list_Data[4] = (struct Data ) { 5, (struct Location ) { 100, 100 }, 50 };
	list_Data[5] = (struct Data ) { 6, (struct Location ) { 200, 100 }, 50 };
	list_Data[6] = (struct Data ) { 7, (struct Location ) { 0, 200 }, 50 };
	list_Data[7] = (struct Data ) { 8, (struct Location ) { 100, 200 }, 50 };
	list_Data[8] = (struct Data ) { 9, (struct Location ) { 200, 200 }, 50 };
	list_Data[9] = (struct Data ) { 10, (struct Location ) { 300, 300 }, 50 };
	list_Data[10] = (struct Data ) { 11, (struct Location ) { 0, 0 }, 10 };
	list_Data[11] = (struct Data ) { 12, (struct Location ) { 100, 0 }, 20 };
	list_Data[12] = (struct Data ) { 13, (struct Location ) { 200, 0 }, 30 };
	list_Data[13] = (struct Data ) { 14, (struct Location ) { 0, 100 }, 40 };
	list_Data[14] = (struct Data ) { 15, (struct Location ) { 100, 100 }, 50 };
	list_Data[15] = (struct Data ) { 16, (struct Location ) { 200, 100 }, 50 };
	list_Data[16] = (struct Data ) { 17, (struct Location ) { 0, 200 }, 50 };
	list_Data[17] = (struct Data ) { 18, (struct Location ) { 100, 200 }, 50 };
	list_Data[18] = (struct Data ) { 19, (struct Location ) { 200, 200 }, 50 };
	list_Data[19] = (struct Data ) { 20, (struct Location ) { 300, 300 }, 50 };

	// load node info
	list_Node = (struct Node *) calloc(n_NodeNo, sizeof(struct Node));
	list_Node[0] = (struct Node ) { 1, (struct Location ) { 0, 0 } };
	list_Node[1] = (struct Node ) { 2, (struct Location ) { 100, 0 } };
	list_Node[2] = (struct Node ) { 3, (struct Location ) { 200, 0 } };
	list_Node[3] = (struct Node ) { 4, (struct Location ) { 0, 100 } };
	list_Node[4] = (struct Node ) { 5, (struct Location ) { 100, 100 } };
	list_Node[5] = (struct Node ) { 6, (struct Location ) { 200, 100 } };
	list_Node[6] = (struct Node ) { 7, (struct Location ) { 0, 200 } };
	list_Node[7] = (struct Node ) { 8, (struct Location ) { 100, 200 } };
	list_Node[8] = (struct Node ) { 9, (struct Location ) { 200, 200 } };
	list_Node[9] = (struct Node ) { 10, (struct Location ) { 300, 300 } };

	// schedule the end of the simulation
	event_schedule(n_Total_SimTime, EVENT_END);

	// schedule the first receive event
	float f_ArriveTime = sim_time + expon(mean_data_interarrival, STREAM_NEW_INTERARRIVAL);
	int n_ArriveNodeId = get_random_int(STREAM_NODE_SELECT) % n_NodeNo + 1;
	int n_ArriveDataId = get_random_int(STREAM_DATA_SELECT) % n_DataNo + 1;

	if (add_to_receive(f_ArriveTime, n_ArriveNodeId, n_ArriveDataId) == 0)
	{
		event_schedule(f_ArriveTime, EVENT_RECEIVE_NEW);
		printf("At %f, Schedule EVENT_RECEIVE_NEW:\ttime:%f\tnode id:%d\tdata id:%d\n", sim_time, f_ArriveTime,
				n_ArriveNodeId,
				n_ArriveDataId);
	}

	// schedule the first request event
	f_ArriveTime = sim_time + expon(mean_request_interarrival, STREAM_REQUEST_INTERARRIVAL);
	n_Request_NodeId = get_random_int(STREAM_NODE_SELECT) % n_NodeNo + 1;
	n_Request_DataId = get_random_int(STREAM_DATA_SELECT) % n_DataNo + 1;
	event_schedule(f_ArriveTime, EVENT_RECEIVE_REQUEST);
}

void report(void) /* Report generator function. */
{
	fprintf(outfile, "[Simulation Results]\n");
	fprintf(outfile, "Total simulation time=%d\n", sim_time);
	fprintf(outfile, "Satisfied Ratio = %f\n", (float) (n_Producer_Hit + n_Cache_Hit) / n_Total_Request);
	fprintf(outfile, "Cache Hit Ratio = %f\n", (float) (n_Cache_Hit) / (n_Producer_Hit + n_Cache_Hit));
	fprintf(outfile, "Cache Replacement Ratio = %f\n", (float) n_Replacement / n_Insert_Cache);
	fprintf(outfile, "Average Overhead = %f\n", (float) n_Total_Overhead / n_NodeNo);

}

// event handler for EVENT_RECEIVE_REQUEST
void receive_request(void)
{
	n_Total_Request++;

	// check the cache
	if (if_in_cache(n_Request_NodeId, n_Request_DataId) == 1)
	{
		if (cal_distance(list_Node[n_Request_NodeId - 1].location, list_Data[n_Request_DataId - 1].location)
				< n_Tx_Range)
		{
			n_Producer_Hit++;
		}
		else
		{
			n_Cache_Hit++;
		}
	}
	else
	{
		n_Miss++;
	}

	// schedule the next request event
	float f_ArriveTime = sim_time + expon(mean_request_interarrival, STREAM_REQUEST_INTERARRIVAL);
	n_Request_NodeId = get_random_int(STREAM_NODE_SELECT) % n_NodeNo + 1;
	n_Request_DataId = get_random_int(STREAM_DATA_SELECT) % n_DataNo + 1;
	event_schedule(f_ArriveTime, EVENT_RECEIVE_REQUEST);

}

// event handler for RECEIVE_NEW event: receive data from outside
void receive_new(void)
{
	float f_ArriveTime = sim_time + expon(mean_data_interarrival, STREAM_NEW_INTERARRIVAL);
	int n_ArriveNodeId = get_random_int(STREAM_NODE_SELECT) % n_NodeNo + 1;
	int n_ArriveDataId = get_random_int(STREAM_DATA_SELECT) % n_DataNo + 1;

	if (add_to_receive(f_ArriveTime, n_ArriveNodeId, n_ArriveDataId) == 0)
	{
		event_schedule(f_ArriveTime, EVENT_RECEIVE_NEW);
		printf("At %f, Schedule EVENT_RECEIVE_NEW:\ttime:%f\tnode id:%d\tdata id:%d\n", sim_time, f_ArriveTime,
				n_ArriveNodeId,
				n_ArriveDataId);
	}
	receive(EVENT_RECEIVE_NEW);
}

// event handler for RECEIVE event
void receive(int n_event_type)
{
	if (list_Receive != NULL)
	{
		// 1. get the receive node id, data id and time
		// get and remove receive event from receive list
		struct Event * old = list_Receive;
		float f_Time = old->time;
		int n_NodeId = old->nodeId, n_DataId = old->dataId;
		// if the event should not happen, schedule a new one (postpone)
		if (f_Time < sim_time)
			return;
		else if (f_Time > sim_time)
		{
			event_schedule(f_Time, n_event_type);
			return;
		}

		list_Receive = list_Receive->next; // remove the first item
		free(old);

		printf("At %f, Node %d receives Data %d\n", sim_time, n_NodeId, n_DataId);

		// 2. if it is in cache, drop it
		if (if_in_cache(n_NodeId, n_DataId) == 1)
		{
			printf("Node %d has Data %d in cache, drop the data!\n", n_NodeId, n_DataId);
			return;
		}
		else // if it is not in cache, cache it
		{
			if (add_to_cache(n_NodeId, n_DataId) == 0)
			{
				printf("At %f, Node %d caches Data %d\n", sim_time, n_NodeId, n_DataId);
			}
			else
			{
				printf("Error: add to cache!\n");
			}
			return;
		}

		// 3. if it is in waiting list, remove it from waiting list
		if (if_in_wait(n_NodeId, n_DataId) == 1)
		{
			if (remove_from_wait(n_NodeId, n_DataId) == 0)
			{
				printf("Node %d has Data %d in waiting list, remove the data from waiting list, drop the data!\n",
						n_NodeId, n_DataId);
			}
			else
			{
				printf("Error: remove from wait!\n");
			}
			return;
		}

		// 4. schedule SEND event based on waiting timer, put to waiting list
		float f_WaitingTimer;
		f_WaitingTimer = lambda / (cal_distance(list_Node[n_NodeId - 1].location, list_Data[n_DataId - 1].location));
		float f_SendTime = sim_time + f_WaitingTimer;
		if (add_to_wait(f_SendTime, n_NodeId, n_DataId) == 0)
		{

			printf("Add to wait:\ttime:%f\tnode id:%d\tdata id:%d\n", f_SendTime, n_NodeId, n_DataId);
			schedule_send();
		}
		else
		{
			printf("Error: add to wait!\n");
		}
	}
}

// get the earliest event from wait list of all nodes and schedule the next send
void schedule_send(void)
{
	float f_Time = INFINITY;
	int n_NodeId = -1, n_DataId = -1, i;

	// find the earliest sending in wait list
	for (i = 0; i < n_NodeNo; i++)
	{
		struct Item * current_item = list_Node[i].wait;
		if (current_item == NULL)
			continue;
		while (current_item != NULL)
		{
			if (current_item->time < f_Time)
			{
				f_Time = current_item->time;
				n_NodeId = i + 1;
				n_DataId = current_item->data.id;
			}
			current_item = current_item->next;
		}
	}

	if (n_NodeId != -1 && n_DataId != -1 && f_Time > sim_time)
	{
		if (remove_from_wait(n_NodeId, n_DataId) == 0)
		{
			printf("Remove from wait:\ttime:%f\tnode id:%d\tdata id:%d\n", f_Time, n_NodeId,
					n_DataId);
		}
		else
		{
			printf("Error: remove from wait!\n");
		}

		if (add_to_send(f_Time, n_NodeId, n_DataId) == 0)
		{
			event_schedule(f_Time, EVENT_SEND);
			printf("At %f, Schedule EVENT_SEND:\ttime:%f\tnode id:%d\tdata id:%d\n", sim_time, f_Time, n_NodeId,
					n_DataId);
		}
		else
		{
			printf("Error: add to send!\n");
		}
	}

}

// event handlder for SEND event
void send(void)
{
	// schedule the next send event
	schedule_send();

	// data send event from send list and remove it
	if (list_Send != NULL)
	{
		// 1. get the send node and data
		// get and remove send event from send list
		struct Event * old = list_Send;
		list_Send = list_Send->next;
		float f_Time = old->time;
		int n_NodeId = old->nodeId, n_DataId = old->dataId;
		free(old);
		printf("At %f, Node %d sends Data %d\n", f_Time, n_NodeId, n_DataId);

		// add to cache
		if (if_in_cache(n_NodeId, n_DataId) == 0) // not in cache
		{

			if (add_to_cache(n_NodeId, n_DataId) == 0)
			{
				printf("At %f, Node %d caches Data %d\n", sim_time, n_NodeId, n_DataId);
			}
			else
			{
				printf("Error: add to cache!\n");
			}
		}

		// 2. schedule RECEIVE event for neighbors and add to list_Receive
		struct Neighbor * neighbor = get_neighbors(n_NodeId);
		while (neighbor != NULL)
		{
			int n_NeighborId = neighbor->node.id;
			float offset = (float) (get_random_int(STREAM_TX_DELAY) % n_NeighborId) / 10;
			float f_ReceiveTime = sim_time + offset;

			if (add_to_receive(f_ReceiveTime, n_NeighborId, n_DataId) == 0)
			{
				event_schedule(f_ReceiveTime, EVENT_RECEIVE);
				printf("At %f, Schedule EVENT_RECEIVE:\ttime:%f\tnode id:%d\tdata id:%d\n", sim_time, f_ReceiveTime,
						n_NeighborId,
						n_DataId);
			}
			else
			{
				printf("Error: add to receive!\n");
			}
			neighbor = neighbor->next;
		}
		n_Total_Overhead++;
	}
	else
		printf("Sending list is empty!\n");
}

//
int add_to_send(float f_time, int n_NodeId, int n_DataId)
{
	// if list_Send is empty
	if (list_Send == NULL)
	{
		list_Send = malloc(sizeof(struct Event));
		list_Send->time = f_time;
		list_Send->nodeId = n_NodeId;
		list_Send->dataId = n_DataId;
		list_Send->next = NULL;
		return 0;
	}

	// if new one is smaller than first one
	if (f_time <= list_Send->time)
	{
		// rescore old one
		struct Event * old = malloc(sizeof(struct Event));
		old->time = list_Send->time;
		old->nodeId = list_Send->nodeId;
		old->dataId = list_Send->dataId;
		old->next = list_Send->next;

		// update the first one
		list_Send->time = f_time;
		list_Send->nodeId = n_NodeId;
		list_Send->dataId = n_DataId;
		list_Send->next = old;
		return 0;
	}

	// if there is only one
	if (list_Send->next == NULL)
	{
		struct Event * new = malloc(sizeof(struct Event));
		new->time = f_time;
		new->nodeId = n_NodeId;
		new->dataId = n_DataId;
		new->next = NULL;
		list_Send->next = new;
		return 0;
	}

	struct Event * prev = list_Send;
	struct Event * current = list_Send->next;
	while (current != NULL)
	{
		// find the first event that larger than new one
		if (prev->time <= f_time && current->time >= f_time)
		{

			struct Event * new = malloc(sizeof(struct Event));
			new->time = f_time;
			new->nodeId = n_NodeId;
			new->dataId = n_DataId;
			new->next = current;
			prev->next = new;
			return 0;
		}
		prev = current;
		current = current->next;
	}

	if (current == NULL && prev->time <= f_time)
	{
		struct Event * new = malloc(sizeof(struct Event));
		new->time = f_time;
		new->nodeId = n_NodeId;
		new->dataId = n_DataId;
		new->next = NULL;
		prev->next = new;
		return 0;
	}
	return -1;
}

void print_send()
{
	printf("Send List: \n");
	struct Event * event = list_Send;
	if (event == NULL)
		printf("empty\n");
	while (event != NULL)
	{
		printf("%f\t%d\t%d\n", event->time, event->nodeId, event->dataId);
		event = event->next;
	}
}

int add_to_receive(float f_time, int n_NodeId, int n_DataId)
{
	// if list_Receive is empty
	if (list_Receive == NULL)
	{
		list_Receive = malloc(sizeof(struct Event));
		list_Receive->time = f_time;
		list_Receive->nodeId = n_NodeId;
		list_Receive->dataId = n_DataId;
		list_Receive->next = NULL;
		return 0;
	}

	// if new one is smaller than first one
	if (f_time <= list_Receive->time)
	{
		// rescore old one
		struct Event * old = malloc(sizeof(struct Event));
		old->time = list_Receive->time;
		old->nodeId = list_Receive->nodeId;
		old->dataId = list_Receive->dataId;
		old->next = list_Receive->next;

		// update the first one
		list_Receive->time = f_time;
		list_Receive->nodeId = n_NodeId;
		list_Receive->dataId = n_DataId;
		list_Receive->next = old;
		return 0;
	}

	// if there is only one
	if (list_Receive->next == NULL)
	{
		struct Event * new = malloc(sizeof(struct Event));
		new->time = f_time;
		new->nodeId = n_NodeId;
		new->dataId = n_DataId;
		new->next = NULL;
		list_Receive->next = new;
		return 0;
	}

	struct Event * prev = list_Receive;
	struct Event * current = list_Receive->next;
	while (current != NULL)
	{
		// find the first event that larger than new one
		if (prev->time <= f_time && current->time >= f_time)
		{

			struct Event * new = malloc(sizeof(struct Event));
			new->time = f_time;
			new->nodeId = n_NodeId;
			new->dataId = n_DataId;
			new->next = current;
			prev->next = new;
			return 0;
		}
		prev = current;
		current = current->next;
	}

	if (current == NULL && prev->time <= f_time)
	{
		struct Event * new = malloc(sizeof(struct Event));
		new->time = f_time;
		new->nodeId = n_NodeId;
		new->dataId = n_DataId;
		new->next = NULL;
		prev->next = new;
		return 0;
	}
	return -1;
}

void print_receive()
{
	printf("Receive List: \n");
	struct Event * event = list_Receive;
	if (event == NULL)
		printf("empty\n");
	while (event != NULL)
	{
		printf("%f\t%d\t%d\n", event->time, event->nodeId, event->dataId);
		event = event->next;
	}
}

// return 1 if data in cache, return 0 if not
int if_in_cache(int n_NodeId, int n_DataId)
{
	int n_InCache = 0;
	struct Item * cache = list_Node[n_NodeId - 1].cache;
	while (cache != NULL)
	{
		if (cache->data.id == n_DataId)
			n_InCache = 1;
		cache = cache->next;
	}

	return n_InCache;
}

// append new item to the end of cache list by node id and data id
int add_to_cache(int n_NodeId, int n_DataId)
{
	n_Insert_Cache++;
	int n_Size = 1;
	if (list_Node[n_NodeId - 1].cache == NULL)
	{
		list_Node[n_NodeId - 1].cache = malloc(sizeof(struct Item));
		list_Node[n_NodeId - 1].cache->data = list_Data[n_DataId - 1];
		list_Node[n_NodeId - 1].cache->next = NULL;
		return 0;
	}

	struct Item * cache = list_Node[n_NodeId - 1].cache;
	while (cache->next != NULL)
	{
		cache = cache->next;
		n_Size++;
	}

	// if the cache is full, delete the first item in the list
	if (n_Size == n_Max_Cache_Size)
	{
		struct Item * head = list_Node[n_NodeId - 1].cache;
		head->data = head->next->data;
		struct Item * duplicate = head->next;
		head->next = head->next->next;
		free(duplicate);
		n_Replacement++;
	}

	// add new item to the end of the list
	cache->next = malloc(sizeof(struct Item));
	cache->next->data = list_Data[n_DataId - 1];
	cache->next->next = NULL;
	return 0;
}

// remove existing item from cache by node id and data id (not for duplicate item!)
int remove_from_cache(int n_NodeId, int n_DataId)
{
	struct Item * cache = list_Node[n_NodeId - 1].cache;

	// if it is the 1st item
	if (cache->data.id == n_DataId)
	{
		// if it is the only item in cache
		if (cache->next == NULL)
		{
			free(cache);
			list_Node[n_NodeId - 1].cache = NULL;
			return 0;
		}
		cache->data = cache->next->data;
		struct Item * duplicate = cache->next;
		cache->next = cache->next->next;
		free(duplicate);
		return 0;
	}

	while (cache->next != NULL)
	{
		if (cache->next->data.id == n_DataId)
		{
			struct Item * duplicate = cache->next;
			cache->next = cache->next->next;
			free(duplicate);
		}
		else
			cache = cache->next;
	}
	return 0;
}

// print current node cache list by node id
void print_node_cache(int n_NodeId)
{
	printf("Cache in Node %d: \n", n_NodeId);
	struct Item * current_item = list_Node[n_NodeId - 1].cache;
	if (current_item == NULL)
		printf("empty\n");
	while (current_item != NULL)
	{
		printf("%d\t(%f, %f)\t%f\n", current_item->data.id, current_item->data.location.x,
				current_item->data.location.y,
				current_item->data.time);
		current_item = current_item->next;
	}

}

// return 1 if data in wait, return 0 if not
int if_in_wait(int n_NodeId, int n_DataId)
{
	if (list_Node[n_NodeId - 1].wait == NULL)
		return 0;
	int n_InWait = 0;
	struct Item * wait = list_Node[n_NodeId - 1].wait;
	while (wait != NULL)
	{
		if (wait->data.id == n_DataId)
			n_InWait = 1;
		wait = wait->next;
	}

	return n_InWait;
}

// append new item to the end of wait list by node id and data id
int add_to_wait(float f_time, int n_NodeId, int n_DataId)
{
	if (list_Node[n_NodeId - 1].wait == NULL)
	{
		list_Node[n_NodeId - 1].wait = malloc(sizeof(struct Item));
		list_Node[n_NodeId - 1].wait->time = f_time;
		list_Node[n_NodeId - 1].wait->data = list_Data[n_DataId - 1];
		list_Node[n_NodeId - 1].wait->next = NULL;
		return 0;
	}

	struct Item * wait = list_Node[n_NodeId - 1].wait;
	while (wait->next != NULL)
	{
		wait = wait->next;
	}
	wait->next = malloc(sizeof(struct Item));
	wait->next->time = f_time;
	wait->next->data = list_Data[n_DataId - 1];
	wait->next->next = NULL;
	return 0;
}

// remove existing item from wait by node id and data id (not for duplicate item!)
int remove_from_wait(int n_NodeId, int n_DataId)
{
	struct Item * wait = list_Node[n_NodeId - 1].wait;

	// if it is the 1st item
	if (wait->data.id == n_DataId)
	{
		// if it is the only item in cache
		if (wait->next == NULL)
		{
			free(wait);
			list_Node[n_NodeId - 1].wait = NULL;
			return 0;
		}
		wait->data = wait->next->data;
		struct Item * duplicate = wait->next;
		wait->next = wait->next->next;
		free(duplicate);
		return 0;
	}

	while (wait->next != NULL)
	{
		if (wait->next->data.id == n_DataId)
		{
			struct Item * duplicate = wait->next;
			wait->next = wait->next->next;
			free(duplicate);
		}
		else
			wait = wait->next;
	}
	return 0;
}

// print current node wait list by node id
void print_node_wait(int n_NodeId)
{
	printf("Wait in Node %d: \n", n_NodeId);
	struct Item * current_item = list_Node[n_NodeId - 1].wait;
	if (current_item == NULL)
		printf("empty\n");
	while (current_item != NULL)
	{
		printf("data id:%d\tdata location:(%f, %f)\tvalid time:%f\tsend time:%f\n", current_item->data.id,
				current_item->data.location.x,
				current_item->data.location.y,
				current_item->data.time, current_item->time);
		current_item = current_item->next;
	}
}

// get neighbors with transmission range and return a linked list
struct Neighbor * get_neighbors(int n_NodeId)
{
	struct Neighbor * neighbors = NULL;
	int i;
	for (i = 0; i < n_NodeNo; i++)
	{
		if (i == n_NodeId - 1)
			continue;
		if (cal_distance(list_Node[i].location, list_Node[n_NodeId - 1].location) <= n_Tx_Range)
		{
			if (neighbors == NULL)
			{
				neighbors = malloc(sizeof(struct Neighbor));
				neighbors->node = list_Node[i];
				neighbors->next = NULL;
			}

			else // add to head of list
			{
				struct Neighbor * current = neighbors;
				while (current->next != NULL)
				{
					current = current->next;
				}
				current->next = malloc(sizeof(struct Neighbor));
				current->next->node = list_Node[i];
				current->next->next = NULL;
			}
		}
	}

	return neighbors;
}

// calculate distance between 2 locations
double cal_distance(struct Location l1, struct Location l2)
{
	return sqrt((l1.x - l2.x) * (l1.x - l2.x) + (l1.y - l2.y) * (l1.y - l2.y));
}

void print_neighbor_list(int n_NodeId)
{
	struct Neighbor * neighbor = get_neighbors(n_NodeId);

	printf("Neighbor list of Node %d:\n", n_NodeId);
	if (neighbor == NULL)
		printf("empty!\n");
	while (neighbor != NULL)
	{
		printf("%d\n", neighbor->node.id);
		neighbor = neighbor->next;
	}
	return;
}

int get_random_int(int stream)
{
	return lcgrand(stream) * 1000000;
}

