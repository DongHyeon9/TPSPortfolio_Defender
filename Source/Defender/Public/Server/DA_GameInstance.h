#pragma once

#include "../Defender.h"
#include "Engine/GameInstance.h"
#include "DA_GameInstance.generated.h"

//������Ʈ���� ���Ǵ� �����ν��Ͻ�
UCLASS()
class DEFENDER_API UDA_GameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

protected:

private:
	//Save data from previous level in json format
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,category="PrevLevelData",meta=(AllowPrivateAccess=true))
	FString prevLevelDataJson;

	TSet<FTimerHandle*>		timerHandleSet;			//�����̸� �ɾ���� �ڵ��� �����ϴ� ����

public:
	UDA_GameInstance();
	/*
	* Get data from the previous level in json format
	* @return return the const FString&
	*/
	FORCEINLINE const FString& GetPrevLevelData()const { return prevLevelDataJson; }
	/*
	* Save data from previous level
	* @param[in] _CurrentLevelData json data from previous level
	*/
	void SaveCurrentLevelData(const FString& _CurrentLevelData);
	/*
	* Reset the timer handle.
	*/
	void ResetTimer();
	/*
	* Register the timer handle
	* @param[in] _TimerHandle Timer handle to register
	*/
	void RegistTimerHandle(FTimerHandle* _TimerHandle);

protected:

private:

};
