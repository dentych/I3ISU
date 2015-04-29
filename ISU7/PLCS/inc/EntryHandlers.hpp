#ifndef ENTRYHANDLERS_HPP_
#define ENTRYHANDLERS_HPP_

#include <iostream>
#include <queue>
#include "../inc/MessageTypes.h"

using namespace std;

void entryGenerateDoorOpenCfm(MsgQueue *mq) {
	DoorOpenCfm *response = new DoorOpenCfm;
	response->result = true;
	mq->send(DOOR_IN_OPEN_CFM, response);
}

void handleEntryDoorOpenReq(DoorOpenReq *req) {
	cout << "ENTRY DOOR opened for CAR #" << req->carID << "." << endl;
	entryGenerateDoorOpenCfm(req->mq);
}

void handleEntryWaitingReqs(queue<DoorOpenReq*> &guardQ, GuardState &state) {
	if (!guardQ.empty()) {
		DoorOpenReq *req = guardQ.front();
		guardQ.pop();
		handleEntryDoorOpenReq(req);
	}
	else {
		state = ST_READY;
	}
}

void handleEntryReady(Message *msg, unsigned long id) {
	switch(id) {
		case DOOR_IN_OPEN_REQ:
		{
			DoorOpenReq *req = static_cast<DoorOpenReq*>(msg);
			handleEntryDoorOpenReq(req);
			break;
		}
		default:
			cout << "##ERROR: " << 
			"Entry guard handler handleEntryReady received a wrong Message: " << id << endl;
	}
}

void handleEntryBusy(Message *msg, unsigned long id, GuardState &state, queue<DoorOpenReq*> &guardQ) {
	switch(id) {
		case DOOR_IN_OPEN_REQ:
		{
			DoorOpenReq *req = static_cast<DoorOpenReq*>(msg);
			guardQ.push(req);
			break;
		}
		case CAR_INSIDE:
		{
			cout << "ENTRY DOOR closed." << endl;
			//CarInside *req = static_cast<CarInside*>(msg);
			handleEntryWaitingReqs(guardQ, state);
			break;
		}
	}
}

void entryHandleMsg(Message *msg, unsigned long id,
	GuardState &state, queue<DoorOpenReq*> &guardQ) {
	//cout << "ENTRY: MSG: " << id << endl;
	switch (state) {
		case ST_READY:
			state = ST_BUSY;
			handleEntryReady(msg, id);
			break;
		case ST_BUSY:
			handleEntryBusy(msg, id, state, guardQ);
			break;
	}
}

#endif
