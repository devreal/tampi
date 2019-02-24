/*
	This file is part of Task-Aware MPI and is licensed under the terms contained in the COPYING and COPYING.LESSER files.
	
	Copyright (C) 2015-2018 Barcelona Supercomputing Center (BSC)
*/

#ifndef SPIN_MUTEX_HPP
#define SPIN_MUTEX_HPP

#include "util/Utils.hpp"

#include <atomic>

namespace util {

template <size_t Size = MAX_SYSTEM_CPUS>
class SpinLock {
private:
	alignas(CACHELINE_SIZE) Padded<std::atomic<size_t> > _buffer[Size] = {};
	alignas(CACHELINE_SIZE) std::atomic<size_t> _head;
	alignas(CACHELINE_SIZE) size_t _next;
	
public:
	SpinLock()
		: _head(0),
		_next(0)
	{}
	
	inline void lock()
	{
		const size_t head = _head.fetch_add(1, std::memory_order_relaxed);
		const size_t idx = head % Size;
		while (_buffer[idx].load(std::memory_order_acquire) != head) {
			int spinsLeft = SPIN_LOCK_READS;
			size_t value;
			do {
				value = _buffer[idx].load(std::memory_order_relaxed);
				spinsLeft--;
			} while (value != head && (spinsLeft > 0));
		}
	}
	
	inline bool try_lock()
	{
		size_t head = _head.load(std::memory_order_relaxed);
		const size_t idx = head % Size;
		if (_buffer[idx].load(std::memory_order_relaxed) != head)
			return false;
		
		return std::atomic_compare_exchange_weak_explicit(
					&_head, &head, head + 1,
					std::memory_order_acquire,
					std::memory_order_relaxed);
	}
	
	inline void unlock()
	{
		const size_t idx = ++_next % Size;
		_buffer[idx].store(_next, std::memory_order_release);
	}
};

}

#endif // SPIN_MUTEX_HPP

