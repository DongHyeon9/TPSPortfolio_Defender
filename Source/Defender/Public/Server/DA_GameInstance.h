#pragma once

#include "../Defender.h"
#include "Engine/GameInstance.h"
#include "DA_GameInstance.generated.h"

//프로젝트에서 사용되는 게임인스턴스
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

	TSet<FTimerHandle*>		timerHandleSet;			//딜레이를 걸어놓은 핸들을 저장하는 집합

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
