#pragma once

#include "Widget/DW_PlayerWidget.h"
#include "DWP_SelectSkillSlot.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSkillCheckedDelegate, UDWP_SelectSkillSlot*, _CheckedSlot, bool, _IsChecked);

class UCheckBox;
class UTextBlock;
class UImage;

//스킬을 선택하는 위젯슬롯
UCLASS()
class DEFENDER_API UDWP_SelectSkillSlot : public UDW_PlayerWidget
{
	GENERATED_BODY()
public:
	//Delegate to be called when the widget is checked
	UPROPERTY(BlueprintAssignable)
	FOnSkillCheckedDelegate onChecked;

protected:

private:
	//Checkbox to display checked status
	UPROPERTY(BlueprintReadOnly, category = "Select Skill Slot", meta = (BindWidget, AllowPrivateAccess = true))
	UCheckBox* cb_Check;

	//Image widget to display skill icon
	UPROPERTY(BlueprintReadOnly, category = "Select Skill Slot", meta = (BindWidget, AllowPrivateAccess = true))
	UImage* img_SkillIcon;

	//A text block that tells you which key to set the skill to.
	UPROPERTY(BlueprintReadOnly, category = "Select Skill Slot", meta = (BindWidget, AllowPrivateAccess = true))
	UTextBlock* txt_Index;

	FString				skillName;			//스킬 이름
	bool				bIsActiveSkill;		//액티브스킬인지 여부

public:
	/*
	* Change check status
	* @param[in] _bIsCheck If true, changes to checked state.
	*/
	void SetCheckState(bool _bIsCheck);
	/*
	* Determine the type of skill
	* @param[in] _bIsActiveSkill If true, it is an active skill, if false, it is a passive skill.
	*/
	void SetSkillType(bool _bIsActiveSkill);
	/*
	* Set the name of the skill
	* @param[in] _SkillName Name of skill to set
	*/
	void SetSkillName(const FString& _SkillName);
	/*
	* Set which numeric key to assign the skill to
	* @param[in] _Index index to allocate
	*/
	void SetIndex(int32 _Index);
	/*
	* Set the image of the skill
	* @param[in] _NewImage Image of skill to set
	*/
	void SetSkillImage(UTexture2D* _NewImage);
	/*
	* Get the name of the skill
	* @return Returns const FString&
	*/
	FORCEINLINE const FString& GetSkillName()const { return skillName; }
	/*
	* It tells you whether the current skill is an active skill.
	* @return If true, it is an active skill.
	*/
	FORCEINLINE bool IsActiveSkill()const { return bIsActiveSkill; }

protected:
	virtual void NativeConstruct()override;
	virtual void NativeDestruct()override;

private:
	UFUNCTION()
	void OnCheck(bool _bIsChecked);

};
