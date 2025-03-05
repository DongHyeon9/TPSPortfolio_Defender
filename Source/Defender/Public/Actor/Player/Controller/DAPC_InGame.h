#pragma once

#include "Actor/Player/DA_PlayerController.h"
#include "DAPC_InGame.generated.h"

class UDWP_MainWidget;
class UDWP_Infomation;
class UDW_PlayerWidget;
class UDWP_Shop;
class UDWP_StageClear;
class UDWP_StageStart;
class UDWP_ReturnMenu;
class UDWP_Defeated;

//�ΰ��ӿ��� ���Ǵ� �÷��̾� ��Ʈ�ѷ�
UCLASS(Blueprintable)
class DEFENDER_API ADAPC_InGame : public ADA_PlayerController
{
	GENERATED_BODY()
public:

protected:

private:
	//The class of the widget that will always be displayed during the game.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "Widget", meta = (AllowPrivateAccess = true))
	TSubclassOf<UDWP_MainWidget> mainWidgetClass;

	//A class of widget that contains player information.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "Widget", meta = (AllowPrivateAccess = true))
	TSubclassOf<UDWP_Infomation> informationWidgetClass;

	//Class of widget to be displayed when clearing the stage
	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "Widget", meta = (AllowPrivateAccess = true))
	TSubclassOf<UDWP_StageClear> stageClearWidgetClass;

	//The class of the widget that will be displayed when the stage starts.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "Widget", meta = (AllowPrivateAccess = true))
	TSubclassOf<UDWP_StageStart> stageStartWidgetClass;

	//Store widget class
	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "Widget", meta = (AllowPrivateAccess = true))
	TSubclassOf<UDWP_Shop> shopWidgetClass;

	//The class of the widget that will be displayed when indicating that interaction is possible.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "Widget", meta = (AllowPrivateAccess = true))
	TSubclassOf<UDW_PlayerWidget> interactionWidgetClass;

	//Class of the return widget
	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "Widget", meta = (AllowPrivateAccess = true))
	TSubclassOf<UDWP_ReturnMenu> returnMenuWidgetClass;

	//Class of the widget that will be displayed when the player dies
	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "Widget", meta = (AllowPrivateAccess = true))
	TSubclassOf<UDWP_Defeated> defeatedWidgetClass;

	UPROPERTY()
	UDWP_MainWidget*		mainWidget;						//�������� �ν��Ͻ�

	UPROPERTY()
	UDWP_Infomation*		informationWidget;				//����â ���� �ν��Ͻ�

	UPROPERTY()
	UDW_PlayerWidget*		interactionWidget;				//��ȣ�ۿ� ���� �ν��Ͻ�

	int32					interactionOverlappedCount;		//��ȣ�ۿ��� �������� Ƚ��

	UPROPERTY()
	UDWP_Shop*				shopWidget;						//���� ������ �ν��Ͻ�

	UPROPERTY()
	UDWP_StageClear*		stageClearWidget;				//�������� Ŭ���� ������ �ν��Ͻ�

	UPROPERTY()
	UDWP_StageStart*		stageStartWidget;				//�������� ���� ������ �ν��Ͻ�

	UPROPERTY()
	UDWP_ReturnMenu*		returnMenuWidget;				//���ư���޴� ������ �ν��Ͻ�

	UPROPERTY()
	UDWP_Defeated*			defeatedWidget;					//�й� ������ �ν��Ͻ�

	FTimerHandle			shopTimerHandle;				//������ ���ѽð� Ÿ�̸� �ڵ�

public:
	ADAPC_InGame();

	/*
	* Create basic widgets used by Defender
	*/
	void CreateInGameWidgets();

	/*
	* Delete mainwidget
	*/
	void RemoveMainWidget();

	/*
	* Display the main widget
	*/
	UFUNCTION(BlueprintCallable, category = "Shop")
	void ShowMainWidget();

	/*
	* Display the store widget
	* @param[in] _TimeLimit The store's time limit. If 0.f, it lasts until the player closes it.
	* @return Returns the displayed store widget
	*/
	UFUNCTION(BlueprintCallable, category = "Shop")
	UDWP_Shop* ShowShopWidget(float _TimeLimit = 0.f);

	/*
	* Display interactive widgets
	*/
	void ShowInteractionWidget();
	/*
	* Removes the interaction widget. 
	* If any actors are still interacting with them, they will remain visible.
	*/
	void RemoveInteractionWidget();
	/*
	* The stage displays a start widget
	*/
	UFUNCTION(BlueprintCallable, category = "Stage Widget")
	void ShowStageStartWidget();
	/*
	* The stage displays a clear widget
	*/
	UFUNCTION(BlueprintCallable, category = "Stage Widget")
	void ShowStageClearWidget();
	/*
	* Convert player data to json format.
	* @return Retruns FString
	*/
	virtual FString SaveJson()override;
	/*
	* Apply json data to the player
	* @param[in] _JsonStr json to apply
	*/
	bool LoadJson(const FString& _JsonStr);
	/*
	* Get data from the previous level.
	* If there is no data from the previous level, create new data.
	*/
	void SetPrevLevelData();

	/*
	* Save the player's data in json format.
	*/
	UFUNCTION(BlueprintCallable, category = "GameIO")
	bool SaveGame();
	/*
	* Get the player's store widget
	* @return Returns UDWP_Shop*
	*/
	FORCEINLINE UDWP_Shop* GetShopWidget()const { return shopWidget; }
	/*
	* Get the player's main widget
	* @return Returns UDWP_MainWidget*
	*/
	FORCEINLINE UDWP_MainWidget* GetMainWidget()const { return mainWidget; }
	/*
	* shows defeat widget
	*/
	UFUNCTION()
	void ShowDefeatedWidget();

protected:
	virtual void BeginPlay()override;
	virtual void SetupInputComponent()override;

private:
	//����ȣ�� �Լ�

	void ShowInformationWidget();

	void ShowReturnMenu();

	UFUNCTION()
	void ShopClosed();

	UFUNCTION()
	void CloseLoadingScreen();
};
