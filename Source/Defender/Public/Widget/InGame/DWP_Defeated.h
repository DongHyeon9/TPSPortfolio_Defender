#pragma once

#include "Widget/DW_PlayerWidget.h"
#include "DWP_Defeated.generated.h"

class UButton;
class UTextBlock;

//플레이어가 패배시 보여줄 위젯
UCLASS()
class DEFENDER_API UDWP_Defeated : public UDW_PlayerWidget
{
	GENERATED_BODY()
public:

protected:

private:
	//Text block to display the points earned
	UPROPERTY(BlueprintReadOnly, category = "Defeated", meta = (BindWidget, AllowPrivateAccess = true))
	UTextBlock* txt_Score;

	//Return to lobby button
	UPROPERTY(BlueprintReadOnly, category = "Defeated", meta = (BindWidget, AllowPrivateAccess = true))
	UButton* btn_ToRobby;

	//Name of lobby to return to
	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "Defeated", meta = (AllowPrivateAccess = true))
	FName robbyName;

public:
	UDWP_Defeated(const FObjectInitializer& _ObjectInitializer);
	/*
	* Add a widget to the viewport. Returns false if the widget is already added to the viewport.
	* @param[in] _Order Viewport order for widgets
	* @return Successful addition of widget
	*/
	virtual bool ShowInViewport(int32 _Order)override;

protected:
	virtual void NativeConstruct()override;
	virtual void NativeDestruct()override;

private:
	UFUNCTION()
	void OnClickedToRobby();
};
