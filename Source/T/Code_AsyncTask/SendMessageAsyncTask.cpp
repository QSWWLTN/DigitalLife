#include "SendMessageAsyncTask.h"
#include "Kismet/KismetSystemLibrary.h"

#include "../Code_Public/GameInstance/ClientGameInstance.h"

DEFINE_LOG_CATEGORY(SendMessageTaskLog);

FSendMessageAsyncTask::~FSendMessageAsyncTask() {
	
}

void FSendMessageAsyncTask::DoWork() {
	UClientGameInstance* T = Cast<UClientGameInstance>(Owner);
	while (!bStop) {
		FPlatformProcess::Sleep(0.5);
		if (T->IsValidLowLevel() || Socket == nullptr || T->StopSocket) {
			break;
		}

		if (T->ClientBuffData.Num() == 0) {
			continue;
		}

		BuffSize = 0;
#if NO_PROTOBUF
		if (T->BuffDataSize > 3) {
			if (T->ClientBuffData[T->BuffDataSize - 1] == '!' && T->ClientBuffData[T->BuffDataSize - 2] == '?') {

				Buff = T->ClientBuffData;
				BuffSize = Buff.Num();
			}
		}
#else
		if (T->BuffDataSize > 3) {
			if (T->ClientBuffData[T->BuffDataSize - 1] == '!' && T->ClientBuffData[T->BuffDataSize - 2] == '?') {

				T->ClientBuffData.RemoveAt(T->ClientBuffData.Num() - 1);
				T->ClientBuffData.RemoveAt(T->ClientBuffData.Num() - 1);

				Buff = T->ClientBuffData;
				BuffSize = Buff.Num();
			}
		}
#endif
		if (BuffSize != 0) {
			int32 NowSize;
			if (!Socket->Send(Buff.GetData(), BuffSize, NowSize)) {
				AsyncTask(ENamedThreads::GameThread, [=] {
					UE_LOG(SendMessageTaskLog, Error, TEXT("发送消息失败"));
					});
			}
			AsyncTask(ENamedThreads::GameThread, [=] {
				UE_LOG(SendMessageTaskLog, Error, TEXT("发送数据量：%d"), NowSize);
				GEngine->AddOnScreenDebugMessage(-1, 1000.f, FColor::Red, FString(TEXT("发送数据量：")) + FString::FromInt(NowSize));

				T->CloseBuff();
			});
		}
	}
}