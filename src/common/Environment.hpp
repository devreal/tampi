/*
	This file is part of Task-Aware MPI and is licensed under the terms contained in the COPYING and COPYING.LESSER files.
	
	Copyright (C) 2015-2019 Barcelona Supercomputing Center (BSC)
*/

#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

#include <atomic>

#include <mpi.h>

#include "TaskContext.hpp"
#include "TaskingModel.hpp"
#include "TicketManager.hpp"

template <typename Lang>
class Environment {
private:
	static std::atomic<bool> _blockingEnabled;
	static std::atomic<bool> _nonBlockingEnabled;
	
	static int poll(__attribute__((unused)) void *data)
	{
		getTicketManager().checkRequests();
		return 0;
	}
	
public:
	Environment() = delete;
	
	Environment(const Environment &) = delete;
	
	const Environment& operator= (const Environment &) = delete;
	
	static inline bool isBlockingEnabled()
	{
		return _blockingEnabled.load();
	}

	static inline bool isNonBlockingEnabled()
	{
		return _nonBlockingEnabled.load();
	}
	
	static inline bool isAnyModeEnabled()
	{
		return isBlockingEnabled() || isNonBlockingEnabled();
	}
	
	static void initialize(bool blockingMode, bool nonBlockingMode)
	{
#if !HAVE_BLOCKING_MODE
		blockingMode = false;
#endif
#if !HAVE_NONBLOCKING_MODE
		nonBlockingMode = false;
#endif
		if (!isAnyModeEnabled()) {
			enableBlocking(blockingMode);
			enableNonBlocking(nonBlockingMode);
			
			Lang::initialize();
			TaskingModel::initialize(blockingMode, nonBlockingMode);
			
			if (nonBlockingMode) {
				TaskingModel::notifyTaskEventCounterAPI();
			}
			
			if (blockingMode || nonBlockingMode) {
				TaskingModel::registerPollingService("TAMPI", Environment::poll);
			}
		}
	}
	
	static void finalize()
	{
		if (isAnyModeEnabled()) {
			TaskingModel::unregisterPollingService("TAMPI", Environment::poll);
			enableBlocking(false);
			enableNonBlocking(false);
		}
	}
	
	static inline TicketManager<Lang> &getTicketManager()
	{
		static TicketManager<Lang> _ticketManager;
		return _ticketManager;
	}
	
private:
	static inline void enableNonBlocking(bool enable)
	{
		_nonBlockingEnabled.store(enable);
	}
	
	static inline void enableBlocking(bool enable)
	{
		_blockingEnabled.store(enable);
	}
};

#endif // ENVIRONMENT_HPP
