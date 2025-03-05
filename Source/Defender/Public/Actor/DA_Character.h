#pragma once

#include "../Defender.h"
#include "GameFramework/Character.h"
#include "DA_Character.generated.h"

class UDC_Stat;
class UDC_AnimMontage;
class UDC_Skill;
class UDC_Minimap;
class UDWP_FloatingDamage;
class UDC_ProjectileManager;

class AController;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnApplyDamageDelegate, float, _Damage, AActor*, _DamageCauser, AController*, _DamageInstigator);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAssetLoadCompletedDelegate);

//프로젝트에서 사용하는 캐릭터의 최상위 클래스
UCLASS(Abstract, NotBlueprintable)
class DEFENDER_API ADA_Character : public ACharacter
{
	GENERATED_BODY()

public:
	//Delegate called when the take damage
	UPROPERTY(BlueprintAssignable)
	FOnApplyDamageDelegate onApplyDamage;

protected:
	//character's initial level
	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "Setting", meta = (AllowPrivateAccess = true))
	int32 characterStartLevel;

private:
	//Manage the AnimMontage a character can have
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "CharacterDefaultConfig", meta = (AllowPrivateAccess = true))
	UDC_AnimMontage* animMontageComp;

	//Components that manage skills
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "CharacterDefaultConfig", meta = (AllowPrivateAccess = true))
	UDC_Skill* skillComp;

	//Manage the character's stats (e.g. health, movement speed, level, etc.)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "CharacterDefaultConfig", meta = (AllowPrivateAccess = true))
	UDC_Stat* statComp;

	//Used to display the location on the minimap.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "CharacterDefaultConfig", meta = (AllowPrivateAccess = true))
	UDC_Minimap* minimapComp;

	//Manage the projectile a character can use
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "CharacterDefaultConfig", meta = (AllowPrivateAccess = true))
	UDC_ProjectileManager* projectileManagerComp;

	//Widget class that will display damage text
	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "WidgetClass", meta = (AllowPrivateAccess = true))
	TSubclassOf<UDWP_FloatingDamage> floatingDamageWidgetClass;

public:
	ADA_Character();
	/*
	* Get the character's skill component
	* @return Return the character's UDC_Skill*
	*/
	FORCEINLINE UDC_Skill* GetSkillComponent()const { return skillComp; }
	/*
	* Get the character's stat component
	* @return Return the character's UDC_Stat*
	*/
	FORCEINLINE UDC_Stat* GetStatComponent()const { return statComp; }
	/*
	* Get the character's montage component
	* @return Return the character's UDC_AnimMontage*
	*/
	FORCEINLINE UDC_AnimMontage* GetAnimMontageComponent()const { return animMontageComp; }
	/*
	* Get the character's projectile manager component
	* @return Return the character's UDC_Skill*
	*/
	FORCEINLINE UDC_ProjectileManager* GetProjectileManagerComponent()const { return projectileManagerComp; }
	/*
	* Get the minimap component
	* @return Return the character's UDC_Minimap*
	*/
	FORCEINLINE UDC_Minimap* GetMinimapComponent()const { return minimapComp; }
	/*
	* The amount of damage actually applied
	* @param[in] _DamageAmount 	How much damage to apply
	* @param[in] _DamageInstigator The Controller responsible for the damage
	* @param[in] _DamageCauser The Actor that directly caused the damage (e.g. the projectile that exploded, the rock that landed on you)
	*/
	UFUNCTION(BlueprintCallable, category = "CharacterDamage")
	virtual void ApplyDamage(float _DamageAmount, AActor* _DamageCauser, AController* _DamageInstigator, FVector _HitPoint = FVector::ZeroVector);
	/*
	* Set the character's starting level
	* @param[in] _StartLevel Starting level to set
	*/
	void SetStartLevel(int32 _StartLevel);

	UFUNCTION(BlueprintCallable, category = "DFCharacter")
	bool GetFloorLocation(FVector& _OutFloorLocation);

protected:
	virtual void BeginPlay() override;
	virtual void CharacterInitialize();

private:

};
