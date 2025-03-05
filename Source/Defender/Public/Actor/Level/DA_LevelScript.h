#pragma once

#include "../Defender.h"
#include "Engine/LevelScriptActor.h"
#include "DA_LevelScript.generated.h"

class ADA_Monster;

//각 스테이지의 규칙을 관리하는 레벨스크립트액터
UCLASS()
class DEFENDER_API ADA_LevelScript : public ALevelScriptActor
{
	GENERATED_BODY()
public:

protected:

private:
	//Time to activate the store at the start of the level
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "Level Setting", meta = (AllowPrivateAccess = true))
	float shopTimer;

	//Stage time limit
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "Level Setting", meta = (AllowPrivateAccess = true))
	float stageTimer;

	//Free time until the next level opens
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "Level Setting", meta = (AllowPrivateAccess = true))
	float levelOpenTimer;

	//Name of next stage
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "Level Setting", meta = (AllowPrivateAccess = true))
	FName nextLevelName;

	//Whether to start the stage automatically.	If true, it is called in the order of store->stage start->stage end.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "Level Setting", meta = (AllowPrivateAccess = true))
	bool bIsAutoStart;

	//The goal of this stage. When this monster dies, stage clear is called.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, category = "Level Setting", meta = (AllowPrivateAccess = true))
	ADA_Monster* missionGoal;

	//The class of the monster to be used in the current stage
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "Monster Pool", meta = (AllowPrivateAccess = true))
	TSubclassOf<ADA_Monster> monsterClass;

	//Maximum summon amount of monsters
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "Monster Pool", meta = (AllowPrivateAccess = true))
	int32 maxMonsterCount;

	//How long does it take for a monster to be summoned?
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "Monster Pool", meta = (AllowPrivateAccess = true))
	float spawnDelay;

	//Range where monsters are summoned
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "Monster Pool", meta = (AllowPrivateAccess = true))
	FVector2D spawnRange;

	//Relative location where monsters will be summoned
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "Monster Pool", meta = (AllowPrivateAccess = true))
	FVector relativeSpawnLocation;

	//Height to start line tracing for random summoning
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "Monster Pool", meta = (AllowPrivateAccess = true))
	float ceilingHeight;

	//Type of monster to be summoned
	//You can set the probability of summoning by duplicate type.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "Monster Data", meta = (AllowPrivateAccess = true))
	TArray<EMONSTER_TYPE> monsterSpawnType;

	//Minimum level of monster
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "Monster Data", meta = (AllowPrivateAccess = true, ClampMin = "1", ClampMax = "20"))
	int32 minMonsterLevel;

	//maximum level of monster
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "Monster Data", meta = (AllowPrivateAccess = true, ClampMin = "1", ClampMax = "20"))
	int32 maxMonsterLevel;

	TQueue<ADA_Monster*>			monsterPool;				//소환된 몬스터를 오브젝트 풀링으로 관리할 몬스터 큐
	FTimerHandle					monsterSpawnHandle;			//몬스터를 일정시간마다 소환하는 타이머핸들
	FTimerHandle					stageTimeHandle;			//스테이지의 제한시간을 확인하는 타이머핸들
	float							currentTime;				//현재 남은 스테이지 시간

public:
	ADA_LevelScript();

	/*
	* Start the stage according to the settings.
	*/
	UFUNCTION(BlueprintCallable, category = "Stage")
	void StageStart();

	/*
	* End the stage
	*/
	UFUNCTION(BlueprintCallable, category = "Stage")
	void StageEnd();

	/*
	* Start summoning monsters
	*/
	UFUNCTION(BlueprintCallable, category = "Stage")
	void StartMonsterSpawn();

	/*
	* Ends summoning of monsters
	*/
	UFUNCTION(BlueprintCallable, category = "Stage")
	void EndMonsterSpawn();

	/*
	* Summon monsters to random locations
	* @param[in] _CanExpand If there are not enough monsters in the object pool, additional monsters are summoned.
	* @return Success or failure of monster summoning
	*/
	UFUNCTION(BlueprintCallable, category = "Stage")
	bool SpawnMonsterRandomLocation(bool _CanExpand = false);

	/*
	* Deletes all summoned monsters
	*/
	UFUNCTION(BlueprintCallable, category = "Stage")
	void MonsterAllDestroy();

	/*
	* Returns the monster in the object pool
	* @return Returns the ADA_Monster*
	*/
	UFUNCTION(BlueprintCallable, category = "Stage")
	ADA_Monster* GetMonster();

	/*
	* Register a player in your level.
	* Registered players will start at the store level.
	* @param[in] _NewPlayer The new player's controller
	*/
	UFUNCTION(BlueprintCallable, category = "Stage")
	void AdmissionPlayer(ADAPC_InGame* _NewPlayer);

	/*
	* Whether the stage starts automatically
	* @return Returns bool
	*/
	FORCEINLINE bool IsAutoStart()const { return bIsAutoStart; }

protected:
	virtual void BeginPlay()override;

private:
	UFUNCTION()
	void MonsterDeActive(ADA_Monster* _Monster);

	UFUNCTION()
	void LevelStart();

	UFUNCTION()
	void PlayerStateChange(ECHARACTER_STATE _NewState);

	UFUNCTION()
	void GoalAssetLoadCompleted();

	void ExpandPool();
	void SpawnMonster();
};
