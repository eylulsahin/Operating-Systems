
//  Created by Eylul Sahin on 8.11.2020.
//

#include <stdio.h>
#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

using namespace std;

int counter = 0;
int seats[2][50];
int turn = 0;


void *reservation(void * num)
{
    int *id = (int *) num;
    
    while(true)
    {
        //non critical region
        if(counter!=100)
        {
        while(turn != *id-1){} // loop & busy wait
        //critical region
        if(counter!=100)
        {cout << "Agency "<< *id <<" Entered Critical Region" <<endl;}
            
        //get random seat
        int i = (rand() % 2);
        int j = (rand() % 50);
            if(seats[i][j] != 0)
            {//do nothing
                if(counter!=100)
                {cout << "Agency "<< *id <<" Exit Critical Region" <<endl<<endl;}
            }
            else if(seats[i][j]==0) //unreserved seat
            {
                seats[i][j] = *id; //reserve the seat
                cout << "Seat Number " << ((50*i)+j)+1 <<" is reserved by Agency " << *id << endl;
                counter++;
                if(counter!=100)
                {cout << "Agency "<< *id <<" Exit Critical Region" <<endl<<endl;}
                turn = ((*id) % 2); //change turn
            }
        }
    }
}



void *ticketsystem(void * dummy2)
{
    pthread_t thread1,thread2;
    for (int i = 0; i <2; i++)
        for (int j = 0; j < 50; j++)
                seats[i][j] = 0;

    int id1 = 1, id2 = 2;

    pthread_create(&thread1,NULL,reservation,(void*)&id1);
    pthread_create(&thread2,NULL,reservation,(void*)&id2);

    while(true)
    {
        if(counter == 100) // all seats are reserved
        {
            /* Wait till threads are complete before main continues. Unless we  */
            /* wait we run the risk of executing an exit which will terminate   */
            /* the process and all threads before the threads have completed.   */

            cout << "No Seats Left" << endl << endl;
            cout << "Plane is full: " << endl << endl;

            for (int i = 0; i < 2; i++)
            {
                for (int j = 0; j < 50; j++)
                {
                    cout << seats[i][j];
                }
                cout << endl;
            }
            
            cout << endl << endl;

            pthread_join(thread1,NULL);
            pthread_join(thread2,NULL);
        
            pthread_exit(NULL);
        }

    }
}

int main()
{
    // create the main thread
    pthread_t parent_thread;
    pthread_create(&parent_thread,NULL,ticketsystem,NULL);

    pthread_join(parent_thread,NULL);

    return 0;
}
