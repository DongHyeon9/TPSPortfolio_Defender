#pragma once

#include "Widget/DW_PlayerWidget.h"
#include "DWP_StageClear.generated.h"

class UTextBlock;

//스테이지를 클리어하면 표시될 위젯
UCLASS()
class DEFENDER_API UDWP_StageClear : public UDW_PlayerWidget
{
	GENERATED_BODY()
public:

protected:

private:
	//Displays current score
	UPROPERTY(BlueprintReadOnly, category = "Stage Clear Widget", meta = (BindWidget, AllowPrivateAccess = true))
	UTextBlock* txt_CurrentScore;

	//Show overall score
	UPROPERTY(BlueprintReadOnly, category = "Stage Clear Widget", meta = (BindWidget, AllowPrivateAccess = true))
	UTextBlock* txt_TotalScore;

	//Indicates the player's level
	UPROPERTY(BlueprintReadOnly, category = "Stage Clear Widget", meta = (BindWidget, AllowPrivateAccess = true))
	UTextBlock* txt_Level;

	//Represents the player's gold
	UPROPERTY(BlueprintReadOnly, category = "Stage Clear Widget", meta = (BindWidget, AllowPrivateAccess = true))
	UTextBlock* txt_Gold;

public:

protected:
	virtual void NativeConstruct()override;

private:

};
