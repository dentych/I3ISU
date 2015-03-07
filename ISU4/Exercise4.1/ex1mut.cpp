#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include "Vector.hpp"

using namespace std;

struct Data {
	int id;
	Vector * vec;
	pthread_mutex_t * mut;
	int waitTime;
};

void * vectorThread(void * arg) {
	Data * d = (Data *) arg;

	for (int i = 0; i < 10; i++) {
		pthread_mutex_lock(d->mut);
		
		if (!d->vec->setAndTest(d->id))
			cout << "*** ERROR in thread # " << d->id << " ***" << endl;
		else
			cout << "--- SUCCESS in thread #" << d->id << " ---" << endl;
	
		pthread_mutex_unlock(d->mut);

		usleep(d->waitTime);
	}

	delete d;
}

int main() {
	Vector vec(100000);

	const int SIZE = 10;
	pthread_t threads[SIZE];
	pthread_mutex_t mut;
	pthread_mutex_init(&mut, NULL);

	for (int i = 0; i < SIZE; i++) {
		Data * d = new Data;
		d->vec = &vec;
		d->mut = &mut;
		d->id = i;
		d->waitTime = 500000;
		pthread_create(&threads[i], NULL, vectorThread, d);
	}

	for (int i = 0; i < SIZE; i++) {
		pthread_join(threads[i], NULL);
	}

	pthread_mutex_destroy(&mut);
}
