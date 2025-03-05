#pragma once

#include "Widget/DW_PlayerWidget.h"
#include "DWP_SkillSlot.generated.h"

class UDO_ActiveBase;

class UProgressBar;
class UImage;
class UTextBlock;

//플레이어가 사용가능한 스킬을 표시하는 위젯
UCLASS()
class DEFENDER_API UDWP_SkillSlot : public UDW_PlayerWidget
{
	GENERATED_BODY()
public:

protected:

private:
	//Widget to display image of skill
	UPROPERTY(BlueprintReadOnly, category = "SkillSlot", meta = (BindWidget, AllowPrivateAccess = true))
	UImage* img_Skill;

	//Displays skill cooldown
	UPROPERTY(BlueprintReadOnly, category = "SkillSlot", meta = (BindWidget, AllowPrivateAccess = true))
	UTextBlock* txt_SkillCooldown;

	//Indicates the input key for the skill
	UPROPERTY(BlueprintReadOnly, category = "SkillSlot", meta = (BindWidget, AllowPrivateAccess = true))
	UTextBlock* txt_SkillKey;

	//Shows the cooldown of the skill as a percentage.
	UPROPERTY(BlueprintReadOnly, category = "SkillSlot", meta = (BindWidget, AllowPrivateAccess = true))
	UProgressBar* pb_CoolDown;

	//key to skill
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "Skill Slot Setting", meta = (AllowPrivateAccess = true))
	FString key;

	UDO_ActiveBase*			currentSkill;		//위젯에 등록된 스킬의 정보

public:
	UDWP_SkillSlot(const FObjectInitializer& _ObjectInitializer);
	/*
	* Set the skill key
	* @param[in] _Key Key that is activated when pressed
	*/
	void SetSkillKey(const FString& _Key);
	/*
	* Set the key text
	* @param[in] _Key Text of key to set
	*/
	void SetSkillKeyText(const FString& _Key);
	/*
	* Set the skill image
	* @param[in] _SkillImage Image of skill to set
	*/
	void SetSkillImage(UTexture2D* _SkillImage);
	/*
	* Gets the key that is activated when pressed
	* @return Returns the const FString&
	*/
	FORCEINLINE const FString& GetSkillKey()const { return key; }
	/*
	* Function called when a skill changes
	* @param[in] _Key Key that is activated when pressed
	* @param[in] _Skill Information on the skill to change
	* @param[in] _bIsRegist Determine whether to register or delete
	*/
	UFUNCTION()
	void SetActiveSkillChange(const FString& _Key, UDO_ActiveBase* _Skill, bool _bIsRegist);

protected:
	virtual void NativeDestruct()override;

private:

	UFUNCTION()
	void SetSkillProgress(float _Total, float _Current);
};
