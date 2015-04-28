#ifndef OSAPI_LINUX_MUTEX_HPP
#define OSAPI_LINUX_MUTEX_HPP

#include <pthread.h>
#include <osapi/Conditional.hpp>

namespace osapi
{    
	class Mutex
	{
		public:
			Mutex();
			~Mutex();
			void lock();
			void unlock();
		private:
			pthread_mutex_t mut;

			friend class Conditional;
	};

}


#endif
