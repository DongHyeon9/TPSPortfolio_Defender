#pragma once

#include "Widget/DW_PlayerWidget.h"
#include "DWP_StartLoad.generated.h"

class ADAPC_Select;

class UButton;
class UEditableTextBox;

//�ҷ����⸦ �� �� �ִ� �ܰ��� ����
UCLASS()
class DEFENDER_API UDWP_StartLoad : public UDW_PlayerWidget
{
	GENERATED_BODY()
public:

protected:

private:
	//Button to check if loading is possible when clicked
	UPROPERTY(BlueprintReadOnly, category = "Start Load", meta = (BindWidget, AllowPrivateAccess = true))
	UButton* btn_Check;

	//Button to return to main screen when clicked
	UPROPERTY(BlueprintReadOnly, category = "Start Load", meta = (BindWidget, AllowPrivateAccess = true))
	UButton* btn_Back;

	//Editable text box where you can write the saved ID
	UPROPERTY(BlueprintReadOnly, category = "Start Load", meta = (BindWidget, AllowPrivateAccess = true))
	UEditableTextBox* etb_ID;

	UPROPERTY()
	ADAPC_Select*			ownerPlayerController;			//������ �������ִ� �÷��̾� ��Ʈ�ѷ�

public:

protected:
	virtual void NativeConstruct()override;
	virtual void NativeDestruct()override;

private:
	UFUNCTION()
	void OnClickedCheck();
};
