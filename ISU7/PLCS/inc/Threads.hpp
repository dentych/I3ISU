#ifndef THREADS_HPP_
#define THREADS_HPP_

#include "MessageTypes.h"
#include "EntryHandlers.hpp"
#include "ExitHandlers.hpp"
#include "CarHandlers.hpp"
#include <osapi/Thread.hpp>

class EntryGuardThread : public ThreadFunctor {
	public:
		EntryGuardThread(MsgQueue *entryQueue)
			: mq(entryQueue), state(ST_READY) {}

		virtual void run() {
			for (;;) {
				unsigned long id;
				Message *msg = mq->receive(id);
				entryHandleMsg(msg, id, state, waitingReqs);
				delete msg;
			}

			delete mq;
		}

		virtual ~EntryGuardThread() {}

	private:
		MsgQueue *mq;
		GuardState state;
		queue<DoorOpenReq*> waitingReqs;
};

class ExitGuardThread : public ThreadFunctor {
	public:
		ExitGuardThread(MsgQueue *exitQueue)
			: mq(exitQueue), state(ST_READY) {}

		virtual void run() {
			for (;;) {
				unsigned long id;
				Message *msg = mq->receive(id);
				exitHandleMsg(msg, id, state, waitingReqs);
				delete msg;
			}

			delete mq;
		}

		virtual ~ExitGuardThread() {}

	private:
		MsgQueue *mq;
		GuardState state;
		queue<DoorOpenReq*> waitingReqs;
};

class CarThread : public ThreadFunctor {
	public:
		CarThread(CarData *data) : cardata(data) {}

		virtual void run() {
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

		virtual ~CarThread() {}

	private:
		CarData *cardata;
};

#endif
