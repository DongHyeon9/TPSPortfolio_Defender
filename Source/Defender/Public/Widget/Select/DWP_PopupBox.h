#pragma once

#include "Widget/DW_PlayerWidget.h"
#include "DWP_PopupBox.generated.h"

class UButton;
class UTextBlock;

//플레이어에게 안내해줄 팝업박스
UCLASS()
class DEFENDER_API UDWP_PopupBox : public UDW_PlayerWidget
{
	GENERATED_BODY()
public:

protected:

private:
	//When you press the button, the widget is deleted.
	UPROPERTY(BlueprintReadOnly, category = "PopupBox", meta = (BindWidget, AllowPrivateAccess = true))
	UButton* btn_Check;

	//Message to the player
	UPROPERTY(BlueprintReadOnly, category = "PopupBox", meta = (BindWidget, AllowPrivateAccess = true))
	UTextBlock* txt_Description;

public:
	/*
	* Set a message to notify the player
	* @param[in] _NewText Message to set
	*/
	void SetDescriptionText(const FText& _NewText);

protected:
	virtual void NativeConstruct()override;
	virtual void NativeDestruct()override;

private:
	UFUNCTION()
	void OnClickedCheckButton();
};
