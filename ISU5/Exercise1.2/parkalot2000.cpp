#include <iostream>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <pthread.h>

using namespace std;

// Enum for car state
enum CarState {
	ARRIVING,
	PARKED,
	EXITING,
	EXITED
};

// Thread function prototypes
void * plcs_entryguard(void *);
void * plcs_exitguard(void *);
void * car(void *);

// Global variables (protected by mutexes)
bool carWaitingEntry = false;
bool carWaitingExit = false;
bool doorEntryOpen = false;
bool doorExitOpen = false;

// Global run variable
bool run = true;

// Global mutexes and conditionals
// Conditionals
pthread_cond_t cond_entry = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_exit = PTHREAD_COND_INITIALIZER;
// Mutexes
pthread_mutex_t mutex_entry = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_exit = PTHREAD_MUTEX_INITIALIZER;

int main() {
	srand(time(NULL));
	// Initalization of variables
	const int maxcars = 10;
	// thread variables
	pthread_t cars[maxcars];
	pthread_t entryguard, exitguard;

	// Initialize the entry guard and exit guard.
	pthread_create(&entryguard, NULL, plcs_entryguard, NULL);
	pthread_create(&exitguard, NULL, plcs_exitguard, NULL);

	// Initialize car0 (only one car in this test)
	for (int i = 0; i < maxcars; i++) {
		int * id = new int;
		*id = i;
		pthread_create(&cars[i], NULL, car, id);
	}
	for (int i = 0; i < maxcars; i++) {
		pthread_join(cars[i], NULL);
	}

	return 0;
}

void * plcs_entryguard(void *) {
	while (run) {
		pthread_mutex_lock(&mutex_entry);
		while (!carWaitingEntry)
			pthread_cond_wait(&cond_entry, &mutex_entry);
		doorEntryOpen = true;
		pthread_cond_signal(&cond_entry);
		while (carWaitingEntry)
			pthread_cond_wait(&cond_entry, &mutex_entry);
		doorEntryOpen = false;
		pthread_mutex_unlock(&mutex_entry);
	}
}

void * plcs_exitguard(void *) {
	while (run) {
		pthread_mutex_lock(&mutex_exit);
		while (!carWaitingExit)
			pthread_cond_wait(&cond_exit, &mutex_exit);
		doorExitOpen = true;
		pthread_cond_signal(&cond_exit);
		while (carWaitingExit)
			pthread_cond_wait(&cond_exit, &mutex_exit);
		doorExitOpen = false;
		pthread_mutex_unlock(&mutex_exit);
	}
}

void * car(void * arg) {
	// VARIABLES <3
	int ID = *(int *) arg;
	CarState state = ARRIVING;
	bool alive = true;

	cout << "Car #" << ID << ": CREATED" << endl;

	while (alive) {
		if (state == ARRIVING) {
			cout << "Car #" << ID << ": ARRIVING" << endl;
			pthread_mutex_lock(&mutex_entry);
			carWaitingEntry = true;
			pthread_cond_signal(&cond_entry);
			while (!doorEntryOpen)
				pthread_cond_wait(&cond_entry, &mutex_entry);
			carWaitingEntry = false;
			state = PARKED;
			pthread_cond_signal(&cond_entry);
			pthread_mutex_unlock(&mutex_entry);
		}
		else if (state == PARKED) {
			int sleepTime = rand() % 10 + 1;
			cout << "Car #" << ID << ": PARKED - " << sleepTime << " SECONDS" << endl;
			sleep(sleepTime);
			state = EXITING;
		}
		else if (state == EXITING) {
			cout << "Car #" << ID << ": EXITING" << endl;
			pthread_mutex_lock(&mutex_exit);
			carWaitingExit = true;
			pthread_cond_signal(&cond_exit);
			while (!doorExitOpen)
				pthread_cond_wait(&cond_exit, &mutex_exit);
			carWaitingExit = false;
			state = EXITED;
			pthread_cond_signal(&cond_exit);
			pthread_mutex_unlock(&mutex_exit);
		}
		else {
			cout << "Car #" << ID << ": EXITED" << endl;
			int sleepTime = rand() % 10 + 1;
			sleep(sleepTime);
			state = ARRIVING;
		}
		usleep(100000);
	}
}
