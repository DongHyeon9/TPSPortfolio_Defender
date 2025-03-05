#pragma once

#include "Actor/Player/DA_PlayerController.h"
#include "DAPC_Select.generated.h"

class UDWP_StartSkill;
class UDWP_StartMain;
class UDWP_StartLoad;
class UDWP_StartCharacter;
class UDW_PlayerWidget;
class UDWP_PopupBox;

//캐릭터선택, 스킬선택, 불러오기 화면에서 사용되는 플레이어 컨트롤러
UCLASS(Blueprintable)
class DEFENDER_API ADAPC_Select : public ADA_PlayerController
{
	GENERATED_BODY()
public:

protected:

private:
	//The name of the map that will be launched once the basic setup is complete.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "Widget", meta = (AllowPrivateAccess = true))
	FName beginStageName;

	//Widgets that appear first on the start screen
	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "Widget", meta = (AllowPrivateAccess = true))
	TSubclassOf<UDWP_StartMain> startMainWidgetClass;

	//Widgets that can be loaded
	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "Widget", meta = (AllowPrivateAccess = true))
	TSubclassOf<UDWP_StartLoad> startLoadWidgetClass;

	//A class of widget that allows you to select a character
	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "Widget", meta = (AllowPrivateAccess = true))
	TSubclassOf<UDWP_StartCharacter> characterSelectWidgetClass;

	//A class of widgets that allow you to select skills
	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "Widget", meta = (AllowPrivateAccess = true))
	TSubclassOf<UDWP_StartSkill> skillSelectWidgetClass;

	//A class of widgets that will notify when the player does something wrong.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "PopupBoxWidget", meta = (AllowPrivateAccess = true))
	TSubclassOf<UDWP_PopupBox> popupBoxWidgetClass;

	//Name entered by player
	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "Player Data", meta = (AllowPrivateAccess = true))
	FString playerName;

	//Index of the character selected by the player
	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "Player Data", meta = (AllowPrivateAccess = true))
	int32 characterIndex;

	//Active skills selected by the player
	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "Player Data", meta = (AllowPrivateAccess = true))
	TArray<FString> activeSkills;

	//Passive skill selected by the player
	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "Player Data", meta = (AllowPrivateAccess = true))
	FString passiveSkill;

	UPROPERTY()
	UDWP_StartMain*			startMainWidget;			//선택레벨 메인위젯의 인스턴스

	UPROPERTY()
	UDWP_StartLoad*			startLoadMainWidget;		//로드메뉴 위젯의 인스턴스

	UPROPERTY()
	UDWP_StartCharacter*	characterSelectWidget;		//캐릭터 선택메뉴 위젯의 인스턴스

	UPROPERTY()
	UDWP_StartSkill*		skillSelectWidget;			//스킬 선택메뉴 위젯의 인스턴스

	UPROPERTY()
	UDWP_PopupBox*			popupBoxWidget;				//안내메시지박스 위젯의 인스턴스

	UPROPERTY()
	UDW_PlayerWidget*		currentShowWidget;			//현재 보여지고있는 위젯

	FString					playerData;					//플레이가 설정한 데이터

public:
	ADAPC_Select();
	/*
	* Convert player data to json format.
	* @return Retruns FString
	*/
	virtual FString SaveJson()override;
	/*
	* Load saved player data
	* @param[in] _PlayerName The name of the player whose data you want to retrieve
	* @return Returns false if there is no data with the player name.
	*/
	UFUNCTION(BlueprintCallable, category = "GameIO")
	bool LoadGame(const FString& _PlayerName);
	/*
	* Shows the main widget of the selection screen
	*/
	UFUNCTION()
	void ShowMainWidget();
	/*
	* Shows the loading widget on the selection screen
	*/
	UFUNCTION()
	void ShowLoadWidget();
	/*
	* Shows the character selection widget on the selection screen.
	*/
	UFUNCTION()
	void ShowCharacterSelectWidget();
	/*
	* Shows the skill selection widget on the selection screen
	*/
	UFUNCTION()
	void ShowSkillSelectWidget();
	/*
	* Reset selected skills
	*/
	void ResetSkills();
	/*
	* Display a notification box
	* @param[in] _NewText Text to be displayed in the notification box
	*/
	void ShowPopupBoxWidget(const FText& _NewText);
	/*
	* Start the game
	*/
	void GameStart();
	/*
	* Set the player's name
	* @param[in] _PlayerName Player name to set
	*/
	void SetPlayerName(const FString& _PlayerName);
	/*
	* Sets the index of the player character
	* @param[in] _Index index to set
	*/
	void SetCharacterIndex(int32 _Index);
	/*
	* Set active skills
	* @param[in] _SkillName Name of skill to set
	* @return Index of set skill
	*/
	int32 SetActiveSkill(const FString& _SkillName);
	/*
	* Delete active skill
	* @param[in] _SkillName Name of skill to delete
	*/
	void RemoveActiveSkill(const FString& _SkillName);
	/*
	* Delete passive skill
	* @param[in] _SkillName Passive skills to delete
	*/
	void RemovePassiveSkill(const FString& _SkillName);
	/*
	* Set passive skills
	* @param[in] _SkillName Name of passive to set
	*/
	void SetPassiveSkill(const FString& _SkillName);
	/*
	* Make sure all skills are set up
	* @return Returns true if all skills are set
	*/
	bool IsAllCheckedSkills();

protected:
	virtual void BeginPlay()override;

private:
	void ChangeCurrentWidget(UDW_PlayerWidget* _NewWidget);

};
