#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h>
#include <time.h>

void logStart(char *tID);  // function to log that a new thread is started
void logFinish(char *tID); // function to log that a thread has finished its time

void startClock();	   // function to start program clock
long getCurrentTime(); // function to check current time since clock was started
time_t programClock;   // the global timer/clock for the program

typedef struct thread // represents a single thread
{
	char tid[4]; // id of the thread as read from file
	int starttime;
	int lifetime;
} Thread;

void *threadRun(void *t);						// the thread function, the code executed by each thread
int readFile(char *fileName, Thread **threads); // function to read the file content and build array of threads

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		printf("Input file name missing...exiting with error code -1\n");
		return -1;
	}

	startClock();

	Thread *thr = NULL;
	int count = readFile(argv[1], &thr);
	pthread_t workers[count];
	int i = 0;
	int index = 0;
	int track = 0;
	while (getCurrentTime() < 26) // put a suitable condition here to run your program
	{
		if (getCurrentTime() >= thr[index].starttime)
		{
			pthread_create(&workers[i++], NULL, threadRun, &thr[index]);
			track++;
			if(track>= count){
				break;
			}
			int old = thr[index].starttime;
			int min = 100;
			for (int j = 0; j < count; j++)
			{
				if (thr[j].starttime > old && thr[j].starttime < min)
				{
					index = j;
					min = thr[j].starttime;
				}
			}
		}
	}

	for(int i =0; i < count; i++){
		pthread_join(workers[i],NULL);
	}
	return 0;
}

int readFile(char *fileName, Thread **threads) // use this method in a suitable way to read file
{
	FILE *in = fopen(fileName, "r");
	if (!in)
	{
		printf("Child A: Error in opening input file...exiting with error code -1\n");
		return -1;
	}

	struct stat st;
	fstat(fileno(in), &st);
	char *fileContent = (char *)malloc(((int)st.st_size + 1) * sizeof(char));
	fileContent[0] = '\0';
	while (!feof(in))
	{
		char line[100];
		if (fgets(line, 100, in) != NULL)
		{
			strncat(fileContent, line, strlen(line));
		}
	}
	fclose(in);

	char *command = NULL;
	int threadCount = 0;
	char *fileCopy = (char *)malloc((strlen(fileContent) + 1) * sizeof(char));
	strcpy(fileCopy, fileContent);
	command = strtok(fileCopy, "\r\n");
	while (command != NULL)
	{
		threadCount++;
		command = strtok(NULL, "\r\n");
	}
	*threads = (Thread *)malloc(sizeof(Thread) * threadCount);

	char *lines[threadCount];
	command = NULL;
	int i = 0;
	command = strtok(fileContent, "\r\n");
	while (command != NULL)
	{
		lines[i] = malloc(sizeof(command) * sizeof(char));
		strcpy(lines[i], command);
		i++;
		command = strtok(NULL, "\r\n");
	}

	for (int k = 0; k < threadCount; k++)
	{
		char *token = NULL;
		int j = 0;
		token = strtok(lines[k], ";");
		while (token != NULL)
		{
			if (j == 0)
			{
				strcpy((*threads)[k].tid, token);
			}
			if (j == 1)
			{
				(*threads)[k].starttime = atoi(token);
			}
			if (j == 2)
			{
				(*threads)[k].lifetime = atoi(token);
			}
			j++;
			token = strtok(NULL, ";");
		}
	}
	return threadCount;
}

void logStart(char *tID) // invoke this method when you start a thread
{
	printf("[%ld] New Thread with ID %s is started.\n", getCurrentTime(), tID);
}

void logFinish(char *tID) // invoke this method when a thread is over
{
	printf("[%ld] Thread with ID %s is finished.\n", getCurrentTime(), tID);
}

void *threadRun(void *t) // implement this function in a suitable way
{
	Thread *thr = t;
	logStart(thr->tid);
	int limit = getCurrentTime() + thr->lifetime;
	while (getCurrentTime() < limit)
		;
	logFinish(thr->tid);
	return NULL;
}

void startClock() // invoke this method when you start servicing threads
{
	programClock = time(NULL);
}

long getCurrentTime() // invoke this method whenever you want to check how much time units passed
// since you invoked startClock()
{
	time_t now;
	now = time(NULL);
	return now - programClock;
}