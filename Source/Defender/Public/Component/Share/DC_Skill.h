#pragma once

#include "Component/DC_PawnComponent.h"
#include "DC_Skill.generated.h"

class UDO_ActiveBase;
class UDO_PassiveBase;
class UDC_AnimMontage;
class UDC_Stat;
class UDO_AnimInstance;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnActiveChangeDelegate, const FString&, _Key, UDO_ActiveBase*, _Skill, bool, bIsRegist);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnPassiveChangeDelegate, const FString&, _Key, UDO_PassiveBase*, _Skill, bool, bIsRegist);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSkillEndDelegate);

//캐릭터의 스킬을 관리하는 컴포넌트
UCLASS(ClassGroup = (DC_Share), meta = (BlueprintSpawnableComponent))
class DEFENDER_API UDC_Skill : public UDC_PawnComponent
{
	GENERATED_BODY()
public:
	//Delegate called when the activated skill changes
	UPROPERTY(BlueprintAssignable)
	FOnActiveChangeDelegate onActiveChange;

	//Delegate called when the passive skill changes
	UPROPERTY(BlueprintAssignable)
	FOnPassiveChangeDelegate onPassiveChange;

	//Delegate called when skill use ends
	UPROPERTY(BlueprintAssignable)
	FOnSkillEndDelegate onSkillEnd;

protected:

private:
	//Active skills that the character has
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skills", meta = (AllowPrivateAccess = true))
	TMap<FString, UDO_ActiveBase*> activeSkills;

	//Passive skills that the character has
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skills", meta = (AllowPrivateAccess = true))
	TMap<FString, UDO_PassiveBase*> passiveSkills;

	//Active skill data table for json save
	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "SkillData", meta = (AllowPrivateAccess = true))
	UDataTable* activeSkillDataTable;

	//Passive skill data table for json save
	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "SkillData", meta = (AllowPrivateAccess = true))
	UDataTable* passiveSkillDataTable;

	UDO_ActiveBase*				activationSkill;		//현재 발동중인 스킬
	UDC_AnimMontage*			montageComp;			//애니메이션 재생을 위한 몽타주 컴포넌트
	UDC_Stat*					statComp;				//상태 변경을 위한 스탯 컴포넌트
	UDO_AnimInstance*			currentAnimInstance;	//현재 애니메이션 인스턴스
	int32						motionIndex;			//재생할 모션의 인덱스
	bool						bIsLoopPass;			//반복재생하던 구간을 통과할지 여부
	bool						bCasting;				//현재 사용중인지 여부
	int32						currentSection;			//콤보공격의 현재 섹션
	
public:
	UDC_Skill();
	/*
	* Uses saved skills according to key input.
	* @param[in] _Key Key to the skill to use
	*/
	void SkillRegistration(FKey _Key);
	/*
	* When you release the skill key, the skill is activated.
	*/
	void SkillActivation();
	/*
	* Apply passive
	*/
	void ApplyPassive();
	/*
	* Add active skills according to class
	* @param[in] _Key Keys to add to skill map
	* @param[in] _SkillClass Class of skill to add to skill map
	* @return Success or failure of skill addition
	*/
	UFUNCTION(BlueprintCallable, category = "SkillComp")
	bool AddActiveSkillbyClass(const FString& _Key, TSubclassOf<UDO_ActiveBase> _SkillClass);
	/*
	* Add active skills according to skill name
	* The skill name is registered in the data table.
	* @param[in] _Key Keys to add to skill map
	* @param[in] _SkillClass Name of skill to add to skill map
	* @return Success or failure of skill addition
	*/
	UFUNCTION(BlueprintCallable, category = "SkillComp")
	bool AddActiveSkillbyName(const FString& _Key, const FString& _SkillName);
	/*
	* Delete the skill corresponding to the key
	* @param[in] _Key Key to erase skill
	* @return Success or failure of skill deletion
	*/
	UFUNCTION(BlueprintCallable, category = "SkillComp")
	bool RemoveActiveSkill(const FString& _Key);
	/*
	* Add passive skills according to class
	* @param[in] _Key Keys to add to skill map
	* @param[in] _SkillClass Class of skill to add to skill map
	* @return Success or failure of skill addition
	*/
	UFUNCTION(BlueprintCallable, category = "SkillComp")
	bool AddPassiveSkillbyClass(const FString& _Key, TSubclassOf<UDO_PassiveBase> _SkillClass);
	/*
	* Add passive skills according to skill name
	* The skill name is registered in the data table.
	* @param[in] _Key Keys to add to skill map
	* @param[in] _SkillClass Name of skill to add to skill map
	* @return Success or failure of skill addition
	*/
	UFUNCTION(BlueprintCallable, category = "SkillComp")
	bool AddPassiveSkillbyName(const FString& _Key, const FString& _SkillName);
	/*
	* Delete the skill corresponding to the key
	* @param[in] _Key Key to erase skill
	* @return Success or failure of skill deletion
	*/
	UFUNCTION(BlueprintCallable, category = "SkillComp")
	bool RemovePassiveSkill(const FString& _Key);
	/*
	* Deletes all active skills you have
	*/
	UFUNCTION(BlueprintCallable, category = "SkillComp")
	void RemoveAllActive();
	/*
	* Deletes all passive skills you have
	*/
	UFUNCTION(BlueprintCallable, category = "SkillComp")
	void RemoveAllPassive();
	/*
	* Get the map of the active skill
	* @return return the const TMap<FString, UDO_ActiveBase*>&
	*/
	FORCEINLINE const TMap<FString, UDO_ActiveBase*>& GetActiveSkillMap()const { return activeSkills; }
	/*
	* Get the map of the passive skill
	* @return return the const TMap<FString, UDO_ActiveBase*>&
	*/
	FORCEINLINE const TMap<FString, UDO_PassiveBase*>& GetPassiveSkillMap()const { return passiveSkills; }
	/*
	* Get the skill corresponding to the key from the map
	* @param[in] _Key Keys to find on the map
	* @return return the UDO_ActiveBase*
	*/
	UDO_ActiveBase* GetActiveSkill(const FString& _Key)const;
	/*
	* Get the skill corresponding to the key from the map
	* @param[in] _Key Keys to find on the map
	* @return return the UDO_PassiveBase*
	*/
	UDO_PassiveBase* GetPassiveSkill(const FString& _Key)const;
	/*
	* Sets the index of the montage to play when activating the skill
	* @param[in] _Index index to set
	*/
	void SetMotionIndex(int32 _Index);
	/*
	* Cancels the skill that was being activated.
	*/
	UFUNCTION(BlueprintCallable, category = "SkillComp")
	void CancelSkill();

protected:
	virtual void BeginPlay()override;

private:
	UFUNCTION()
	void AnimInstanceChange(UDO_AnimInstance* _AnimInstance);

	UFUNCTION()
	void ChangedState(ECHARACTER_STATE _NewState);

	UFUNCTION()
	void CastingStart();

	UFUNCTION()
	void LoopCheck();

	UFUNCTION()
	void UseSkill();

	UFUNCTION()
	void EndSkill();

	UFUNCTION()
	void ComboAttack();

	UFUNCTION()
	void SkillNotify();
};
