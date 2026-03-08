#ifndef ENGINE_JOBS_HPP
#define ENGINE_JOBS_HPP

#include "Defines.hpp"
#include <functional>
#include <condition_variable>

namespace jobs {
	/**
	* Create the internal resources such as worker threads, etc. Call it once when initializing the application.
	*/
	void Initialize();
	/**
	* Add a job to execute asynchronously. Any idle thread will execute this job.
	* @param job
	*/
	GAPI void Execute(const std::function<void()>& job);
	/**
	* Check if any threads are working currently or not
	*/
	bool IsBusy();
	/**
	* Wait until all threads become idle
	*/
	GAPI void Wait();
};
#endif //ENGINE_JOBS_HPP
