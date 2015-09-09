/**************************
*
*	Ananya Kirti
*
**************************/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


#define MAXSIZE 25
#define MAXTIME 20
#define BASE 70
#define RESOURCE_REQUIRED 1000


// assign scheduler weights.
#define PRIORITY_WEIGHT			4
#define WAITING_TIME_WEIGHT		3
#define TOTAL_COST_WEIGHT 		3


// PCB Structs.
struct PCB{
	int PID;
	int state;
	/*
	-1  : Inactive
	 0  : Active, in ready queue
	 1  : Runing
	-2  : Completed
	*/
	int resources_needed[MAXSIZE];
	int no_of_resources_needed;
	int p;
	int total_cost;
	int scheduler_priority;
	int arrival_time,burst_time,turnaround_time,wait_time,end_time,remaining_time;
	int response_time;
};


void generate_resources(int matrix[][2], int rows, int columns){
	// this initiatlises the resource matrix.
	int i,j, dummy;
	for (int i = 0; i < rows; ++i){
		for (int j = 0; j < columns; ++j){
			dummy = random()%MAXSIZE;
			// ensure that this is greater than zero.
			while(dummy <= 0){
				dummy = random()%MAXSIZE;
			}
			matrix[i][j] = dummy;
		}
	}
	printf("Resources generated randomly.\n");
}

// function to print the system resources.
void print_resources(int resources[][2], int rows, int columns){
	printf("The System Resources are:\n");
	int i;
	for (i = 0; i < rows; i++){
		printf("%d, %d \n", resources[i][0], resources[i][1]);
	}
}


// function to dynamically generate processes.
void generate_processes(struct PCB processes[], int resources[][2], int no_of_pocesses, int no_of_resources){
	// create the processes.
	int i,j,index, dummy,p;
	int time = 0 ;
	for (i = 0; i < no_of_pocesses; i++){
		dummy = random()%MAXSIZE;


		processes[i].PID                = BASE +i;
		processes[i].state              =  -1;
		processes[i].arrival_time       = time;
		processes[i].burst_time         = -1;
		processes[i].turnaround_time    = -1;
		processes[i].wait_time          =  0;
		processes[i].end_time           = -1;
		processes[i].remaining_time     = -1;
		processes[i].response_time      = -1;

		time = time + (random()% MAXTIME);

		p = 0;
		index = random() % (no_of_resources -1);
		while((index) <= 0){
			index = random()%(no_of_resources -1);
		}
		for (j = 0; j < (no_of_resources -1); j++){
			if( j < index){
				dummy = random()%(no_of_resources -1);
				// ensure that this is greater than zero.
				processes[i].resources_needed[j] = dummy;
				p = p + resources[dummy][0];
			}
			else{
				processes[i].resources_needed[j] = -1;
			}
			// printf("%d,", processes[i].resources_needed[j]);
		}
		// printf("\n" );

		// printf("%d\n", p);
		processes[i].total_cost         =  p;
		processes[i].p                  =  p/index;
		processes[i].scheduler_priority =  ((processes[i].p * PRIORITY_WEIGHT) + (processes[i].wait_time * WAITING_TIME_WEIGHT) + (processes[i].total_cost * TOTAL_COST_WEIGHT) )/10;	//this I would give, with weights.
		// printf("%d\n", processes[i].scheduler_priority);
	}
	printf("Processes generated randomly.\n");
}


// function which returns the highest prioity process. This could be implemented as a binary heap.
int traverse_priority_queue(struct PCB processes[], int resources[][2], int no_of_pocesses, int no_of_resources, int time_increment, int time){
	// this returns the id of the max priority Process.
	// printf("Scheduling\n");
	int i, j, max_priority, max_id;
	max_priority = 0;
	max_id = 0;
	for (i = 0; i < no_of_pocesses; i++){
		// if the process has spawned.
		if (processes[i].state == 0){
			// printf("Sceduler %d\n",processes[i].scheduler_priority );
			processes[i].wait_time = processes[i].wait_time + time_increment;
			if (max_priority <= processes[i].scheduler_priority){
				max_priority = processes[i].scheduler_priority;
				max_id = i;
			}
		}
		if ((processes[i].arrival_time <= time) && (processes[i].state == -1)){
			processes[i].state = 0;
			processes[i].wait_time = 0;
		}
		// printf("priority: %d \t PID %d, \tSTATE:%d \n",processes[i].scheduler_priority , processes[i].PID, processes[i].state);
	}
	processes[max_id].wait_time = 0;
	processes[max_id].state = 1;
	return max_id; 
}

