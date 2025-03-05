#include "Server/DA_GameInstance.h"

UDA_GameInstance::UDA_GameInstance()
{
}

void UDA_GameInstance::SaveCurrentLevelData(const FString& _CurrentLevelData)
{
	DFLOG_N(Display);
	prevLevelDataJson = _CurrentLevelData;
}

void UDA_GameInstance::ResetTimer()
{
	//�����̰� �ɷ��ִ� Ÿ�̸� �ڵ��� �������ݴϴ�
	for (auto timerHandle : timerHandleSet)
		if (timerHandle->IsValid()) {
			GetWorld()->GetTimerManager().ClearTimer(*timerHandle);
			timerHandle->Invalidate();
		}
	timerHandleSet.Empty();
}

void UDA_GameInstance::RegistTimerHandle(FTimerHandle* _TimerHandle)
{
	timerHandleSet.Add(_TimerHandle);
}
