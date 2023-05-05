#pragma once

#include "CoreMinimal.h"
#include "Async/AsyncWork.h"

DECLARE_LOG_CATEGORY_EXTERN(HookKeyboardTaskLog, Log, All);

class FHookKeyboardAsyncTask : public FNonAbandonableTask {
	
public:
	FHookKeyboardAsyncTask(UObject* Owner);
	~FHookKeyboardAsyncTask();

public:
	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(FReadMessageAsyncTask, STATGROUP_ThreadPoolAsyncTasks);
	}

	void DoWork();

private:
	UObject* Owner;
};