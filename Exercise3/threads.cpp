#include <iostream>
#include <pthread.h>
#include <unistd.h>

using namespace std;


void* printfunc(void *ptr);

int main()
{

	pthread_t thread1, thread2;

	int tnumber1 = 1;
	int tnumber2 = 2;


	pthread_create(&thread1, NULL, &printfunc, (void*) &tnumber1);
	pthread_create(&thread2, NULL, &printfunc, (void*) &tnumber2);


	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);

}




void* printfunc(void *ptr)
{

	int tnumber = *(int*)ptr;

	int number = 0;

	while( number != 10)
	{


		cout << "Hello #" << number << " from thread #" << tnumber << "\n";
		number++;
		sleep(1);

	}

	pthread_exit(0);
}
