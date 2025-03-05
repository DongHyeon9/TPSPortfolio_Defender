#pragma once

#include "Widget/DW_UserWidget.h"
#include "DW_MonsterHP.generated.h"

class ADA_Monster;

class UProgressBar;
class UImage;
class UTextBlock;

//몬스터의 현재 HP를 표시해주는 위젯
UCLASS()
class DEFENDER_API UDW_MonsterHP : public UDW_UserWidget
{
	GENERATED_BODY()
public:

protected:

private:
	//Progress bar to visually display HP
	UPROPERTY(BlueprintReadOnly, category = "Monster Info", meta = (BindWidget, AllowPrivateAccess = true))
	UProgressBar* pb_HPBar;

	//pictures of monsters
	UPROPERTY(BlueprintReadOnly, category = "Monster Info", meta = (BindWidget, AllowPrivateAccess = true))
	UImage* img_Monster;

	//Text block to display the name of the monster
	UPROPERTY(BlueprintReadOnly, category = "Monster Info", meta = (BindWidget, AllowPrivateAccess = true))
	UTextBlock* txt_MonsterStatus;

public:
	/*
	* Initializes the widget to display information about the current monster.
	*/
	virtual void InitializeWidget()override;
	/*
	* Set the image of the monster
	* @param[in] _NewTexture Texture to set
	*/
	void SetMonsterImage(UTexture2D* _NewTexture);

protected:
	virtual void NativeConstruct()override;
	virtual void NativeDestruct()override;

private:
	UFUNCTION()
	void OnChangeHP(float _MaxHP, float _CurrentHP);
};
