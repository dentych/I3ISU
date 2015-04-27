#ifndef CARHANDLERS_HPP_
#define CARHANDLERS_HPP_

#include <iostream>
#include <unistd.h>
#include "../../Exercise1/Message.h"
#include "../../Exercise1/MsgQueue.h"
#include "../inc/MessageTypes.h"

using namespace std;

void carGenerateInOpenReq(MsgQueue *carQueue, MsgQueue *entryQueue, int id) {
	DoorOpenReq *req = new DoorOpenReq;
	req->mq = carQueue;
	req->carID = id;

	entryQueue->send(DOOR_IN_OPEN_REQ, req);
}

void carGenerateOutOpenReq(MsgQueue *carQueue, MsgQueue *exitQueue, int id) {
	DoorOpenReq *req = new DoorOpenReq;
	req->mq = carQueue;
	req->carID = id;

	exitQueue->send(DOOR_OUT_OPEN_REQ, req);
}

void carGenerateCarInsideMsg(MsgQueue *entryQueue, int carID) {
	CarInside *msg = new CarInside;
	msg->id = carID;

	entryQueue->send(CAR_INSIDE, msg);
}

void carGenerateCarOutsideMsg(MsgQueue *entryQueue, int carID) {
	CarOutside *msg = new CarOutside;
	msg->id = carID;

	entryQueue->send(CAR_OUTSIDE, msg);
}

void handleCarParked(int carID) {
	int num = rand() % 10;
	cout << "Car #" << carID << " parked for " << num << " seconds." << endl;
	sleep(num);
}

void handleCarOutsideTimeout() {
	int num = rand() % 10;
	sleep(num);
}

void carHandleMsg(unsigned long id, Message *msg, CarData *cardata) {
	switch(id) {
		case DOOR_IN_OPEN_CFM:
		{
			DoorOpenCfm *cfm = static_cast<DoorOpenCfm*>(msg);
			cout << "Car #" << cardata->ID << " drives into parking lot." << endl;
			carGenerateCarInsideMsg(cardata->entryMQ, cardata->ID);
			handleCarParked(cardata->ID);
			carGenerateOutOpenReq(&cardata->carQ, cardata->exitMQ, cardata->ID);
			break;
		}
		case DOOR_OUT_OPEN_CFM:
		{
			DoorOpenCfm *cfm = static_cast<DoorOpenCfm*>(msg);
			cout << "Car #" << cardata->ID << " drives out of parking lot." << endl;
			carGenerateCarOutsideMsg(cardata->exitMQ, cardata->ID);
			handleCarOutsideTimeout();
			carGenerateInOpenReq(&cardata->carQ, cardata->entryMQ, cardata->ID);
		}
	}
}

#endif
