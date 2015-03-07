#ifndef SCOPEDLOCKER_H_
#define SCOPEDLOCKER_H_

#include <pthread.h>

class ScopedLocker {
public:
	ScopedLocker(pthread_mutex_t * mut) {
		this->mut = mut;
		pthread_mutex_lock(this->mut);
	}

	~ScopedLocker() {
		pthread_mutex_unlock(this->mut);
	}
private:
	pthread_mutex_t * mut;
};

#endif
