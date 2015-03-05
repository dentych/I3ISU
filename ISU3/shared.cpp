#include <iostream>
#include <pthread.h>
#include <unistd.h>

using namespace std;


void* printfunc(void *ptr);
void* countfunc(void *ptr);

unsigned int shared = 0;

int main()
{

	pthread_t incrementer, reader;

	pthread_create(&incrementer, NULL, &countfunc, (void*)&shared);
	pthread_create(&reader, NULL, &printfunc, (void*)&shared);


	pthread_join(incrementer, NULL);
	pthread_join(reader, NULL);

}

void* countfunc(void *ptr)
{

	while(1)
	{

		shared++;
		sleep(1);

	}

	pthread_exit(0);
}

void* printfunc(void *ptr)
{

	while(1)
	{


		cout << "Shared er " << shared <<  endl;
		sleep(1);

	}

	pthread_exit(0);
}

