#pragma once

#include "CoreMinimal.h"
#include "Async/AsyncWork.h"
#include "Sockets.h"

DECLARE_LOG_CATEGORY_EXTERN(SendMessageTaskLog, Log, All);

class FSendMessageAsyncTask : public FNonAbandonableTask {

public:
	FSendMessageAsyncTask(UObject* Owner, FSocket* Socket) :
		Owner(Owner),
		Socket(Socket)
	{

	};

	~FSendMessageAsyncTask();

	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(FSendMessageAsyncTask, STATGROUP_ThreadPoolAsyncTasks);
	}

public:
	void DoWork();

public:
	uint64 BuffSize = 0;
	TArray<uint8> Buff;

private:
	friend class FAutoDeleteAsyncTask<FSendMessageAsyncTask>;

private:
	UObject* Owner;
	FSocket* Socket;

	bool bStop = false;
};