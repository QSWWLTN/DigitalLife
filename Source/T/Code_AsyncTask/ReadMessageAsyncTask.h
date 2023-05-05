#pragma once

#include "CoreMinimal.h"
#include "Async/AsyncWork.h"
#include "Sockets.h"

#if !NO_PROTOBUF

#include "libProtobuf/include/google/protobuf/message.h"
#include "../Code_Struct/DigitalLife.pb.h"

#endif

#include "../Code_Public/TestFunctionLibrary.h"

DECLARE_LOG_CATEGORY_EXTERN(ReadMessageTaskLog, Log, All);

class FReadMessageAsyncTask : public FNonAbandonableTask {
public:
	FReadMessageAsyncTask(UObject* Owner, FSocket* Socket) :
		Owner(Owner),
		Socket(Socket)
	{

	};

	~FReadMessageAsyncTask();

	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(FReadMessageAsyncTask, STATGROUP_ThreadPoolAsyncTasks);
	}

public:
	void DoWork();

#if !PLATFORM_ANDROID
	const FString SaveFilePath = UTestFunctionLibrary::GetRootPath() / TEXT("Temp/Read.wav");
#else
	const FString SaveFilePath = UTestFunctionLibrary::GetRootPath() / TEXT("Content/Movies/Read.wav");
#endif

private:
	friend class FAutoDeleteAsyncTask<FReadMessageAsyncTask>;

private:
	UObject* Owner;
	FSocket* Socket;

	TArray<uint8> ReadBuff;
	int32 RecvSize = 0;

#if !NO_PROTOBUF
	const google::protobuf::Descriptor* BaseAskDes = google::protobuf::DescriptorPool::generated_pool()->FindMessageTypeByName("DigitalLifeNameSpace.C_BaseAsk");
#endif
};