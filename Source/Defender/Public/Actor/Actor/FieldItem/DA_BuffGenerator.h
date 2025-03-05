#pragma once

#include "Actor/DA_Actor.h"
#include "DA_BuffGenerator.generated.h"

class ADA_Buff;

class UStaticMeshComponent;

//일정시간마다 버프를 생성하는 액터
UCLASS()
class DEFENDER_API ADA_BuffGenerator : public ADA_Actor
{
	GENERATED_BODY()
public:

protected:

private:
	//Basic root component for buff generator
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "Generator DefaultConfig", meta = (AllowPrivateAccess = true))
	USceneComponent* rootComp;

	//Visual mesh of buff components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "Generator DefaultConfig", meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* meshComp;

	//Type of buff the buff generator will create
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Generator Setting", meta = (AllowPrivateAccess = true))
	TSubclassOf<ADA_Buff> spawnBuffClass;

	//Relative location of the buff to be created
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Generator Setting", meta = (AllowPrivateAccess = true))
	FVector spawnRelativeLocation;

	//Time taken to respawn
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Generator Setting", meta = (AllowPrivateAccess = true))
	float spawnCoolDown;

	UPROPERTY()
	ADA_Buff*				spawnedBuff;				//소환된 버프

	FTimerHandle			generatorTimerHandle;		//버프 재활성화 타이머 핸들

public:
	ADA_BuffGenerator();

	/*
	* When called, a buff is created in accordance with the time.
	*/
	UFUNCTION(BlueprintCallable, category = "Buff Generator")
	void StartGenerator();

	/*
	* When called, the created buff is deleted and reset.
	* Please call before changing the buff type or when deactivating the buff generator.
	*/
	UFUNCTION(BlueprintCallable, category = "Buff Generator")
	void StopGenerator();

protected:
	virtual void BeginPlay()override;

private:
	void BuffActive();

	UFUNCTION()
	void BuffDeActive();

	UFUNCTION()
	void BuffDestroy(AActor* _Actor, EEndPlayReason::Type _EndPlayReason);


};
