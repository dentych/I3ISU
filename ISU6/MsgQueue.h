#include <iostream>
#include <deque>
#include <pthread.h>
#include "Message.h"

struct Item {
	unsigned long id;
	Message * msg;
};

class MsgQueue {
public:
	MsgQueue(unsigned long maxSize);
	void send(unsigned long id, Message * msg = NULL);
	Message * receive(unsigned long & id);
	~MsgQueue();
private:
	std::deque<Item> mqitems;
	unsigned long maxSize;
	pthread_mutex_t mqMut = PTHREAD_MUTEX_INITIALIZER;
	pthread_cond_t sender = PTHREAD_COND_INITIALIZER;
	pthread_cond_t receiver = PTHREAD_COND_INITIALIZER;
};
