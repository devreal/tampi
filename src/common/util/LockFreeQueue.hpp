/*
	This file is part of Task-Aware MPI and is licensed under the terms contained in the COPYING and COPYING.LESSER files.
	
	Copyright (C) 2015-2019 Barcelona Supercomputing Center (BSC)
*/

#ifndef LOCK_FREE_QUEUE_HPP
#define LOCK_FREE_QUEUE_HPP

#include <functional>
#include <mutex>

#include "SpinLock.hpp"

#include <boost/version.hpp>

// Support for BOOST 1.67
#if BOOST_VERSION == 106700
#include <boost/next_prior.hpp>
#endif

#include <boost/lockfree/spsc_queue.hpp>

namespace util {

template <typename T, size_t Size = 2048>
class LockFreeQueue {
private:
	typedef boost::lockfree::spsc_queue<T, boost::lockfree::capacity<Size> > queue_t;
	typedef std::function<void()> ProgressFunction;
	
	SpinLock _adderMutex;
	queue_t _queue;
	
public:
	LockFreeQueue() :
		_adderMutex(),
		_queue()
	{}
	
	inline void add(const T &element, ProgressFunction progressFunction)
	{
		std::lock_guard<SpinLock> guard(_adderMutex);
		
		int pushed = 0;
		do {
			pushed = _queue.push(element);
			if (pushed < 1) {
				progressFunction();
			}
		} while (pushed == 0);
	}
	
	inline void add(const T elements[], int count, ProgressFunction progressFunction)
	{
		assert(elements != nullptr);
		std::lock_guard<SpinLock> guard(_adderMutex);
		
		int pushed = 0;
		do {
			pushed += _queue.push(&elements[pushed], count - pushed);
			if (pushed < count) {
				progressFunction();
			}
		} while (pushed < count);
	}
	
	inline int retrieve(T elements[], int count)
	{
		assert(elements != nullptr);
		return _queue.pop(elements, count);
	}
};

} // namespace util

#endif // LOCK_FREE_QUEUE_HPP
