#pragma once

#include "Widget/DW_PlayerWidget.h"
#include "DWP_BuySellButton.generated.h"

class UButton;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBuySellModeDelegate, bool, _bIsBuy);

//플레이어가 살건지 팔건지를 고를 수 있는 위젯
UCLASS()
class DEFENDER_API UDWP_BuySellButton : public UDW_PlayerWidget
{
	GENERATED_BODY()
public:
	//A delegate that changes depending on which button is clicked to buy or sell
	UPROPERTY(BlueprintAssignable)
	FOnBuySellModeDelegate onBuySellMode;

protected:

private:
	//buy button
	UPROPERTY(BlueprintReadOnly, category = "BuySellButton", meta = (BindWidget, AllowPrivateAccess = true))
	UButton* btn_Buy;

	//sale button
	UPROPERTY(BlueprintReadOnly, category = "BuySellButton", meta = (BindWidget, AllowPrivateAccess = true))
	UButton* btn_Sell;

public:

protected:
	virtual void NativeConstruct()override;
	virtual void NativeDestruct()override;

private:
	UFUNCTION()
	void OnClickedBuy();

	UFUNCTION()
	void OnClickedSell();
};
