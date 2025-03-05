#pragma once

#include "Skills/DO_SkillBase.h"
#include "UObject/NoExportTypes.h"
#include "DO_ActiveBase.generated.h"

class UAnimMontage;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCoolDownDelegate, float, _Total, float, _Current);

//��Ƽ�� ��ų�� �ֻ��� Ŭ����
UCLASS(Blueprintable,BlueprintType)
class DEFENDER_API UDO_ActiveBase : public UDO_SkillBase
{
	GENERATED_BODY()
public: 
	//Delegate called whenever the remaining cooldown changes
	UPROPERTY(BlueprintAssignable)
	FOnCoolDownDelegate onCoolDown;

protected:
	FTimerHandle		coolDownHandle;				//��Ÿ�� ��꿡 ���Ǵ� Ÿ�̸� �ڵ�

private:
	//Total cooldown time of skill
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, category = "ActiveSkill Common", meta = (AllowPrivateAccess = true))
	float totalCoolDown;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "ActiveSkill Common", meta = (AllowPrivateAccess = true))
	bool bIsCharging;

	//Animmontage that plays when using a skill
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, category = "ActiveSkill Common", meta = (AllowPrivateAccess = true))
	TArray<UAnimMontage*> castAction;

	//State of the character when the skill is activated
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ActiveSkill Common", meta = (AllowPrivateAccess = true))
	ECHARACTER_STATE castState;

	//Whether to use line tracing in the preparation stage
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "LineTrace Setting", meta = (AllowPrivateAccess = true))
	bool bIsLineTraceSkill;

	//Starting position of line tracing
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, category = "LineTrace Setting", meta = (AllowPrivateAccess = true))
	FVector startLoctionTrace;

	//End position of line tracing
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, category = "LineTrace Setting", meta = (AllowPrivateAccess = true))
	FVector endLoctionTrace;

	//Type of object to be inspected by line tracing
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "LineTrace Setting", meta = (AllowPrivateAccess = true))
	TArray<TEnumAsByte<EObjectTypeQuery>> objectTypeTrace;

	//Actors that line tracing ignores
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, category = "LineTrace Setting", meta = (AllowPrivateAccess = true))
	TArray<AActor*> ignoreActorsTrace;	

	float				currentCoolDown;			//���� ��Ÿ��
	float				cooldownCheckInterval;		//��Ÿ�� ����� �ϴ� ����
	bool				bIsPreparation;				//��ų�� �غ�ܰ� ���� �Ǻ�

public:
	UDO_ActiveBase();
	/*
	* It puts the skill in the preparation stage.
	* In the skill preparation stage, the logic is implemented in the subclass.
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, category = "ActiveSkill", meta = (HideSelfPin = true))
	void SkillPreparation();
	virtual void SkillPreparation_Implementation();
	/*
	* Use the skill. 
	* The usage logic is implemented in the subclass.
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, category = "ActiveSkill", meta = (HideSelfPin = true))
	void UseSkill();
	virtual void UseSkill_Implementation();
	/*
	* Cancel the skill.
	* Cancellation logic is implemented in subclasses.
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, category = "ActiveSkill", meta = (HideSelfPin = true))
	void CancelSkill();
	virtual void CancelSkill_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, category = "ActiveSkill", meta = (HideSelfPin = true))
	void EndSkill();
	virtual void EndSkill_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, category = "ActiveSkill", meta = (HideSelfPin = true))
	void SkillNotify();
	virtual void SkillNotify_Implementation();
	/*
	* Returns the result of line tracing used in the tick. 
	* Logic is implemented in subclasses
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, category = "ActiveSkill", meta = (HideSelfPin = true))
	void SkillTick(float _DeltaTime, const FHitResult& _HitResult);
	virtual void SkillTick_Implementation(float _DeltaTime, const FHitResult& _HitResult);
	/*
	* Cool time applies. 
	* If you do not enter the parameter or enter a negative number, the skill's original cooldown time is applied.
	* @param[in] _CoolDown Cool time to apply
	*/
	UFUNCTION(BlueprintCallable, category = "ActiveSkill")
	virtual void ApplyCoolDown(float _CoolDown = -1.f);
	/*
	* Check whether the current skill is available
	* @return returns bool
	*/
	bool IsAvailable() { return currentCoolDown <= 0; }
	/*
	* Set the character to use
	* @param[in] _OwnerCharacter Owner to set up
	*/
	virtual void SetOwnerCharacter(ADA_Character* _OwnerCharacter);
	/*
	* Get the Anim Montage that plays while casting a skill.
	* @param[in] _Index Index of the montage to get
	* @return return the UAnimMontage*
	*/
	FORCEINLINE UAnimMontage* GetCastAction(int32 _Index = 0)const;
	/*
	* Retrieves the player's status to be set when the skill is activated.
	* @return return the ECHARACTER_STATE
	*/
	FORCEINLINE ECHARACTER_STATE GetCastState()const { return castState; }
	/*
	* Gets whether it is a rechargeable skill or not.
	* @return Returns bool
	*/
	FORCEINLINE bool IsChargingSkill()const { return bIsCharging; }
	/*
	* Whether the skill plays an animation montage
	* @return Returns bool
	*/
	FORCEINLINE bool IsSkillWithAnimation()const { return !castAction.IsEmpty(); }

protected:
	virtual void Tick(float _DeltaTime)override;

private:

};
