#pragma once

#include "Widget/DW_PlayerWidget.h"
#include "DWP_StartSkill.generated.h"

class UDWP_SelectSkillSlot;
class ADAPC_Select;

class UButton;

//스킬을 설정하는 단계의 위젯
UCLASS()
class DEFENDER_API UDWP_StartSkill : public UDW_PlayerWidget
{
	GENERATED_BODY()
public:

protected:

private:
	//스킬선택 위젯슬롯을 담고있는 배열
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "Start Skill", meta = (AllowPrivateAccess = true))
	TArray<UDWP_SelectSkillSlot*> skillSlots;

	//Button to start the game after checking whether all skills have been selected when clicked
	UPROPERTY(BlueprintReadOnly, category = "Start Skill", meta = (BindWidget, AllowPrivateAccess = true))
	UButton* btn_Check;

	//Button to reset selected skills
	UPROPERTY(BlueprintReadOnly, category = "Start Skill", meta = (BindWidget, AllowPrivateAccess = true))
	UButton* btn_Reset;

	//Button to return to character selection screen
	UPROPERTY(BlueprintReadOnly, category = "Start Skill", meta = (BindWidget, AllowPrivateAccess = true))
	UButton* btn_Back;

	//Data table containing information about the active skills to be displayed
	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "Start Skill Setting", meta = (AllowPrivateAccess = true))
	UDataTable* activeDataTable;

	//Data table containing information about the passive skills to be displayed
	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "Start Skill Setting", meta = (AllowPrivateAccess = true))
	UDataTable* passiveDataTable;

	UPROPERTY()
	ADAPC_Select*						ownerPlayerController;			//위젯을 가지고있는 플레이어 컨트롤러

public:
	UDWP_StartSkill(const FObjectInitializer& _ObjectInitializer);
	/*
	* Initialize widgets before adding them to the viewport.
	*/
	virtual void InitializeWidget()override;

protected:
	virtual void NativeConstruct()override;
	virtual void NativeDestruct()override;

private:
	UFUNCTION()
	void OnClickedCheck();

	UFUNCTION()
	void OnClickedReset();

	UFUNCTION()
	void OnCheck(UDWP_SelectSkillSlot* _CheckedSlot, bool _IsChecked);
};
