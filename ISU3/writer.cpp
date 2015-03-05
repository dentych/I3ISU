#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include "Vector.hpp"

using namespace std;

struct Data
{
	Vector* vec;
	int id;
};

void* writerfunc(void *ptr)
{
	Data d = *(Data*)ptr;

	for(int i = 0; i<10; i++)
	{

		if(!d.vec->setAndTest(d.id))
			cout << "ERROR!!!!! ID#: " << d.id << endl;

		sleep(1);

	}
}


int main()
{
	Vector vec;
	int Size = 10;

	pthread_t threads[Size];

	for(int i = 0; i < Size; i++)
	{
		Data *d = new Data;
		d->id = i;
		d->vec = &vec;

		pthread_create(&threads[i], NULL,&writerfunc, d);
	}

	for(int i = 0; i < Size; i++)
	{
		pthread_join(threads[i], NULL);
	}

}

