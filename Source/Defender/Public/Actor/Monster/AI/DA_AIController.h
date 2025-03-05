#pragma once

#include "../Defender.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Sight.h"
#include "DA_AIController.generated.h"

class ADA_Monster;
class ADA_Player;

class UAIPerceptionComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChangeTraceStateDelegate, EMONSTER_TRACE_STATE, _NewState);

//몬스터에 사용되는 컨트롤러
UCLASS()
class DEFENDER_API ADA_AIController : public AAIController
{
	GENERATED_BODY()
public:
	static const FName patrolPosKey;		//정찰할 위치의 키 이름
	static const FName targetKey;			//공격할 대상의 키 이름
	static const FName skillNameKey;		//다음 발동할 스킬의 키 이름
	static const FName canAttackKey;		//공격이 가능한지 판별해주는 키 이름

	//Delegate called when the trace status changes
	UPROPERTY(BlueprintAssignable)
	FOnChangeTraceStateDelegate onTraceStateChange;

protected:

private:
	//Components for monster detection
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "AI DefaultConfig", meta = (AllowPrivateAccess = true))
	UAIPerceptionComponent* aiPerceptionComp;

	//Detection that reacts when a monster receives damage
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "AI DefaultConfig", meta = (AllowPrivateAccess = true))
	UAISenseConfig_Damage* damageSense;

	//Detection that reacts when a monster sees it
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "AI DefaultConfig", meta = (AllowPrivateAccess = true))
	UAISenseConfig_Sight* sightSense;

	//Detection that the monster reacts when it hears a sound
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "AI DefaultConfig", meta = (AllowPrivateAccess = true))
	UAISenseConfig_Hearing* hearingSense;

	//The monster currently controlled by the controller
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "AI DefaultConfig", meta = (AllowPrivateAccess = true))
	ADA_Monster* controlledMonster;

	//Current tracing status
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, category = "Monster State", meta = (AllowPrivateAccess = true))
	EMONSTER_TRACE_STATE currentTraceState;

	//A map containing the range of skills that monsters can activate
	UPROPERTY(VisibleInstanceOnly, BluePrintReadOnly, category = "Monster Skill", meta = (AllowPrivateAccess = true))
	TMap<FString, float> skillRangeMap;

	//The player the monster will attack
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "Monster Target", meta = (AllowPrivateAccess = true))
	UObject* targetPlayer;

	ECHARACTER_STATE		currentCharacterState;		//현재 캐릭터의 상태
	FVector					patrolTemp;					//갈려고했던 정찰 위치

public:
	ADA_AIController();
	/*
	* Get AI perception components
	* @return Return the UAIPerceptionComponent*
	*/
	FORCEINLINE UAIPerceptionComponent* GetAIPerceptionComponent()const { return aiPerceptionComp; }
	/*
	* Get the ID of the sight sensor
	* @return Returns FAISenseID
	*/
	FORCEINLINE FAISenseID GetSightID()const { return UAISense::GetSenseID(UAISense_Sight::StaticClass()); }
	/*
	* Get the ID of the hearing sensor
	* @return Returns FAISenseID
	*/
	FORCEINLINE FAISenseID GetHearingID()const { return UAISense::GetSenseID(UAISense_Hearing::StaticClass()); }
	/*
	* Get the ID of the damage sensor
	* @return Returns FAISenseID
	*/
	FORCEINLINE FAISenseID GetDamageID()const { return UAISense::GetSenseID(UAISense_Damage::StaticClass());	}
	/*
	* Get the current tracing status
	* @return Returns EMONSTER_TRACE_STATE
	*/
	FORCEINLINE EMONSTER_TRACE_STATE GetTraceState()const { return currentTraceState; }
	/*
	* Get the player you are currently targeting
	* @return Return ADA_Player*
	*/
	FORCEINLINE ADA_Player* GetTargetPlayer()const { return Cast<ADA_Player>(targetPlayer); }
	/*
	* Get the monster you are controlling
	* @return Returns the ADA_Monster*
	*/
	FORCEINLINE ADA_Monster* GetControlledMonster()const { return controlledMonster; }
	/*
	* Set trace status
	* @param[in] _TraceState Trace state to set
	*/
	UFUNCTION(BlueprintCallable, category = "AI Controller")
	void SetTraceState(EMONSTER_TRACE_STATE _TraceState);
	/*
	* Initialize the AI ??controller.
	* Set the range for each skill
	* @param[in] _MonsterSkillData Monster's skill data to set the range
	*/
	UFUNCTION(BlueprintCallable, category = "AI Controller")
	void InitializeAIController(const TArray<FDFActiveSkillData>& _MonsterSkillData);
	/*
	* Re-enable AI.
	*/
	UFUNCTION(BlueprintCallable, category = "AI Controller")
	void RunAI();
	/*
	* Disable AI
	* @param[in] _Reason Why stop?
	*/
	UFUNCTION(BlueprintCallable, category = "AI Controller")
	void StopAI(const FString& _Reason);
	/*
	* Gets how much range a skill has
	* @param[in] _SkillName Skill name to get range from
	* @param[out] _OutRange Range returned
	* @return Returns false if there is no skill
	*/
	bool GetSkillRange(const FString& _SkillName, float& _OutRange);
	/*
	* A random skill name is taken from among the monster's skills.
	* @return Return the FString
	*/
	FString GetRandomSkillName();

protected:
	virtual void OnPossess(APawn* _InPawn)override;

private:
	UFUNCTION()
	void OnUpdatedPerception(AActor* _Actor, FAIStimulus _Stimulus);

	UFUNCTION()
	void OnChangeState(ECHARACTER_STATE _CurrentState);

	FORCEINLINE bool IsComparisonHigherState(EMONSTER_TRACE_STATE _CompareState) { return (int32)currentTraceState < (int32)_CompareState; }

};
