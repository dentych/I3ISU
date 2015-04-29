#include <iostream>
#include <string>
#include <unistd.h>
#include <stdlib.h>
#include "../inc/CarHandlers.hpp"
#include "../inc/EntryHandlers.hpp"
#include "../inc/ExitHandlers.hpp"

using namespace std;

void * entryGuardThread(void *data) {
	MsgQueue *mq = static_cast<MsgQueue*>(data);
	GuardState state = ST_READY;
	queue<DoorOpenReq*> waitingReqs;

	for (;;) {
		unsigned long id;
		Message *msg = mq->receive(id);
		entryHandleMsg(msg, id, state, waitingReqs);
		delete msg;
	}
}

void * exitGuardThread(void *data) {
	MsgQueue *mq = static_cast<MsgQueue*>(data);
	GuardState state = ST_READY;
	queue<DoorOpenReq*> waitingReqs;

	for (;;) {
		unsigned long id;
		Message *msg = mq->receive(id);
		exitHandleMsg(msg, id, state, waitingReqs);
		delete msg;
	}
}

void * carThread(void *data) {
	// Convert void * data to CarData struct.
	CarData *cardata = static_cast<CarData*>(data);
	
	// Start ze car
	carGenerateInOpenReq(&cardata->carQ, cardata->entryMQ, cardata->ID);

	for (;;) {
		unsigned long id;
		Message *msg = (cardata->carQ).receive(id);
		carHandleMsg(id, msg, cardata);
		delete msg;
	}

	delete cardata;
}

int main() {
	srand(time(NULL));

	int carAmount = 25;

	// Guard message queue
	MsgQueue entryQueue(carAmount);
	MsgQueue exitQueue(carAmount);

	// Thread variables
	pthread_t entryGuard, exitGuard, cars[carAmount];

	if (pthread_create(&entryGuard, NULL, entryGuardThread, &entryQueue) != 0) {
		cout << "Error creating entryGuard thread.." << endl;
		return -1;
	}
	if (pthread_create(&exitGuard, NULL, exitGuardThread, &exitQueue) != 0) {
		cout << "Error creating exitGuard thread.." << endl;
		return -1;
	}
	for (int i = 0; i < carAmount; i++) {
		CarData *data = new CarData;
		data->entryMQ = &entryQueue;
		data->exitMQ = &exitQueue;
		data->ID = i;
		if (pthread_create(&cars[i], NULL, carThread, data) != 0) {
			cout << "Error creating car #" << data->ID;
		}
	}

	pthread_join(entryGuard, NULL);
	pthread_join(exitGuard, NULL);
	for (int i = 0; i < carAmount; i++) {
		pthread_join(cars[i], NULL);
	}
}