// this process updates the processes after every CPU Burst.
void update_priority_queue(struct PCB processes[], int resources[][2], int no_of_pocesses, int no_of_resources, int time_increment, int time){

	int i,j, dummy;
	int p, index = 0;
	for (i = 0; i < no_of_pocesses; i++){


		if (processes[i].state == 0){
			processes[i].wait_time = processes[i].wait_time + time_increment;
		}
		p = 0;
		for (j = 0; j < MAXSIZE; j++){
			if ((processes[i].resources_needed[j] >=0 )&& (processes[i].resources_needed[j] < no_of_resources)){
				dummy = processes[i].resources_needed[j];
				p = p + resources[dummy][0];
				index++;
			}
		}

		if (index >0 ){
			processes[i].p                  =  p/index;
		}
		else{
			processes[i].p                  =  -1;	
		}
		processes[i].scheduler_priority =  ((processes[i].p * PRIORITY_WEIGHT) + (processes[i].wait_time * WAITING_TIME_WEIGHT) + (processes[i].total_cost * TOTAL_COST_WEIGHT) )/10;	//this I would give, with weights.
		// printf("%d\n", processes[i].scheduler_priority);

		if ((processes[i].arrival_time <= time) && (processes[i].state == -1)){
			processes[i].state = 0;
			processes[i].wait_time = 0;
		}
		// printf("%d \n", processes[i].scheduler_priority);
	}
}


// this is the main scheduler algorithm.
void scheduler_algorithm(struct PCB processes[], int resources[][2], int no_of_pocesses, int no_of_resources){
	int i;
	int current, current_resource, dummy ,state, end_state = 0;
	int dum_current;
	long time_initial, time_final;
	int time_increment = 0;
	int runstate = 1;
	time_initial = time_final = 0;
	current = 0;
	while(end_state == 0){ 

		sleep(1);
		printf("#########################\n");

		time_initial = time_final;

		// select the process to be brought into CPU
		current = traverse_priority_queue(processes, resources, no_of_pocesses, no_of_resources,  time_increment,  time_final);
		// current = 0;
		printf("Process with PID %2d BEGINS  executing\n", current);
		// printf("%d\n",time_final );
		dummy = 0;
		for (i = 0; i < MAXSIZE; i++){
			if ((processes[current].resources_needed[i] >= 0) && (processes[current].resources_needed[i] < no_of_resources)){
				dummy = dummy + resources[processes[current].resources_needed[i]][1];
				processes[current].resources_needed[i] = -1;
				// printf("dummy %d\n", dummy);
			}
			if (dummy > MAXTIME){
				// printf("CPU Burst Over\n");
					break;
			}
		}
		state = 0;
		for (i = 0; i < MAXSIZE; i++){
			if ((processes[current].resources_needed[i] >= 0) && (processes[current].resources_needed[i] < no_of_resources)){
				state = 1;
				processes[current].state = 0;
				printf("Process with PID %2d PREMEPTED\n", current);
				break;
			}
		}
		if (state == 0){
			processes[current].state = -2;
			printf("Process with PID %2d FINISHED executing\n", current);
		}
		time_final = time_final + dummy;
		time_increment = dummy;
		update_priority_queue(processes, resources, no_of_pocesses, no_of_resources,  time_increment,  time_final);
		
		end_state = 1;
		for (i = 0; i < no_of_pocesses; i++){
			if (processes[i].state != -2){
				end_state = 0;
				break;
			}
		}

	}
}


// main function.
int main(int argc, char const *argv[]){
	printf("CPU Scheduling!\n");

	int const no_of_resources = atoi(argv[1]);
	int resources[no_of_resources][2]; 						// this is an matrix of resources, each resource has a cost and capacity associated with it.
	generate_resources(resources, no_of_resources, 2); 		// we would randomise this.
	// print_resources(resources, no_of_resources, 2);		// function to print the resources.

	int const no_of_pocesses = atoi(argv[2]);
	struct PCB processes[no_of_pocesses];										// PCB of a process, implemented as an array struct.
	generate_processes(processes, resources, no_of_pocesses, no_of_resources);	// this randomly generates a array of PCBs

	printf("System Setup Complete.\n");
	scheduler_algorithm(processes, resources, no_of_pocesses, no_of_resources);
}