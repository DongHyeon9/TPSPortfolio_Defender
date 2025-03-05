#pragma once

#include "Actor/DA_Character.h"
#include "Interface/DI_ObjectPoolingActor.h"
#include "DA_Monster.generated.h"

class ADA_AIController;
class UDW_MonsterHP;

class UWidgetComponent;
class ALevelSequenceActor;

struct FStreamableHandle;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeActivateDelegate, ADA_Monster*, _Monster);

//프로젝트에서 사용되는 몬스터의 최상위 클래스
UCLASS(Blueprintable)
class DEFENDER_API ADA_Monster : 
	public ADA_Character,
	public IDI_ObjectPoolingActor
{
	GENERATED_BODY()
public:
	//Delegate called when a monster is deactivated
	UPROPERTY(BlueprintAssignable)
	FOnDeActivateDelegate onDeActivate;

	//Delegate called when all of the character's assets are loaded
	UPROPERTY(BlueprintAssignable)
	FOnAssetLoadCompletedDelegate onAssetLoadCompleted;

protected:

private:
	//Location where the camera will point when locking on
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "MonsterDefaultConfig", meta = (AllowPrivateAccess = true))
	USceneComponent* lockOnArea;

	//Components that equip monster weapons
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "MonsterDefaultConfig", meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* weaponComp;

	//Widget component that displays monster HP
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "MonsterDefaultConfig", meta = (AllowPrivateAccess = true))
	UWidgetComponent* hpWidgetComp;

	//AI controller currently responsible for controlling monsters
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "MonsterDefaultConfig", meta = (AllowPrivateAccess = true))
	ADA_AIController* monsterAIController;

	//type of monster
	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "Monster Setting", meta = (AllowPrivateAccess = true))
	EMONSTER_TYPE monsterType;

	//Data table containing types of monsters
	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "Monster Setting", meta = (AllowPrivateAccess = true))
	UDataTable* monsterDataTable;

	//Time taken from monster death to deactivation
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, category = "Monster Setting", meta = (AllowPrivateAccess = true))
	float deactivateDelay;

	//Check whether the monster is summoned to the field
	UPROPERTY(EditAnywhere, BlueprintReadWrite , category = "Monster Setting", meta = (AllowPrivateAccess = true))
	bool bIsSpawned;

	//Cinematics that monsters have
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, category = "Monster Setting", meta = (AllowPrivateAccess = true))
	TArray<ALevelSequenceActor*> cinematics;

	//Index of the cinematic to be displayed as soon as it starts
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Monster Setting", meta = (AllowPrivateAccess = true))
	int32 beginCinematicIndex;

	bool							bIsActive;						//캐릭터의 활성 상태를 나타낸다
	TSharedPtr<FStreamableHandle>	assetStreamingHandle;			//에셋들을 불러올 핸들
	TSharedPtr<FStreamableHandle>	weaponStreamingHandle;			//무기를 불러올 핸들을 저장
	TArray<FSoftObjectPath>			monsterAssetsToLoad;			//불러올 몬스터 에셋의 경로들을 저장
	FDFMonsterData*					monsterData;					//데이터 테이블에서 불러운 몬스터의 정보
	FTimerHandle					deactivateTimerHandle;			//몬스터 비활성화 타이머핸들

	UPROPERTY()
	UDW_MonsterHP*					monsterWidget;					//몬스터의 HP를 나타내는 위젯

public:
	ADA_Monster();

	/*
	* Initialize the monster
	* @param[in] _MonsterType type of monster
	* @param[in] _NewLevel monster level
	* @param[in] _DeactivateDelay Time taken to deactivate a monster after it dies
	*/
	UFUNCTION(BlueprintCallable, category = "Monster")
	void InitializeMonster(EMONSTER_TYPE _MonsterType, int32 _NewLevel, float _DeactivateDelay = 1.f);
	/*
	* Determines the active state of the character's collision
	* @param[in] _bIsActive When true, visibility is enabled
	*/
	virtual void MeshActive_Implementation(bool _bIsActive)override final;
	/*
	* Set the visibility the character's mesh
	* @param[in] _bIsActive When true, enable collision
	*/
	virtual void CollisionActive_Implementation(bool _bIsActive)override final;
	/*
	* Set the variables needed to change the activation state.
	* @param[in] _bIsActive If true, activate it
	*/
	virtual void ActiveSetting_Implementation(bool _bIsActive)override;
	/*
	* Checks if this actor is currently active
	* @return returns bool
	*/
	FORCEINLINE virtual bool IsActive()const { return bIsActive; }
	/*
	* Returns the targeted location
	* @return return the FVector
	*/
	FORCEINLINE FVector GetLockOnArea()const { return lockOnArea->GetComponentLocation(); }
	/*
	* Get the monster's current data table
	* @return Return the FDFMonsterData*
	*/
	FORCEINLINE FDFMonsterData* GetMonsterData()const { return monsterData; }
	/*
	* Play the cinematic
	* @param[in] _Index Index of the cinematic to play
	*/
	UFUNCTION(BlueprintCallable, category = "Cinematic")
	void PlayCinematic(int32 _Index);
	/*
	* This function is called after the cinematic has finished playing.
	*/
	UFUNCTION(BlueprintCallable, category = "Cinematic")
	void OnEndCinematic();

protected:
	virtual void PossessedBy(AController* _NewController);
	virtual void BeginPlay()override;

private:
	void OnAssetLoadCompleted();

	UFUNCTION()
	void OnChangeHP(float _MaxHP, float _CurrentHP);

	UFUNCTION()
	void HitStart();

	UFUNCTION()
	void HitEnd();

	UFUNCTION()
	void StunEnd();

	UFUNCTION()
	void DeadEnd();

	UFUNCTION()
	void OnStateChange(ECHARACTER_STATE _NewState);
};
