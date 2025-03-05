#pragma once

#include "Widget/DW_PlayerWidget.h"
#include "DWP_Count.generated.h"

class UTextBlock;
class UButton;
class UEditableTextBox;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnConfirmedDelegate, int32, _Count, int32, _TotalPrice);

//��� ��� ������ �÷��̾ �Է��� �� �ִ� ����
UCLASS()
class DEFENDER_API UDWP_Count : public UDW_PlayerWidget
{
	GENERATED_BODY()
public:
	//Delegate that tells you how much the purchase or sale is confirmed
	UPROPERTY(BlueprintAssignable)
	FOnConfirmedDelegate onConfirmed;

protected:

private:
	//Button that decreases the number by 1
	UPROPERTY(BlueprintReadOnly, category = "CountWidget", meta = (BindWidget, AllowPrivateAccess = true))
	UButton* btn_Minus;

	//Buttons whose number increases by 1
	UPROPERTY(BlueprintReadOnly, category = "CountWidget", meta = (BindWidget, AllowPrivateAccess = true))
	UButton* btn_Plus;

	//Cancel button
	UPROPERTY(BlueprintReadOnly, category = "CountWidget", meta = (BindWidget, AllowPrivateAccess = true))
	UButton* btn_Cancel;

	//Confirm button
	UPROPERTY(BlueprintReadOnly, category = "CountWidget", meta = (BindWidget, AllowPrivateAccess = true))
	UButton* btn_OK;

	//Total amount of gold required for purchase
	UPROPERTY(BlueprintReadOnly, category = "CountWidget", meta = (BindWidget, AllowPrivateAccess = true))
	UTextBlock* txt_TotalGold;

	//Text box to enter the quantity to purchase
	UPROPERTY(BlueprintReadOnly, category = "CountWidget", meta = (BindWidget, AllowPrivateAccess = true))
	UEditableTextBox* etb_Count;

	int32		currentCount;		//���� ������ ����
	int32		playerGold;			//�÷��̾��� ���� ���
	int32		itemPrice;			//������ ����
	bool		bIsBuy;				//�Ǹ����� �������� �Ǻ�

public:
	/*
	* Set the player's gold holdings
	* @param[in] _TotalGold Player's gold holdings
	*/
	void SetPlayerGold(int32 _TotalGold);
	/*
	* Set the price of the item
	* @param[in] _ItemPrice the price of the item
	*/
	void SetItemPrice(int32 _ItemPrice);
	/*
	* Decide whether to buy or sell
	* @param[in] _bIsBuy If purchasing, enter true
	*/
	void SetBuy(bool _bIsBuy);

protected:
	virtual void NativeConstruct()override;
	virtual void NativeDestruct()override;

private:
	UFUNCTION()
	void OnCommitText(const FText& _Text, ETextCommit::Type _CommitMethod);

	UFUNCTION()
	void OnClickedMinus();

	UFUNCTION()
	void OnClickedPlus();

	UFUNCTION()
	void OnClickedOK();

	UFUNCTION()
	void OnClickedCancel();

	void CalculateTextBox();
};
