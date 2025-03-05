#pragma once

#include "Actor/DA_Character.h"
#include "DA_Player.generated.h"

class UDC_Inventory;
class UDC_Targeting;
class UDC_Weapon;
class UDC_Action;

class UInputComponent;
class USpringArmComponent;
class UCameraComponent;

struct FStreamableHandle;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterChangeDelegate, UTexture2D*, _CharacterIcon);

//플레이어가 조종하는 캐릭터의 최상위 클래스
UCLASS(Blueprintable)
class DEFENDER_API ADA_Player : public ADA_Character
{
	GENERATED_BODY()
public:
	//Delegate called when the character changes
	UPROPERTY(BlueprintAssignable)
	FOnCharacterChangeDelegate onCharacterChange;

	//Delegate called when the asset has finished loading
	UPROPERTY(BlueprintAssignable)
	FOnAssetLoadCompletedDelegate onAssetLoadCompleted;

protected:

private:
	//Component to position the camera away from the player
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* cameraSpringArm;

	//The camera through which the player will actually see the game world
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* playerCamera;

	//Component that support the inventory function
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerDefaultConfig", meta = (AllowPrivateAccess = "true"))
	UDC_Inventory* inventoryComp;

	//Component that support the lock - on
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerDefaultConfig", meta = (AllowPrivateAccess = "true"))
	UDC_Targeting* targetingComp;

	//A component that manages weapons
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerDefaultConfig", meta = (AllowPrivateAccess = "true"))
	UDC_Weapon* weaponComp;

	//Components that support player actions(parkour, movement, jumping, etc.)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerDefaultConfig", meta = (AllowPrivateAccess = "true"))
	UDC_Action* actionComp;

	//Player character index
	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "PlayerCharacter", meta = (AllowPrivateAccess = true))
	int32 playerCharacterIndex;

	//Data table containing character data
	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "PlayerCharacter", meta = (AllowPrivateAccess = true))
	UDataTable* playerDataTable;

	TSharedPtr<FStreamableHandle>	assetStreamingHandle;		//FStreamableManager로 불러온 핸들을 저장
	TArray<FSoftObjectPath>			characterAssetsToLoad;		//불러올 에셋들의 경로를 저장
	FDFPlayerData*					playerData;					//데이터 테이블에서 불러온 캐릭터 정보

public:
	ADA_Player();
	/*
	* Return the character's spring arm
	* @return Return the character's USpringArmComponent*
	*/
	FORCEINLINE USpringArmComponent* GetCameraSpringArmComponent() const { return cameraSpringArm; }
	/*
	* Return the character's camera
	* @return Return the character's UCameraComponent*
	*/
	FORCEINLINE UCameraComponent* GetPlayerCameraComponent() const { return playerCamera; }
	/*
	* Return the character's inventory component
	* @return Return the character's UDC_Inventory*
	*/
	FORCEINLINE UDC_Inventory* GetInventoryComponent() const { return inventoryComp; }
	/*
	* Return the character's targeting component
	* @return Return the character's UDC_Targeting*
	*/
	FORCEINLINE UDC_Targeting* GetTargetingComponent() const { return targetingComp; }
	/*
	* Return the character's weapon component
	* @return Return the character's UDC_Weapon*
	*/
	FORCEINLINE UDC_Weapon* GetWeaponComponent() const { return weaponComp; }
	/*
	* Return the character's action component
	* @return Return the character's UDC_Action*
	*/
	FORCEINLINE UDC_Action* GetActionComponent() const { return actionComp; }
	/*
	* Get the index of the current character
	* @return Returns the int32
	*/
	FORCEINLINE int32 GetPlayerCharacterIndex()const { return playerCharacterIndex; }
	/*
	* Sets the character according to the character's index.
	* @param[in] _CharacterIndex Character index to set
	*/
	UFUNCTION(BlueprintCallable, category = "Set Character")
	/*
	* Set up the player character
	* @param[in] _CharacterIndex Index of character to set
	*/
	void SetPlayerCharacter(int32 _CharacterIndex);
	/*
	* Get the character's data
	* @return Returns FDFPlayerData*
	*/
	FORCEINLINE FDFPlayerData* GetPlayerData()const { return playerData; }

protected:
	virtual void SetupPlayerInputComponent(UInputComponent* _PlayerInputComponent) override;
	virtual void CharacterInitialize()override;
	virtual void PossessedBy(AController* _NewController);

private:
	void OnAssetLoadCompleted();

};
