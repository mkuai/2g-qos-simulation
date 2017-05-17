// Meng Kuai
// Lab2 CS 580 Spring 2015
// 3/8/2015

#include "simlib.h"             /* Required for use of simlib.c. */

#define NUMBER_OF_CHANNELS    100  /* Number of channels */
#define EVENT_RECEIVE       1  /* Event type for new call arrival. */
#define EVENT_SEND   2  /* Event type for handoff call arrival. */
#define EVENT_END         3  /* Event type for departure. */
#define LIST_QUEUE              1  /* List number for queue. */
#define LIST_SERVER             2  /* List number for server. */
#define SAMPST_DELAYS           1  /* sampst variable for delays in queue. */
#define SAMPST_N_CHANNELS       2  /* sampst variable for number of used channels */
#define STREAM_NEW_INTERARRIVAL         1  /* Random-number stream for interarrivals. */
#define STREAM_HANDOFF_INTERARRIVAL     2  /* Random-number stream for interarrivals. */
#define STREAM_SERVICE                  2  /* Random-number stream for service times. */

/* Declare non-simlib global variables. */
int num_calls_arrived, num_calls_required;
float mean_new_interarrival, mean_handoff_interarrival, mean_service;
FILE  *infile, *outfile;
int n_current_channels, n_new_arrival, n_handoff_arrival, n_new_reject, n_handoff_reject;
float sim_time_previous, f_utilization, f_utilization_sum;

/* Declare non-simlib functions. */
void init_model(void);
void receive(void);
void send(void);
void finishing(void);
void report(void);


main()  /* Main function. */
{
    /* Open input and output files. */
    // check opening file is successful or not
    if ((infile = fopen("input.txt", "r")) == NULL) {
        printf("Cannot open input.txt!\n");
        return -1;
    }
    if ((outfile = fopen("output.txt", "w")) == NULL) {
        printf("Cannot open output.txt!\n");
        return -1;
    }

    /* Read input parameters. */
    //check return value is correct or not !!!
    if (fscanf(infile, "%f %f %f %d", &mean_new_interarrival, &mean_handoff_interarrival, &mean_service,
            &num_calls_required) == 0) {
        printf("Input file is not in correct format!\n");
        return -1;
    }

    /* Write report heading and input parameters. */

    fprintf(outfile, "Mean new interarrival time%11.3f minutes\n\n",
            mean_new_interarrival);
    fprintf(outfile, "Mean handoff interarrival time%11.3f minutes\n\n",
            mean_handoff_interarrival);
    fprintf(outfile, "Mean service time%16.3f minutes\n\n", mean_service);
    fprintf(outfile, "Number of calls served%14d\n\n\n", num_calls_required);

    /* Initialize simlib */

    init_simlib();

    /* Set maxatr = max(maximum number of attributes per record, 4) */

    maxatr = 4;  /* NEVER SET maxatr TO BE SMALLER THAN 4. */

    /* Initialize the model. */

    init_model();

    /* Run the simulation while more delays are still needed. */

    while (num_calls_arrived < num_calls_required) {

        /* Determine the next event. */

        timing();

        /* Invoke the appropriate event function. */

        switch (next_event_type) {
            case EVENT_RECEIVE:
                receive();
                break;
            case EVENT_SEND:
                send();
                break;
            case EVENT_END:
                finishing();
                break;
        }
    }

    /* Invoke the report generator and end the simulation. */

    report();

    fclose(infile);
    fclose(outfile);

    return 0;
}


void init_model(void)  /* Initialization function. */
{
    num_calls_arrived = 0;
    f_utilization = 0.0;
    sim_time_previous = sim_time; // record time for utilization
    n_current_channels = NUMBER_OF_CHANNELS;
    n_new_arrival = n_handoff_arrival = n_new_reject = n_handoff_reject = 0;
    // schedule next new arrival event
    event_schedule(sim_time + expon(mean_new_interarrival, STREAM_NEW_INTERARRIVAL), EVENT_RECEIVE);
    // schedule next handoff arrival event
    event_schedule(sim_time + expon(mean_handoff_interarrival, STREAM_HANDOFF_INTERARRIVAL), EVENT_SEND);
}


void receive(void)  /* Arrival event function. */
{
    num_calls_arrived++;
    n_new_arrival++;
    // schedule next arrival event
    event_schedule(sim_time + expon(mean_new_interarrival, STREAM_NEW_INTERARRIVAL), EVENT_RECEIVE);

    // if there are channels available, reduce one channel and schedule a new finishing event
    if (n_current_channels > 0) {
        // obtaint the utilization before number of channels changes
        f_utilization = (sim_time - sim_time_previous) * (NUMBER_OF_CHANNELS - n_current_channels);
        f_utilization_sum += f_utilization;
        sim_time_previous = sim_time; // record time for utilization
        n_current_channels--;
        event_schedule(sim_time + expon(mean_service, STREAM_SERVICE), EVENT_END);
    }
    // if there is no channel available, increase reject number
    else {
        n_new_reject++;
    }
}

void send(void)  /* Arrival event function. */
{
    num_calls_arrived++;
    n_handoff_arrival++;
    // schedule next arrival event
    event_schedule(sim_time + expon(mean_handoff_interarrival, STREAM_HANDOFF_INTERARRIVAL), EVENT_SEND);

    // if there are channels available, reduce one channel and schedule a new finishing event
    if (n_current_channels > 0) {
        f_utilization = (sim_time - sim_time_previous) * (NUMBER_OF_CHANNELS - n_current_channels);
        f_utilization_sum += f_utilization;
        sim_time_previous = sim_time; // record time for utilization
        n_current_channels--;
        event_schedule(sim_time + expon(mean_service, STREAM_SERVICE), EVENT_END);
    }
        // if there is no channel available, increase reject number
    else {
        n_handoff_reject++;
    }
}


void finishing(void)  /* Departure event function. */
{
    f_utilization = (sim_time - sim_time_previous) * (NUMBER_OF_CHANNELS - n_current_channels);
    f_utilization_sum += f_utilization;
    sim_time_previous = sim_time; // record time for utilization
    n_current_channels++;

}


void report(void)  /* Report generator function. */
{
    /* Get and write out estimates of desired measures of performance. */

    float utilization, cbp, hdp;
    utilization = f_utilization_sum / (sim_time * NUMBER_OF_CHANNELS);
    cbp = (float)n_new_reject/n_new_arrival;
    hdp = (float)n_handoff_reject/n_handoff_arrival;

    printf("%.3f\t%.3f\t%.3f", utilization, cbp, hdp);

    fprintf(outfile, "Simulation running time\t%f\n\n\n", sim_time);
    fprintf(outfile, "Channel utilization\t%f\n\n\n", utilization);
    fprintf(outfile, "Call Block Probability\t%f\n\n\n", cbp);
	fprintf(outfile, "Handoff Dropping Probability\t%f\n\n\n", hdp);
}

