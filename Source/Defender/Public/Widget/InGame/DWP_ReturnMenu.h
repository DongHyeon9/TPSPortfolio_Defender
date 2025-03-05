#pragma once

#include "Widget/DW_PlayerWidget.h"
#include "DWP_ReturnMenu.generated.h"

class UDWP_PopupBox;

class UButton;

//플레이가 게임을 저장 혹은 로비로 돌아가기 위해 사용할 위젯
UCLASS()
class DEFENDER_API UDWP_ReturnMenu : public UDW_PlayerWidget
{
	GENERATED_BODY()
public:

protected:
	
private:
	//Button to clear the widget and continue the game
	UPROPERTY(BlueprintReadOnly, category = "Return Menu", meta = (BindWidget, AllowPrivateAccess = true))
	UButton* btn_ReturnToGame;

	//Button to save current player data
	UPROPERTY(BlueprintReadOnly, category = "Return Menu", meta = (BindWidget, AllowPrivateAccess = true))
	UButton* btn_SaveGame;

	//Button to return to the lobby
	UPROPERTY(BlueprintReadOnly, category = "Return Menu", meta = (BindWidget, AllowPrivateAccess = true))
	UButton* btn_ReturnToRobby;

	//Name of lobby to return to
	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "Return Menu", meta = (AllowPrivateAccess = true))
	FName robbyName;

	//A widget class for a pop - up box that guides the player.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "Return Menu", meta = (AllowPrivateAccess = true))
	TSubclassOf<UDWP_PopupBox> popupBoxWidgetClass;

	UPROPERTY()
	UDWP_PopupBox*			popupBoxWidget;			//팝업박스 위젯의 인스턴스

public:
	UDWP_ReturnMenu(const FObjectInitializer& _ObjectInitializer);
	/*
	* Add a widget to the viewport. Returns false if the widget is already added to the viewport.
	* @param[in] _Order Viewport order for widgets
	* @return Successful addition of widget
	*/
	virtual bool ShowInViewport(int32 _Order)override;
	/*
	* Removes a widget from the viewport. Returns failure if the widget is not added to the viewport.
	* @return Success or failure of widget deletion
	*/
	virtual bool RemoveInViewport()override;

protected:
	virtual void NativeConstruct()override;
	virtual void NativeDestruct()override;

private:
	UFUNCTION()
	void OnClickedToRobby();

	UFUNCTION()
	void OnClickedToGame();

	UFUNCTION()
	void OnClickedSaveGame();
};
