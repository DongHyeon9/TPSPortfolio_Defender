#pragma once

#include "CoreMinimal.h"
#include "DF_Enum.h"
#include "Engine/DataTable.h"
#include "DF_Struct.generated.h"

class IDI_ObjectPoolingActor;
class ADA_Item;
class UDOA_Player;
class UDWP_Scope;
class ADA_Projectile;
class ADA_Character;
class UDO_ActiveBase;
class UDO_PassiveBase;
class UDO_AnimInstance;
class UDW_MonsterHP;

class UBehaviorTree;
class UBlackboardData;
class UStaticMesh;
class UTexture2D;
class UNiagaraSystem;

USTRUCT(BlueprintType, Atomic)
struct FProjectileInitializer {
	GENERATED_USTRUCT_BODY()

	FProjectileInitializer() :
		projectileType(EPROJETILE_TYPE::STRAIGHT),
		mesh(),
		ownerCharacter(),
		damage(),
		damageRadius(50.f),
		meshRelativeTransform(FRotator(), FVector(), FVector(1.f)),
		ignoreTags(),
		lifeTime(5.f),
		initializeSpeed(),
		maxSpeed(),
		gravityScale(),
		sphereRadius(30.f),
		tailEffect(),
		tailEffectSize(1.f),
		finishEffect(),
		finishEffectSize(1.f)
	{}

	virtual ~FProjectileInitializer() {}
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Projectile Setting")
	EPROJETILE_TYPE projectileType;
	virtual void ApplySetting(ADA_Projectile* _Projectile)const;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Projectile Setting")
	UStaticMesh* mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Projectile Setting")
	ADA_Character* ownerCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Projectile Setting")
	float damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Projectile Setting")
	float damageRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Projectile Setting")
	FTransform meshRelativeTransform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Projectile Setting")
	TArray<FName> ignoreTags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Projectile Setting")
	float lifeTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Projectile Setting")
	float initializeSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Projectile Setting")
	float maxSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Projectile Setting")
	float gravityScale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Projectile Setting")
	float sphereRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Projectile Setting")
	UNiagaraSystem* tailEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Projectile Setting")
	float tailEffectSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Projectile Setting")
	UNiagaraSystem* finishEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Projectile Setting")
	float finishEffectSize;

	friend class ADA_Projectile;
};

USTRUCT(BlueprintType, Atomic)
struct FHomingInitializer : public FProjectileInitializer {
	GENERATED_USTRUCT_BODY()

	FHomingInitializer() :
		floatCurve(),
		targetActor(),
		targetLocation(),
		homingDelay(),
		homingSpeed()
	{
		projectileType = EPROJETILE_TYPE::HOMING;
		lifeTime = 0.f;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "homing Setting")
	UCurveFloat* floatCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "homing Setting")
	AActor* targetActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "homing Setting")
	FVector targetLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "homing Setting")
	float homingDelay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "homing Setting")
	float homingSpeed;

protected:
	virtual void ApplySetting(ADA_Projectile* _Projectile)const override;

	friend class ADA_Projectile;
};

USTRUCT(Atomic, BlueprintType)
struct FWeaponIK
{
	GENERATED_USTRUCT_BODY()
public:
	FWeaponIK() :
		leftHand_Effector(),
		leftHand_JointTarget(),
		rightHand_Effector(),
		rightHand_JointTarget() {}
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Weapon Infomation")
	FVector leftHand_Effector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Weapon Infomation")
	FVector leftHand_JointTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Weapon Infomation")
	FVector rightHand_Effector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Weapon Infomation")
	FVector rightHand_JointTarget;
};

USTRUCT(Atomic, BlueprintType)
struct FFireSelector
{
	GENERATED_USTRUCT_BODY()
public:
	FFireSelector() :
		burstMode(),
		autoMode() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Weapon Infomation")
	bool burstMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Weapon Infomation")
	bool autoMode;
};

