#pragma once

#include "Skills/Active/DO_ActiveBase.h"
#include "DOSA_Timer.generated.h"

class UDC_Stat;
class UDO_AnimInstance;

//지속시간이 있는 스킬의 최상위 클래스
UCLASS()
class DEFENDER_API UDOSA_Timer : public UDO_ActiveBase
{
	GENERATED_BODY()
public:

protected:

private:
	//Skill duration
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "TimerSetting", meta = (AllowPrivateAccess = true))
	float skillDuration;

	//Whether to apply cooldown after use ends
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "TimerSetting", meta = (AllowPrivateAccess = true))
	bool afterCoolDownApply;

	FTimerHandle		skillTimerHandle;		//스킬 지속시간을 관리하는 타이머의 핸들
	UDC_Stat*			ownerStatComp;			//상태를 설정할 오너캐릭터의 스탯컴포넌트
	UDO_AnimInstance*	animInstance;			//인덱스를 전달해줄 오너캐릭터의 애님인스턴스

public:
	UDOSA_Timer();

	virtual void UseSkill_Implementation()override;
	/*
	* Called when the skill duration ends
	* Logic implementation is done in subclasses.
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, category = "ActiveSkill", meta = (HideSelfPin = true))
	void SkillTimeOut();
	virtual void SkillTimeOut_Implementation();
	/*
	* Set the character to use
	* @param[in] _OwnerCharacter Owner to set up
	*/
	virtual void SetOwnerCharacter(ADA_Character* _OwnerCharacter)override;
	/*
	* Cancel the skill
	*/
	virtual void CancelSkill_Implementation()override;
	/*
	* Cool time applies.
	* If you do not enter the parameter or enter a negative number, the skill's original cooldown time is applied.
	* @param[in] _CoolDown Cool time to apply
	*/
	virtual void ApplyCoolDown(float _CoolDown = -1.f)override;

protected:

private:

};
