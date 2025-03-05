#pragma once

#include "Component/DC_PawnComponent.h"
#include "DC_Weapon.generated.h"

#define CURRENT_WEAPON weaponDataArray[currentWeaponIndex]
#define CURRENT_WEAPON_NAME weaponDataArray[currentWeaponIndex]->weaponName

class UDWP_Scope;
class UDW_PlayerWidget;
class ADA_Player;
class ADAPC_InGame;
class UDC_Stat;
class UDC_ProjectileManager;
class UParticleSystem;

struct FStreamableHandle;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWeaponChangeDelegate, const FString&, _WeaponName, EWEAPON_OWNED_STATE, _WeaponOwnedState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMagzineChangeDelegate, const FWeaponOwnedData&, _WeaponData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFireSelectorChangeDelegate, EWEAPON_FIRE_SELECTOR, _CurrentMode);

//플레이어의 무기를 관리하는 컴포넌트
UCLASS(ClassGroup = (DC_Player), meta = (BlueprintSpawnableComponent))
class DEFENDER_API UDC_Weapon : public UDC_PawnComponent
{
	GENERATED_BODY()
public:
	//Delegate called when the weapon changes
	UPROPERTY(BlueprintAssignable)
	FOnWeaponChangeDelegate	onWeaponChange;

	//Delegate called when magazine information changes
	UPROPERTY(BlueprintAssignable)
	FOnMagzineChangeDelegate onMagazineChange;

	//Delegate called when FireSelector changes
	UPROPERTY(BlueprintAssignable)
	FOnFireSelectorChangeDelegate onFireSelectorChange;

protected:

private:
	//The name of the socket the weapon will attach to
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "OwnerConfig", meta = (AllowPrivateAccess))
	FName attachSocketName;

	//Pointer to the skeletal mesh created by the host
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "OwnerConfig", meta = (AllowPrivateAccess))
	USkeletalMeshComponent* weaponSkeletalComponent;

	//Data table containing weapon information
	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "WaeponData", meta = (AllowPrivateAccess))
	UDataTable* weaponDataTable;

	//Map showing ownership information of weapons
	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "WeaponData", meta = (AllowPrivateAccess))
	TMap<FString, FWeaponOwnedData> weaponMap;

	//A class of widgets representing the crosshairs.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "WeaponData", meta = (AllowPrivateAccess))
	TSubclassOf<UDW_PlayerWidget> crossHairWidget;

	//Effect that appears when a bullet hits
	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "WeaponData", meta = (AllowPrivateAccess))
	UParticleSystem* sparkEffect;

	UPROPERTY()
	UDWP_Scope*						currentScopeWidget;				//현재 무기의 스코프
	UPROPERTY()
	UDW_PlayerWidget*				currentCrossHairWidget;			//현재 조준점
	
	ADA_Player*						ownerPlayer;					//현재 플레이어 캐릭터
	ADAPC_InGame*					ownerPlayerController;			//현재 플레이어 컨트롤러

	TArray<FDFWeaponData*>			weaponDataArray;				//현재 무기의 데이터 테이블 행
	float							weaponCoolDown;					//현재 무기의 공격 쿨타임
	int32							currentWeaponIndex;				//현재 무기의 인덱스

	FVector							lastStartLocation;				//람다호출을 위한 변수백업
	FVector							lastEndLocation;				//람다호출을 위한 변수백업
	bool							bLastZooming;					//람다호출을 위한 변수백업

	TSharedPtr<FStreamableHandle>	assetStreamingHandle;			//FStreamableManager로 불러온 핸들을 저장
	FSoftObjectPath					weaponAssetToLoad;				//불러올 무기 에셋의 경로를 저장
	UDC_Stat*						statComp;						//플레이어의 스탯컴포넌트
	UDC_ProjectileManager*			characterProjectileManager;		//플레이어의 프로젝타일 매니저
	UDOA_Player*					currentAnimInstance;			//현재 플레이어의 애님인스턴스