USTRUCT(Atomic, BlueprintType)
struct FCharacterStat
{
	GENERATED_USTRUCT_BODY()
public:
	FCharacterStat() :
		characterLevel(),
		currentExp(),
		currentHP(),
		additionMoveSpeed(),
		additionAttckDamage()
	{}
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Character Level")
	int32 characterLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Character Level")
	int32 currentExp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Character HP")
	float currentHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Character Movement")
	TMap<FString, float> additionMoveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Character Damage")
	TMap<FString, float> additionAttckDamage;
};

USTRUCT(BlueprintType, Atomic)
struct FDFCharacterData :public FTableRowBase {
	GENERATED_USTRUCT_BODY()

public:
	FDFCharacterData() :
		characterHP(),
		attackDamage(),
		moveSpeed(),
		dropExp(),
		nextExp(),
		score()
	{}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Character Data")
	float characterHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Character Data")
	float attackDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Character Data")
	float moveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Character Data")
	int32 dropExp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Character Data")
	int32 nextExp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Character Data")
	int32 dropGold;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Character Data")
	int32 score;
};

USTRUCT(BlueprintType, Atomic)
struct FDFWeaponData : public FTableRowBase {
	GENERATED_USTRUCT_BODY()

public:
	FDFWeaponData() :
		weaponName(),
		weaponImage(),
		weaponSkeletalMesh(),
		weaponDamage(),
		attackInterval(),
		maxRecoil(),
		minRecoil(),
		soundRange(20.f),
		loudness(50.f),
		maganize(),
		weaponPrice(),
		magazinePrice(),
		weaponAttackType(),
		normalState(),
		zoomState(),
		fireSelectorSetting(),
		reloadAnimation(),
		fireAnimation()
	{}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Weapon Infomation")
	FString weaponName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Weapon Infomation")
	UTexture2D* weaponImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Weapon Infomation")
	TSoftObjectPtr<USkeletalMesh> weaponSkeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Weapon Infomation")
	float weaponDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Weapon Infomation")
	float attackInterval;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Weapon Infomation")
	float maxRecoil;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Weapon Infomation")
	float minRecoil;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Weapon Infomation")
	float soundRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Weapon Infomation")
	float loudness;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Weapon Infomation")
	int32 maganize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Weapon Infomation")
	int32 weaponPrice;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Weapon Infomation")
	int32 magazinePrice;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Weapon Infomation")
	EWEAPON_ATTACK_TYPE weaponAttackType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Weapon Infomation")
	FWeaponIK normalState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Weapon Infomation")
	FWeaponIK zoomState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Weapon Infomation")
	FFireSelector fireSelectorSetting;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Weapon Infomation")
	TArray<UAnimMontage*> reloadAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Weapon Infomation")
	UAnimationAsset* fireAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Weapon Infomation")
	TSubclassOf<UDWP_Scope> weaponScope;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Weapon Infomation")
	FProjectileInitializer projectileSetting;
};

USTRUCT(Atomic, BlueprintType)
struct FObjectPoolingData
{
	GENERATED_USTRUCT_BODY()
public:
	FObjectPoolingData() :
		spawnObjectType(),
		instanceObjectsKey(),
		spawnCounter() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Object Pooling Data")
	TSubclassOf<AActor> spawnObjectType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Object Pooling Data")
	FString instanceObjectsKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Object Pooling Data")
	int32 spawnCounter;
};

USTRUCT(BlueprintType, Atomic)
struct FDFItem :public FTableRowBase {
	GENERATED_USTRUCT_BODY()

public:
	FDFItem() :
		itemName(),
		iconImage(),
		itemMesh(),
		itemColor(),
		itemCollisionRadius(),
		packetCount(),
		itemPrice(),
		useableImplementation(),
		description() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Item Property")
	FString itemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Item Property")
	UTexture2D* iconImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Item Property")
	UStaticMesh* itemMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Item Property")
	TArray<UMaterialInterface*> itemColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Item Property")
	FTransform itemRelativeTransform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Item Property")
	float itemCollisionRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Item Property")
	int32 packetCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Item Property")
	int32 itemPrice;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Item Property")
	TSubclassOf<ADA_Item> useableImplementation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Item Property")
	FText description;
};

