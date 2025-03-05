#pragma once

#include "Widget/DW_PlayerWidget.h"
#include "DWP_StartMain.generated.h"

class ADAPC_Select;

class UButton;

//게임을 시작하면 제일 먼저 보일 메인 위젯
UCLASS()
class DEFENDER_API UDWP_StartMain : public UDW_PlayerWidget
{
	GENERATED_BODY()
public:

protected:

private:
	//Button that takes you to the character creation screen when clicked
	UPROPERTY(BlueprintReadOnly, category = "Start Main", meta = (BindWidget, AllowPrivateAccess = true))
	UButton* btn_Start;

	//Button that moves to the loading screen when clicked
	UPROPERTY(BlueprintReadOnly, category = "Start Main", meta = (BindWidget, AllowPrivateAccess = true))
	UButton* btn_Load;

	//Button that ends the game when clicked
	UPROPERTY(BlueprintReadOnly, category = "Start Main", meta = (BindWidget, AllowPrivateAccess = true))
	UButton* btn_Quit;

	UPROPERTY()
	ADAPC_Select*			ownerPlayerController;			//위젯을 가지고있는 플레이어 컨트롤러

public:

protected:
	virtual void NativeConstruct()override;
	virtual void NativeDestruct()override;

private:
	UFUNCTION()
	void OnClickedQuit();

};