public:
	UDC_Weapon();
	/*
	* Zoom in. Bullet accuracy increases
	*/
	void ZoomIn();
	/*
	* Zoom out. Bullet accuracy is low
	*/
	void ZoomOut();
	/*
	* Changes the current weapon to the name of the skeletal mesh
	* @param[in] _WeaponName The name of the skeletal mesh of the weapon to change
	* @return Returns whether weapon replacement was successful or not
	*/
	UFUNCTION(BlueprintCallable, category = "WeaponSetting")
	bool SetWeapon(const FString& _WeaponName);
	/*
	* Changes the attack state of the current weapon in the order of semi, burst, and auto.
	* In the data table, if the bool value of burst and auto is false, that value is skipped.
	*/
	UFUNCTION(BlueprintCallable, category = "WeaponSetting")
	void FireModeChange();
	/*
	* Reloads the current weapon
	*/
	UFUNCTION(BlueprintCallable, category = "WeaponComp")
	void Reload();
	/*
	* Attack with current weapon
	* @param[in] _FireLocation Weapon firing starting point
	* @param[in] _TracingEndLocation The point at which the weapon fires
	* @param[in] _bIsZooming Zoom status
	*/
	UFUNCTION(BlueprintCallable, category = "WeaponComp")
	void Fire(FVector _FireLocation, FVector _TracingEndLocation, bool _bIsZooming);
	/*
	* Drop the specified weapon
	* @param[in] _WeaponName Name of weapon to be droped
	*/
	UFUNCTION(BlueprintCallable, category = "WeaponComp")
	bool DropWeapon(const FString& _WeaponName);
	/*
	* acquire a weapon
	* @param[in] _WeaponName Name of weapon to be acquired
	*/
	UFUNCTION(BlueprintCallable, category = "WeaponComp")
	bool AddWeapon(const FString& _WeaponName);
	/*
	* Provides magazines to weapons
	* param[in] _WeaponName Name of the weapon from which the magazine will be fed
	* param[in] _BulletCount Number of magazines
	*/
	UFUNCTION(BlueprintCallable, category = "WeaponComp")
	bool AddMagazine(const FString& _WeaponName, int32 _BulletCount);
	/*
	* Modify the weapon's Owned status, current magazine, total magazine, etc.
	* param[in] _WeaponName Name of the weapon to be modified
	* param[in] _WeaopnOwnedState Owned status to be modified
	*/
	void SetOwnedState(const FString& _WeaponName, const FWeaponOwnedData& _WeaopnOwnedState);
	/*
	* Change weapons in order
	*/
	void WeaponIndexUp();
	/*
	* Change weapons in order
	*/
	void WeaponIndexDown();
	/*
	* Get comprehensive data on your current weapon
	* @return Returns const FWeaponOwnedData&
	*/
	UFUNCTION(BlueprintCallable, category = "WeaponComp")
	const FWeaponOwnedData& GetCurrentWeaponData()const { return weaponMap[CURRENT_WEAPON_NAME]; }
	/*
	* Gets the current weapon's skeletalMesh
	* @return Return the USkeletalMeshComponent*
	*/
	FORCEINLINE USkeletalMeshComponent* GetCurrentWeaponSkeletalMeshComponent()const { return weaponSkeletalComponent; }
	/*
	* Get all the possession status and data of the weapons you can own
	* @return Returns TArray<FWeaponOwnedData>
	*/
	FORCEINLINE TArray<FWeaponOwnedData> GetAllWeaponData()const;
	/*
	* Get WeaponMap
	* @return return the TMap<FString, FWeaponOwnedData>
	*/
	FORCEINLINE TMap<FString, FWeaponOwnedData> GetWeaponMap()const { return weaponMap; }
	/*
	* Called when starting over without loading data.
	*/
	void NewStartSetting();
	/*
	* Set up the player controller
	* @param[in] _NewPlayerController Player controller to set
	*/
	void SetPlayerController(ADAPC_InGame* _NewPlayerController);

protected:
	virtual void InitializeComponent()override;
	virtual void BeginPlay() override;
	virtual void TickComponent(float _DeltaTime, ELevelTick _TickType, FActorComponentTickFunction* _ThisTickFunction)override;

private:
	//내부호출 함수

	UFUNCTION()
	void ChangeAnimInstance(UDO_AnimInstance* _AnimInstance);

	UFUNCTION()
	void StateChange(ECHARACTER_STATE _CurrentState);

	UFUNCTION()
	void ReloadEnd();

	void SetFireSelectorNextMode();
	void TracingAttack(FVector _StartLocation, FVector _EndLocation, bool _bIsZooming);
	int32 CheckIndexRange(int32 _Index);
	void CalculateMagazine(const FString& _CurrentWeaponName);
	void OnAssetLoadCompleted();
	void RemoveScopeWidget();
};
