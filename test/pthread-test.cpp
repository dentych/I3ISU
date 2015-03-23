#include <iostream>
#include <pthread.h>
#include <unistd.h>

using namespace std;

// Function prototypes
void * thread1(void *);
void * thread2(void *);

struct Data {
	pthread_mutex_t * mut;
	pthread_cond_t * cond;
	int id;
};

bool waiting = false;
bool opendoor = false;

int main() {
	pthread_mutex_t mut_entry = PTHREAD_MUTEX_INITIALIZER;
	pthread_cond_t cond_entry = PTHREAD_COND_INITIALIZER;

	pthread_t first;
	pthread_t second;

	Data * data = new Data;
	data->mut = &mut_entry;
	data->cond = &cond_entry;
	data->id = 0;
	pthread_create(&first, NULL, thread1, data);

	Data * data2 = new Data;
	data2->mut = &mut_entry;
	data2->cond = &cond_entry;
	data2->id = 1;
	pthread_create(&second, NULL, thread2, data2);

	pthread_join(first, NULL);
	pthread_join(second, NULL);
}

void * thread1(void * arg) {
	Data * data = (Data) arg;
	while (1) {
		pthread_mutex_lock(data->mut);
		cout << "Mutex locked in thread1" << endl;

		while (!waiting) {
			pthread_cond_wait(data->cond, data->mut);
			cout << "Waiting for cond in thread1" << endl;
		}
		opendoor = true;
		cout << "Door open." << endl;
		pthread_cond_signal(data->cond);
		cout << "Signal sent from thread1" << endl;
		while (waiting) {
			cout << "Waiting for cond in thread1" << endl;
			pthread_cond_wait(data->cond, data->mut);
		}
		opendoor = false;
		cout << "Door closed." << endl;
		pthread_mutex_unlock(data->mut);
		cout << "Mutex unlocked in thread1" << endl;
	}
}

void * thread2(void * arg) {
	Data * data = (Data) arg;
	while (1) {
		pthread_mutex_lock(data->mut);
		cout << "Mutex locked in thread2" << endl;
		waiting = true;
		cout << "Waiting true in thread2" << endl;
		pthread_cond_signal(data->cond);
		cout << "Cond signal sent from thread2" << endl;
		while (!opendoor) {
			cout << "Waiting for cond signal in thread2" << endl;
			pthread_cond_wait(data->cond, data->mut);
		}
		waiting = false;
		cout << "Waiting false in thread2" << endl;
		pthread_cond_signal(data->cond);
		pthread_mutex_unlock(data->mut);
	}
	sleep(1);
}