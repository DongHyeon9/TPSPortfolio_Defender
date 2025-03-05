#pragma once

#include "../Defender.h"
#include "Animation/AnimInstance.h"
#include "DO_AnimInstance.generated.h"

class ADA_Character;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCastingStartDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSkillLoopCheckDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUseSkillDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSkillNotifyDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEndSkillDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHitStartDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHitEndDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStunEndDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnComboAttackDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeadEndDelegate);

//프로젝트에서 사용하는 애님인스턴스의 최상위 클래스
UCLASS(Abstract, NotBlueprintable)
class DEFENDER_API UDO_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	//Delegate called when casting begins
	UPROPERTY(BlueprintAssignable)
	FOnCastingStartDelegate onCastingStart;

	UPROPERTY(BlueprintAssignable)
	FOnSkillLoopCheckDelegate onSkillLoopCheck;

	//Delegate called when a skill is used
	UPROPERTY(BlueprintAssignable)
	FOnUseSkillDelegate onUseSkill;

	UPROPERTY(BlueprintAssignable)
	FOnSkillNotifyDelegate onSkillNotify;

	UPROPERTY(BlueprintAssignable)
	FOnEndSkillDelegate onEndSkill;

	//Delegate called when a shot starts
	UPROPERTY(BlueprintAssignable)
	FOnHitStartDelegate onHitStart;

	//Delegate called when the hit ends
	UPROPERTY(BlueprintAssignable)
	FOnHitEndDelegate onHitEnd;

	UPROPERTY(BlueprintAssignable)
	FOnStunEndDelegate onStunEnd;

	UPROPERTY(BlueprintAssignable)
	FOnComboAttackDelegate onComboAttack;

	UPROPERTY(BlueprintAssignable)
	FOnDeadEndDelegate onDeadEnd;

protected:

private:
	//Owner of AnimInstance
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "Character", meta = (AllowPrivateAccess = true))
	ADA_Character* ownerCharacter;

	//owner's current speed
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "CharacterMove", meta = (AllowPrivateAccess = true))
	float currentCharacterSpeed;

	//owner's current velocity
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "CharacterMove", meta = (AllowPrivateAccess = true))
	FVector characterVelocity;

	//player jump state
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "CharacterState", meta = (AllowPrivateAccess = true))
	bool bIsInAir;

	//Current character status
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "CharacterState", meta = (AllowPrivateAccess = true))
	ECHARACTER_STATE characterState;

	//Index of preparatory animation
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "Skill Cast", meta = (AllowPrivateAccess = true))
	int32 castingAnimationIndex;

public:
	UDO_AnimInstance();
	/*
	* Return the owner of the AnimInstance
	* @return Return the ADA_Character*
	*/
	FORCEINLINE ADA_Character* GetOwnerCharacter()const { return ownerCharacter; }
	/*
	* Return the character's current speed
	* @return Return the float
	*/
	FORCEINLINE float GetCurrentSpeed()const { return currentCharacterSpeed; }
	/*
	* Return the character's current velocity
	* @return Return the const FVector&
	*/
	FORCEINLINE const FVector& GetCurrentVelocity()const { return characterVelocity; }
	/*
	* Return the character's current jump state
	* @return Return the bool
	*/
	FORCEINLINE bool IsInAir()const { return bIsInAir; }

protected:
	virtual void NativeUpdateAnimation(float _DeltaSeconds)override;

	UFUNCTION()
	void AnimNotify_CastingStart();

	UFUNCTION()
	void AnimNotify_SkillLoopCheck();

	UFUNCTION()
	void AnimNotify_UseSkill();

	UFUNCTION()
	void AnimNotify_SkillNotify();

	UFUNCTION()
	void AnimNotify_EndSkill();

	UFUNCTION()
	void AnimNotify_HitStart();

	UFUNCTION()
	void AnimNotify_HitEnd();

	UFUNCTION()
	void AnimNotify_StunEnd();

	UFUNCTION()
	void AnimNotify_ComboAttack();

	UFUNCTION()
	void AnimNotify_DeadEnd();

private:

};
