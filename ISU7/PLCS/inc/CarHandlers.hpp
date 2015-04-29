#ifndef CARHANDLERS_HPP_
#define CARHANDLERS_HPP_

#include <iostream>
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

void carGenerateCarOutsideMsg(MsgQueue *exitQueue, int carID) {
	CarOutside *msg = new CarOutside;
	msg->id = carID;

	exitQueue->send(CAR_OUTSIDE, msg);
}

void handleCarParked(int carID) {
	int num = rand() % 10;
	cout << "Car #" << carID << " parked for " << num << " seconds." << endl;
	osapi::sleep(num*1000);
}

void handleCarOutsideTimeout() {
	int num = rand() % 10;
	osapi::sleep(num*1000);
}

void carHandleMsg(unsigned long id, Message *msg, CarData *cardata) {
	switch(id) {
		case DOOR_IN_OPEN_CFM:
		{
			//DoorOpenCfm *cfm = static_cast<DoorOpenCfm*>(msg);
			cout << "Car #" << cardata->ID << " drives into parking lot." << endl;
			carGenerateCarInsideMsg(cardata->entryMQ, cardata->ID);
			handleCarParked(cardata->ID);
			carGenerateOutOpenReq(&cardata->carQ, cardata->exitMQ, cardata->ID);
			break;
		}
		case DOOR_OUT_OPEN_CFM:
		{
			//DoorOpenCfm *cfm = static_cast<DoorOpenCfm*>(msg);
			cout << "Car #" << cardata->ID << " drives out of parking lot." << endl;
			carGenerateCarOutsideMsg(cardata->exitMQ, cardata->ID);
			handleCarOutsideTimeout();
			carGenerateInOpenReq(&cardata->carQ, cardata->entryMQ, cardata->ID);
		}
	}
}

#endif
