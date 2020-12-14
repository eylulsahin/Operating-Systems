#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include <stdlib.h> 
#include <queue> 
#include <semaphore.h>
using namespace std;

#define NUM_THREADS 10
#define MEMORY_SIZE 150

struct node
{
	int id;
	int size;
};


queue<node> myqueue; // shared que
pthread_mutex_t sharedLock = PTHREAD_MUTEX_INITIALIZER; // mutex
pthread_t server; // server thread handle
sem_t semlist[NUM_THREADS]; // thread semaphores

int thread_message[NUM_THREADS]; // thread memory information
char memory[MEMORY_SIZE]; // memory size
int sizerem = 0;


void my_malloc(int thread_id, int size)
{
	//This function will add the struct to the queue
	pthread_mutex_lock(&sharedLock);
	node dummy;
	dummy.id = thread_id;
	dummy.size = size;
	myqueue.push(dummy);
	pthread_mutex_unlock(&sharedLock);
}

void * server_function(void *)
{
	//This function should grant or decline a thread depending on memory size.
	//append access requests to myqueue
	
	int flag=0; //to keep count of the number of threads
	while(true){
		
		if(!myqueue.empty()) //when there is an item in the queue, pop the item
		{
			flag++; 
			node request = myqueue.front(); //item to pop, contains requesting thread id and size
			myqueue.pop();
			//check if there is available space 
			int available = MEMORY_SIZE - sizerem;
			if(available < request.size) //not enough space, return -1
				thread_message[request.id] = -1;	
			else
			{  
				thread_message[request.id] = sizerem; //there is enough space, return the start adress
			}
			sem_post(&semlist[request.id]);
		}
		else if (flag == NUM_THREADS && myqueue.empty())
		{
			break;
		}
		
	}
}

void * thread_function(void* id) 
{
	//This function will create a random size, and call my_malloc
	//Block
	//Then fill the memory with id's or give an error prompt


	int randomsize = rand() % (MEMORY_SIZE/6); //generate random size for the memory 
	//cout << "random size is " << randomsize << endl; //for debug
	int idptr = *((int *) id);
	char charid = '0'+idptr;
	my_malloc(idptr, randomsize);

	pthread_mutex_lock(&sharedLock); //blocked until server processes its request
	sem_wait(&semlist[idptr]); //wait for the answer

	if(thread_message[idptr] == -1) //no available memory, access not accepted
	{
		cout << "Thread " << idptr << ": Not enough memory" << endl; 
	}
	else //grant access
	{  
		for (int i=sizerem; i<randomsize+sizerem; i++)
			{
				//cout<<"i is "<<i<< ", idptr is "<<idptr<<endl; //for debug purposes
				memory[i] = charid ; //set to char value of its own id
			}
		sizerem=sizerem+randomsize;
	}

	pthread_mutex_unlock(&sharedLock);
}

void init()	 
{
	pthread_mutex_lock(&sharedLock);	//lock
	for(int i = 0; i < NUM_THREADS; i++) //initialize all the semaphores
	{sem_init(&semlist[i],0,0);}
	for (int i = 0; i < MEMORY_SIZE; i++)	//initialize memory 
  	{char zero = '0'; memory[i] = zero;}
   	pthread_create(&server,NULL,server_function,NULL); //server
	pthread_mutex_unlock(&sharedLock); //unlock
}



void dump_memory() 
{
// You need to print the whole memory array here.
	cout<<"Memory Dump: "<<endl;
	for (int i=0; i<MEMORY_SIZE; i++){
		cout << memory[i];  
	}
	cout << endl;
}

int main (int argc, char *argv[])
 {
	 pthread_t thread[NUM_THREADS];
 	//You need to create a thread ID array here
 	int idarr[10] = {0,1,2,3,4,5,6,7,8,9};

 	init();	 // call init

 	//You need to create threads with using thread ID array, using pthread_create()
 	for(int i=0; i < NUM_THREADS; i++){
         pthread_create(&thread[i],NULL,thread_function,(void *) &idarr[i]);
     }

 	//You need to join the threads
 	for(int i=0; i < NUM_THREADS; i++){
         pthread_join(thread[i],NULL);
     }
 	
 	dump_memory();  // this will print out the memory

 	printf("\nMemory Indexes:\n" );
 	for (int i = 0; i < NUM_THREADS; i++)
 	{
 		printf("%d," ,thread_message[i]); // this will print out the memory indexes
 	}
 	printf("\nTerminating...\n");
 }
