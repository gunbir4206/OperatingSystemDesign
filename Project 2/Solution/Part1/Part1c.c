// Project 2 - Part 1c

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <string.h>
#include <stddef.h>


static int *global_maximum; // global max variable
static int *global_sum;   // global sum variable
static int *global_minimum;   // global min variable
static int *terminate; // variable for terminating processes
static int inputs = 15;

int main(int argc, char* argv[]) {

	global_maximum = mmap(NULL, sizeof *global_maximum, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	*global_maximum = 0;

	global_sum = mmap(NULL, sizeof *global_sum, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	*global_sum = 0;

	global_minimum = mmap(NULL, sizeof *global_minimum, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	*global_minimum = 1000;

	terminate = mmap(NULL, sizeof *terminate, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	*terminate = 0;

	FILE *fp = fopen("data.txt", "r");  // open data.txt for reading inputs
	int elements[inputs];   // declare integer array called elements for inputs from file
	int i = 0;
	int num;
	while (fscanf(fp, "%d", &num) > 0) {  
		elements[i] = num;  // append input elements to array
		i++;
	}
	fclose(fp);    // close file 


	FILE *writeF = fopen("outputC.txt", "w");  // open output file for writing

	int pid;  // fork processes
	pid = fork();

	if (pid == 0) {  // child
		pid = fork();   // fork
		if (pid == 0) {   // child
			pid = fork();  // fork
			if (pid == 0) {   // child
				printf("Hi I'm process %d and my parent is %d.\n", getpid(), getppid());   // identify yourself and your parent

				int max = calculateMax(elements, 0);  // calculate max
				int sum = calculateSum(elements, 0);  // calculate sum
				int min = calculateMin(elements, 0);  // calculate min

				if (max > *global_maximum) {  // set maximum
					*global_maximum = max;
				}
				if (min < *global_minimum) {  // set minimum
					*global_minimum = min;
				}
				*global_sum += sum;

				wait(NULL);
				printf("Max=%d\n", *global_maximum);  // print results...
				printf("Min=%d\n", *global_minimum);
				printf("Sum=%d\n", *global_sum);

				exit(0);   // successful termination
			}
			else if (pid > 0) {
				printf("Hi I'm process %d and my parent is %d.\n", getpid(), getppid());   // identity child/parent

				int max = calculateMax(elements, (inputs / 5));  // calculate max
				int sum = calculateSum(elements, (inputs / 5));  // calculate sum
				int min = calculateMin(elements, (inputs / 5));  // calculate min

				if (max > *global_maximum) {  // set maximum
					*global_maximum = max;
				}   
				if (min < *global_minimum) {  // set minimum
					*global_minimum = min;
				}
				*global_sum += sum;

				exit(0);  
			}
		}
		else if (pid > 0) {
			printf("Hi I'm process %d and my parent is %d.\n", getpid(), getppid());

			int max = calculateMax(elements, (2 * inputs / 5));   // calculate max
			int sum = calculateSum(elements, (2 * inputs / 5));   // calculate sum
			int min = calculateMin(elements, (2 * inputs / 5));   // calculate min

			if (max > *global_maximum) {   // set maximum
				*global_maximum = max;
			}
			if (min < *global_minimum) {   // set minimum
				*global_minimum = min;
			}
			*global_sum += sum;

			exit(0);
		}
	}
	else if (pid > 0) {
		pid = fork();
		if (pid == 0) {
			printf("Hi I'm process %d and my parent is %d.\n", getpid(), getppid());

			int max = calculateMax(elements, (3 * inputs / 5));     // calculate max
			int sum = calculateSum(elements, (3 * inputs / 5));    // calculate sum
			int min = calculateMin(elements, (3 * inputs / 5));    // calculate min

			if (max > *global_maximum) {   // set maximum
				*global_maximum = max;
			}  
			if (min < *global_minimum) {   // set minimum
				*global_minimum = min;
			}
			*global_sum += sum;

			exit(0);
		}
		else if (pid > 0) {
			printf("Hi I'm process %d and my parent is %d.\n", getpid(), getppid());

			int max = calculateMax(elements, (4 * inputs / 5));   // calculate max
			int sum = calculateSum(elements, (4 * inputs / 5));    // calculate sum
			int min = calculateMin(elements, (4 * inputs / 5));    // calculate min 

			if (max > *global_maximum) {   // set maximum
				*global_maximum = max;
			}
			if (min < *global_minimum) {   // set minimum
				*global_minimum = min;
			}
			*global_sum += sum;

			exit(0);

		}
	}

	fclose(writeF);  // close output file
	system("pause");   // pause output screen
	return 0;   // terminate program
}

void signal_handler(int signal) {

	if (signal == SIGINT) {   // interrupt/terminate
		printf("process %d caught SIGINT, terminating\n", getpid());
		*terminate = 1;
		exit(0);
	}
	else if (signal == SIGTSTP) {   // stop/terminate
		printf("process %d caught SIGTSTP, terminating\n", getpid());
		*terminate = 1;
		exit(0);
	}
	else if (signal == SIGTERM) {    // terminate
		printf("process %d caught SIGTERM, terminating\n", getpid());
		*terminate = 1;
		exit(0);
	}
	else {   // unknown signal
		printf("process %d caught unknown signal, terminating\n", getpid());
		*terminate = 1;
		exit(0);
	}

}

void detectSignals() {
	signal(SIGINT, signal_handler);
	signal(SIGTSTP, signal_handler);
	signal(SIGTERM, signal_handler);

	if (*terminate == 1) {   // terminate
		printf("process %d can die, terminating\n", getpid());
		exit(0);
	}

}

int calculateSum(int elements[inputs], int counter) {  // method to calculate sum
	int sum = 0;
	while (1) {    // infinite while loop
		detectSignals();
	}
	for (int i = counter; i < counter + (inputs / 5); i++) {
		sum = sum + elements[i];
	}
	return sum;
}

int calculateMin(int elements[inputs], int counter) {  // method to calculate minimum
	int min = elements[counter];
	while (1) {   // infinite while loop
		detectSignals();
	}
	for (int i = counter; i < counter + (inputs / 5); i++) {
		if (min > elements[i]) min = elements[i];
	}
	return min;
}

int calculateMax(int elements[inputs], int counter) {   // method to calculate maximum
	int max = elements[counter];
	while (1) {     // infinite while loop
		detectSignals();
	}
	for (int i = counter; i < counter + (inputs / 5); i++) {
		if (max < elements[i]) max = elements[i];
	}
	return max;
}