USTRUCT(Atomic, BlueprintType)
struct FWeaponOwnedData
{
	GENERATED_USTRUCT_BODY()
public:
	FWeaponOwnedData() :
		ownedState(EWEAPON_OWNED_STATE::NONE_OWNED),
		weaponData(),
		fireMode(EWEAPON_FIRE_SELECTOR::SEMI),
		current(),
		total()
	{}
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Ownership")
	EWEAPON_OWNED_STATE ownedState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Ownership")
	FDFWeaponData weaponData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "FireMode")
	EWEAPON_FIRE_SELECTOR fireMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Maganize")
	int32 current;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Maganize")
	int32 total;
};

USTRUCT(BlueprintType, Atomic)
struct FDFPlayerData :public FTableRowBase {
	GENERATED_USTRUCT_BODY()

public:
	FDFPlayerData() :
		characterSkeletalMesh(),
		animInstance(),
		animMontageMap()
	{}
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Character Infomation")
	TSoftObjectPtr<UTexture2D> characterIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Character Infomation")
	TSoftObjectPtr<USkeletalMesh> characterSkeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Character Infomation")
	TSoftClassPtr<UDOA_Player> animInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Character Infomation")
	TMap<FString, UAnimMontage*> animMontageMap;
};

USTRUCT(BlueprintType, Atomic)
struct FDFActiveSkillData :public FTableRowBase {
	GENERATED_USTRUCT_BODY()

public:
	FDFActiveSkillData() :
		activeSkillClass()
	{}
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "ActiveSkill")
	TSubclassOf<UDO_ActiveBase> activeSkillClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "MonsterSetting")
	FString monsterSkillKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "MonsterSetting")
	float attackRange;
};

USTRUCT(BlueprintType, Atomic)
struct FDFPassiveSkillData :public FTableRowBase {
	GENERATED_USTRUCT_BODY()

public:
	FDFPassiveSkillData() :
		passiveSkillClass()
	{}
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "PassiveSkill")
	TSubclassOf<UDO_PassiveBase> passiveSkillClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "PassiveSkill")
	FString monsterSkillKey;
};

USTRUCT(BlueprintType, Atomic)
struct FDFMonsterData :public FTableRowBase {
	GENERATED_USTRUCT_BODY()

public:
	FDFMonsterData() :
		monsterName(),
		skeletalMesh(),
		weaponMesh(),
		weaponRelativeTransform(FTransform(FRotator::ZeroRotator, FVector::ZeroVector, FVector(1.f))),
		animBP(),
		behaviorTree(),
		scale(1.f),
		widgetLocation(FVector(0.f, 0.f, 100.f)),
		hearingRange(2000.f),
		maxAgeHearing(10.f),
		sightRadius(1000.f),
		loseSightRadius(1500.f),
		sightAngle(70.f),
		maxAgeSight(10.f)
	{}
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "MonsterData")
	FString monsterName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "MonsterData")
	TSoftObjectPtr<USkeletalMesh> skeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "MonsterData")
	TSoftObjectPtr<UStaticMesh> weaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "MonsterData")
	FTransform weaponRelativeTransform;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "MonsterData")
	TSoftClassPtr<UDO_AnimInstance> animBP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "MonsterData")
	TSoftObjectPtr<UBehaviorTree> behaviorTree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "MonsterData")
	TSoftObjectPtr<UBlackboardData> blackBoard;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "MonsterData")
	float scale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "MonsterData")
	TSoftClassPtr<UDW_MonsterHP> monsterHPWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "MonsterData")
	TSoftObjectPtr<UTexture2D> monsterImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "MonsterData")
	FVector widgetLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "MonsterData")
	TMap<FString, UAnimMontage*> animMontageMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "MonsterData")
	TArray<FDFActiveSkillData> monsterActiveSkills;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "MonsterData")
	TArray<FDFPassiveSkillData> monsterPassiveSkills;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "AIPerception_Hearing")
	float hearingRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "AIPerception_Hearing")
	float maxAgeHearing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "AIPerception_Sight")
	float sightRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "AIPerception_Sight")
	float loseSightRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "AIPerception_Sight")
	float sightAngle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "AIPerception_Sight")
	float maxAgeSight;
};