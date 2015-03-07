#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include "Vector.hpp"

using namespace std;

struct Data {
	int id;
	Vector * vec;
	sem_t * sem;
	int waitTime;
};

void * vectorThread(void * arg) {
	Data * d = (Data *) arg;

	for (int i = 0; i < 10; i++) {
		sem_wait(d->sem);

		if (!d->vec->setAndTest(d->id))
			cout << "*** ERROR in thread # " << d->id << " ***" << endl;
		else
			cout << "--- SUCCESS in thread #" << d->id << " ---" << endl;
	
		sem_post(d->sem);

		usleep(d->waitTime);
	}

	delete d;
}

int main() {
	Vector vec(100000);

	const int SIZE = 10;
	pthread_t threads[SIZE];
	sem_t sem;
	sem_init(&sem, 0, 1);

	for (int i = 0; i < SIZE; i++) {
		Data * d = new Data;
		d->vec = &vec;
		d->sem = &sem;
		d->id = i;
		d->waitTime = 500000;
		pthread_create(&threads[i], NULL, vectorThread, d);
	}

	for (int i = 0; i < SIZE; i++) {
		pthread_join(threads[i], NULL);
	}

	sem_destroy(&sem);
}
