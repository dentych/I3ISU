#include <iostream>
#include <string>
#include <unistd.h>
#include <stdlib.h>
#include "../../Exercise1/Message.h"
#include "../../Exercise1/MsgQueue.h"

using namespace std;

enum {
	DOOR_IN_OPEN_REQ,
	DOOR_IN_OPEN_CFM,
	DOOR_OUT_OPEN_REQ,
	DOOR_OUT_OPEN_CFM,
	CAR_INSIDE,
	CAR_OUTSIDE
};

enum GuardState {
	ST_READY,
	ST_BUSY
};

struct CarData {
	MsgQueue *entryMQ;
	MsgQueue *exitMQ;
	int ID;
};

struct DoorOpenReq : public Message {
	MsgQueue *mq;
	int carID;
};

struct DoorOpenCfm : public Message {
	bool result;
};

struct CarInside : public Message {
	int id;
};

struct CarOutside : public Message {
	int id;
};

void db(string msg) {
	bool debugOn = false;

	if (debugOn) {
		cout << "\033[2;37m" << msg << "\033[0m" << endl;
	}
}

/* command = 0 closes the door, command = 1 opens the door */
void handleDoorRequest(unsigned long id, int carID = -1) {
	switch(id) {
		case DOOR_IN_OPEN_REQ:
			cout << "ENTRY door opened for " << carID << "." << endl;
			break;
		case DOOR_OUT_OPEN_REQ:
			cout << "EXIT door opened for " << carID << "." << endl;
			break;
		case CAR_INSIDE:
			cout << "ENTRY door closed." << endl;
			break;
		case CAR_OUTSIDE:
			cout << "EXIT door closed." << endl;
			break;
	}
}

void dispatcher(Message * msg, unsigned long id, int who = -1) {
	switch(id) {
		case DOOR_IN_OPEN_REQ:
		{
			DoorOpenReq *req = static_cast<DoorOpenReq*>(msg);
			handleDoorRequest(id, req->carID);

			DoorOpenCfm * cfm = new DoorOpenCfm;
			cfm->result = 0;
			req->mq->send(DOOR_IN_OPEN_CFM, cfm);
			break;
		}
		case DOOR_OUT_OPEN_REQ:
		{
			DoorOpenReq *req = static_cast<DoorOpenReq*>(msg);
			handleDoorRequest(id, req->carID);

			DoorOpenCfm * cfm = new DoorOpenCfm;
			cfm->result = 0;
			req->mq->send(DOOR_OUT_OPEN_CFM, cfm);
			break;
		}
		case CAR_INSIDE:
		{
			CarInside *msgCast = static_cast<CarInside*>(msg);
			cout << "Car #" << msgCast->id << " comes inside." << endl;
			handleDoorRequest(id);
			break;
		}
		case CAR_OUTSIDE:
		{
			CarOutside *msgCast = static_cast<CarOutside*>(msg);
			cout << "Car #" << msgCast->id << " drives out." << endl;
			handleDoorRequest(id);
			break;
		}
	}
}

void * entryGuardThread(void *data) {
	MsgQueue *mq = static_cast<MsgQueue*>(data);
	queue<Message*> waitingReqs;
	GuardState state = ST_READY;

	for (;;) {
		db("For loop");
		unsigned long id;
		Message *msg;

		if (state == ST_READY) {
			if (waitingReqs.empty()) {
				msg = mq->receive(id);
				dispatcher(msg, id);
				delete msg;
			}
			else {
				msg = waitingReqs.front();
				waitingReqs.pop();
				dispatcher(msg, DOOR_IN_OPEN_REQ);
				delete msg;
			}
			state = ST_BUSY;
		}
		else {
			// ST_BUSY
			msg = mq->receive(id);

			if (id == CAR_INSIDE) {
				dispatcher(msg, id);
				delete msg;
				state = ST_READY;
			}
			else {
				// ID DOOR_IN_OPEN_REQ
				waitingReqs.push(msg);
			}
		}
	}
}

void * carThread(void *data) {
	// Convert void * data to CarData struct.
	CarData *cardata = static_cast<CarData*>(data);
	// Extract the mq pointers from the CarData struct
	MsgQueue *entryMQ = cardata->entryMQ;
	MsgQueue *exitMQ = cardata->exitMQ;
	// ID of this car thread
	int carID = cardata->ID;
	// A queue specific for this car (for the door open confirm)
	MsgQueue carQ(1);
	// ID returned by the MsgQueue receive method
	unsigned long id;
	// Message pointer to hold the pointer to the messages received from carQ.
	Message *msg;

	for (;;) {
		{
			/* Entry */
			DoorOpenReq * req = new DoorOpenReq;
			req->mq = &carQ;
			req->carID = carID;
			cout << "Car #" << carID << " wants to get in!" << endl;
			entryMQ->send(DOOR_IN_OPEN_REQ, req);

			// Receive the door open confirm and send it to dispatch.
			msg = carQ.receive(id);
			dispatcher(msg, id, carID);
			delete msg;
		}
		
		/* Notify that car is inside */
		CarInside *answer = new CarInside;
		answer->id = carID;
		entryMQ->send(CAR_INSIDE, answer);

		/* Sleep while parked */
		int waitTime = rand() % 10;
		cout << "Car #" << carID << " parked for " << waitTime << " seconds." << endl;
		sleep(waitTime);

		{
			/* Exit */
			DoorOpenReq * req = new DoorOpenReq;
			req->mq = &carQ;
			req->carID = carID;
			cout << "Car #" << carID << " wants to get out!" << endl;
			exitMQ->send(DOOR_OUT_OPEN_REQ, req);

			// Receive the door open confirm and send it to dispatch.
			msg = carQ.receive(id);
			dispatcher(msg, id, carID);
			delete msg;

			// Drive out and notify exitGuard.
			CarOutside *answer = new CarOutside;
			answer->id = carID;
			exitMQ->send(CAR_OUTSIDE, answer);
		}

		// Wait minimum 2 seconds before car tries to get in again.
		sleep(rand() % 5 + 2);
	}

	delete cardata;
}

void * exitGuardThread(void *data) {
	MsgQueue *mq = static_cast<MsgQueue*>(data);
	queue<Message*> waitingReqs;
	GuardState state = ST_READY;

	for (;;) {
		db("For loop");
		unsigned long id;
		Message *msg;

		if (state == ST_READY) {
			if (waitingReqs.empty()) {
				msg = mq->receive(id);
				dispatcher(msg, id);
				delete msg;
			}
			else {
				msg = waitingReqs.front();
				waitingReqs.pop();
				dispatcher(msg, DOOR_OUT_OPEN_REQ);
				delete msg;
			}
			state = ST_BUSY;
		}
		else {
			// ST_BUSY
			msg = mq->receive(id);

			if (id == CAR_OUTSIDE) {
				dispatcher(msg, id);
				delete msg;
				state = ST_READY;
			}
			else {
				// ID DOOR_OUT_OPEN_REQ
				waitingReqs.push(msg);
			}
		}
	}
}

int main() {
	srand(time(NULL));

	int carAmount = 200;

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
