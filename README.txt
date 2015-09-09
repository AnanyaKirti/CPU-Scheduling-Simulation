# CPU Scheduling

This Program Simulates a CPU Scheduling algorithm. It gives the CPU to an algorithm with the maximum priority which depends on 
 	1. Priority	{p}			40%
 	2. Wait Time.			30%
 	3. Total Cost 			30%

Each process has a Program Control block(PCB), which consists of the Process ID (PID) its memory needs ,arrival_time, burst_time, turnaround_time, wait_time, end_time, and remaining_time;

Each Process must have a unique PID, to keep things simple I would generate their PID in increasing order, which would ensure a unique PID, and would not require book-keeping. The ideal method to maintain the PID would be to generate the PID randomly and then check if there is a process with that PID, if there is such a process, generate another PID, and check again.

The system is assumed to have R resources, each resource R(j) has a cost C(j) associated with it, and a capacity I(j) associated with it. The capacity is represented as I(j) instruction per second.

I have creted a n*2 matrix for each resource, where ith the row represents the ID of the resource, and the 1st column represents the Cost of using that resource, and the 2nd column represents the capacity of the resource.

Each new Process a requires a subset of the resources. Each Process hence has a cost and capacity associated with it. I would choose this subset randomly from the resource list. 

The Process might require one or more of this resource, in a sequence. Hence I would use a simple array of random size. Once this array is empty, I would assume that the process has finished executing and hence should be removed from the CPU.

If a process is running, and yet the scheduler decides to allot the CPU time to another process (Context Switch) then the current state of the process is saved in it's PCB. And another program is given CPU time.

To run the Stimulation
 
 $ ./main no_of_resources no_of_processes.

 I have assigned weights to 
 	1. Priority	{p}			40%
 	2. Wait Time.			30%
 	3. Total Cost 			30%
 which can be tweeked, once they are set, they should not be altered.

 # Observations.
 Since my scheduler keeps waiting time as a consideration for context switch there would be no starvation.

 The performance of an algorithm depends on the system it is being built for.
 My algorithm was better than FCFS, which had high waiting time, and reponse time.
 For a certain process, STRF was faster, however if there was a long burst of CPU given, then my algorithm was better.


 I have made some functions, which have some overhead ,I did not consider these overheads for my experimentation,as there might be a better way to implement these.

 Scheduling has some over head, which is not to be neglected, in which case FCFS has an advantage.