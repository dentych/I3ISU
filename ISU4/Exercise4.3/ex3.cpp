#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include "Vector.hpp"

using namespace std;

struct Data {
	int id;
	Vector * vec;
	int waitTime;
};

void * vectorThread(void * arg) {
	Data * d = (Data *) arg;

	for (int i = 0; i < 10; i++) {
		if (!d->vec->setAndTest(d->id)) {
			cout << "*** ERROR in thread # " << d->id << " ***" << endl;
		}
		else
			cout << "--- SUCCESS in thread #" << d->id << " ---" << endl;

		usleep(d->waitTime);
	}

	delete d;
}

int main() {
	const int SIZE = 10;

	Vector vec(15000);
	pthread_t threads[SIZE];

	for (int i = 0; i < SIZE; i++) {
		Data * d = new Data;
		d->vec = &vec;
		d->id = i;
		d->waitTime = 50000;
		pthread_create(&threads[i], NULL, vectorThread, d);
	}
	
	int errors = 0;
	for (int i = 0; i < SIZE; i++) {
		pthread_join(threads[i], NULL);
	}
}
