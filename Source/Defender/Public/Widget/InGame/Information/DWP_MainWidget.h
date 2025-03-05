#pragma once

#include "Widget/DW_PlayerWidget.h"
#include "DWP_MainWidget.generated.h"

class UDWP_SkillSlot;

class UProgressBar;
class UImage;
class UTextBlock;

//플레이어의 상태와 게임의 정보를 담고있는 핵심위젯
UCLASS()
class DEFENDER_API UDWP_MainWidget : public UDW_PlayerWidget
{
	GENERATED_BODY()

public:

protected:

private:
	//Progress bar showing HP
	UPROPERTY(BlueprintReadOnly, category = "Character Info", meta = (BindWidget, AllowPrivateAccess = true))
	UProgressBar* pb_HPBar;

	//Progress bar showing EXP
	UPROPERTY(BlueprintReadOnly, category = "Character Info", meta = (BindWidget, AllowPrivateAccess = true))
	UProgressBar* pb_EXPBar;

	//Displays the character's profile picture
	UPROPERTY(BlueprintReadOnly, category = "Character Info", meta = (BindWidget, AllowPrivateAccess = true))
	UImage* img_CharacterImage;

	UPROPERTY(BlueprintReadOnly, category = "Character Info", meta = (BindWidget, AllowPrivateAccess = true))
	UImage* img_FireSelector;

	//Widget that displays the minimap.
	UPROPERTY(BlueprintReadOnly, category = "Minimap", meta = (BindWidget, AllowPrivateAccess = true))
	UImage* img_Minimap;

	//Widget that displays the current weapon's magazine
	UPROPERTY(BlueprintReadOnly, category = "Character Info", meta = (BindWidget, AllowPrivateAccess = true))
	UTextBlock* txt_Magazine;

	//Widget that displays the player's name
	UPROPERTY(BlueprintReadOnly, category = "Character Info", meta = (BindWidget, AllowPrivateAccess = true))
	UTextBlock* txt_Name;

	//Widget that displays the player's level
	UPROPERTY(BlueprintReadOnly, category = "Character Info", meta = (BindWidget, AllowPrivateAccess = true))
	UTextBlock* txt_Level;

	//Background widgets for widgets containing skill images
	UPROPERTY(BlueprintReadOnly, category = "Skill", meta = (BindWidget, AllowPrivateAccess = true))
	UImage* img_SkillBackground;

	//Widget that displays the highest score
	UPROPERTY(BlueprintReadOnly, category = "Game Info", meta = (BindWidget, AllowPrivateAccess = true))
	UTextBlock* txt_HighScore;

	//Widget that displays the current score
	UPROPERTY(BlueprintReadOnly, category = "Game Info", meta = (BindWidget, AllowPrivateAccess = true))
	UTextBlock* txt_CurrentScore;

	//Widget that displays the time limit for the current level
	UPROPERTY(BlueprintReadOnly, category = "Game Info", meta = (BindWidget, AllowPrivateAccess = true))
	UTextBlock* txt_Timer;

	//Widget containing data on skills that players can use
	UPROPERTY(BlueprintReadOnly, category = "SkillSlot", meta = (BindWidget, AllowPrivateAccess = true))
	UDWP_SkillSlot* WG_SkillSlot_1;

	//Widget containing data on skills that players can use
	UPROPERTY(BlueprintReadOnly, category = "SkillSlot", meta = (BindWidget, AllowPrivateAccess = true))
	UDWP_SkillSlot* WG_SkillSlot_2;

	//Widget containing data on skills that players can use
	UPROPERTY(BlueprintReadOnly, category = "SkillSlot", meta = (BindWidget, AllowPrivateAccess = true))
	UDWP_SkillSlot* WG_SkillSlot_3;

	UPROPERTY(BlueprintReadOnly, category = "SkillSlot", meta = (BindWidget, AllowPrivateAccess = true))
	UImage* img_Passive;

	//image of potion
	UPROPERTY(BlueprintReadOnly, category = "Potion", meta = (BindWidget, AllowPrivateAccess = true))
	UImage* img_Potion;

	//Number of potions
	UPROPERTY(BlueprintReadOnly, category = "Potion", meta = (BindWidget, AllowPrivateAccess = true))
	UTextBlock* txt_potionCount;

	//Name of the potion
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "Potion", meta = (AllowPrivateAccess = true))
	FString potionName;

	//Name of currently equipped weapon
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "Weapon", meta = (AllowPrivateAccess = true))
	FString weaponName;

	TArray<UDWP_SkillSlot*>			skillSlots{};		//스킬 위젯을 담아놓은 배열

public:
	UDWP_MainWidget(const FObjectInitializer& _ObjectInitializer);
	/*
	* Initialize widgets before adding them to the viewport.
	*/
	virtual void InitializeWidget()override;
	/*
	* Set up the HP Progress Bar
	* param[in] _MaxHP Max HP
	* param[in] _CurrentHP Current HP
	*/
	UFUNCTION()
	void SetHPBar(float _MaxHP, float _CurrentHP);
	/*
	* Set up the EXP Progress Bar
	* param[in] _TotalEXP Amount of experience required to reach the next level
	* param[in] _CurrentExp Current EXP
	*/
	UFUNCTION()
	void SetEXPBar(int32 _TotalEXP, int32 _CurrentExp);
	/*
	* Set up the player level
	* param[in] _Level level to set
	*/
	UFUNCTION()
	void SetLevel(int32 _Level);
	/*
	* Sets the player's name
	* @param[in] _NewName player's name
	*/
	UFUNCTION()
	void SetPlayerName(const FString& _NewName);
	void SetLevelTimer(float _NewTime);

protected:
	virtual void NativeConstruct()override;
	virtual void NativeDestruct()override;

private:
	//내부 호출 함수

	UFUNCTION()
	void SetMaganize(const FWeaponOwnedData& _WeaponData);

	UFUNCTION()
	void RegistPotion(const FString& _ItemName, UTexture2D* _ItemImage, int32 _ItemCount);

	UFUNCTION()
	void SetPotionCount(const FString& _ItemName, int32 _TotalItemCount);

	UFUNCTION()
	void SetCurrentWeapon(const FString& _WeaponName, EWEAPON_OWNED_STATE _WeaponOwnedState);

	UFUNCTION()
	void SetScore(int32 _Current, int32 _High);

	UFUNCTION()
	void SetPassiveIcon(const FString& _Key, UDO_PassiveBase* _Skill, bool bIsRegist);

	UFUNCTION()
	void SetCharacterImage(UTexture2D* _CharacterIcon);

	UFUNCTION()
	void SetFireSelector(EWEAPON_FIRE_SELECTOR _CurrentMode);

	FText SetTimerText(float _Time);
};
