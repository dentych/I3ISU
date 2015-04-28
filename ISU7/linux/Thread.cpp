#include <unistd.h>
#include <errno.h>
#include <iostream>
#include <osapi/Thread.hpp>


namespace osapi
{
	Thread::Thread(ThreadFunctor* tf,
			Thread::ThreadPriority priority, 
			const std::string& name,
			bool autoStart)
		: tf_(tf), priority_(priority), name_(name), attached_(true)
	{
		if(autoStart)
			start();
	}


	Thread::~Thread()
	{
		detach();
	}

	void Thread::start()
	{

		if(getuid() == 0) // Check to see if we are root
		{
			/* Steps to go through */
			pthread_attr_t  attr;
			if(pthread_attr_init(&attr) != 0) throw ThreadError();										// Init attr
			sched_param sched_p;
			if(pthread_attr_setschedpolicy(&attr, SCHED_RR) != 0) throw ThreadError();					// Set RR scheduling (RT, timesliced)
			if(pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED) != 0) throw ThreadError();	// Create thread with explicit (non-inherited) scheduling - setting priority will not work otherwise!
			sched_p.sched_priority = static_cast<int>(priority_);							// Set priority
			if(pthread_attr_setschedparam(&attr, &sched_p) != 0) throw ThreadError();					// Use the priority        

			/* Missing thread creation */
			pthread_attr_destroy(&attr);
		}
		else
		{
			/* Missing thread creation */
		}
		// If ok no exception was thrown thus we have a valid thread and its attached!
		attached_ = true;
	}


	void Thread::setPriority(Thread::ThreadPriority priority)
	{
		if(!attached_) throw ThreadError();

		/* Missing contents...*/
	}




	void Thread::join()
	{
		if(!attached_) throw ThreadError();
		tf_->threadDone_.wait();
	}

	void Thread::detach()
	{
		if(!attached_) throw ThreadError();
		attached_ = false;
		tf_ = NULL;
		pthread_detach(threadId_);
	}

}
