#ifndef EXITHANDLERS_HPP_
#define EXITHANDLERS_HPP_

#include <iostream>
#include <queue>
#include "../inc/MessageTypes.h"

using namespace std;

void exitGenerateDoorOpenCfm(MsgQueue *mq) {
	DoorOpenCfm *response = new DoorOpenCfm;
	response->result = true;
	mq->send(DOOR_OUT_OPEN_CFM, response);
}

void handleExitDoorOpenReq(DoorOpenReq *req) {
	cout << "EXIT DOOR opened for CAR #" << req->carID << "." << endl;
	exitGenerateDoorOpenCfm(req->mq);
}

void handleExitWaitingReqs(queue<DoorOpenReq*> &guardQ, GuardState &state) {
	if (!guardQ.empty()) {
		DoorOpenReq *req = guardQ.front();
		guardQ.pop();
		handleExitDoorOpenReq(req);
	}
	else {
		state = ST_READY;
	}
}

void handleExitReady(Message *msg, unsigned long id) {
	switch(id) {
		case DOOR_OUT_OPEN_REQ:
		{
			DoorOpenReq *req = static_cast<DoorOpenReq*>(msg);
			handleExitDoorOpenReq(req);
			break;
		}
		default:
			cout << "##ERROR: " << 
			"Exit guard handler handleExitReady received a wrong Message: " << id << endl;
	}
}

void handleExitBusy(Message *msg, unsigned long id, GuardState &state, queue<DoorOpenReq*> &guardQ) {
	switch(id) {
		case DOOR_OUT_OPEN_REQ:
		{
			DoorOpenReq *req = static_cast<DoorOpenReq*>(msg);
			guardQ.push(req);
			break;
		}
		case CAR_OUTSIDE:
		{
			cout << "EXIT DOOR closed." << endl;
			//CarInside *req = static_cast<CarInside*>(msg);
			handleExitWaitingReqs(guardQ, state);
			break;
		}
	}
}

void exitHandleMsg(Message *msg, unsigned long id,
	GuardState &state, queue<DoorOpenReq*> &guardQ) {
	//cout << "EXIT: MSG: " << id << endl;
	switch (state) {
		case ST_READY:
			state = ST_BUSY;
			handleExitReady(msg, id);
			break;
		case ST_BUSY:
			handleExitBusy(msg, id, state, guardQ);
			break;
	}
}

#endif
