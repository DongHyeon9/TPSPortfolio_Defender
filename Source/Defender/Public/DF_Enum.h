#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EPLAYER_MOVEMENT : uint8 {
	IDLE		UMETA(DisplayName = "Idle"),
	MOVING		UMETA(DisplayName = "Moving"),
	ROTATION	UMETA(DisplayName = "Rotation"),
};

UENUM(BlueprintType)
enum class ECAMERA_MODE : uint8 {
	DEFAULT		UMETA(DisplayName = "Default"),
	FREE		UMETA(DisplayName = "Free"),
	RESTORE		UMETA(DisplayName = "Restore"),
};

UENUM(BlueprintType)
enum class EWEAPON_FIRE_SELECTOR : uint8 {
	SEMI	UMETA(DisplayName = "Semi"),
	BURST	UMETA(DisplayName = "Burst"),
	AUTO	UMETA(DisplayName = "Auto"),
};

UENUM(BlueprintType)
enum class EITEM_TYPE : uint8 {
	WEWAPON		UMETA(DisplayName = "Weapon"),
	USE			UMETA(DisplayName = "Use"),
};

UENUM(BlueprintType)
enum class EWEAPON_ATTACK_TYPE : uint8 {
	TRACING			UMETA(DisplayName = "Tracing"),
	TRACINGMULTI	UMETA(DisplayName = "TracingMulti"),
	PROJECTILE		UMETA(DisplayName = "Projectile"),
};

UENUM(BlueprintType)
enum class EPARKOUR_STATE : uint8 {
	NOT_PARKOUR		UMETA(DisplayName = "NotParkour"),
	PARKOURING		UMETA(DisplayName = "Parkouring"),
};

UENUM(BlueprintType)
enum class EWEAPON_OWNED_STATE : uint8 {
	OWNED		UMETA(DisplayName = "Owned"),
	NONE_OWNED	UMETA(DisplayName = "None Owned"),
	EQUIP		UMETA(DisplayName = "Equip"),
};

UENUM(BlueprintType)
enum class ESTAT_DATA_TYPE : uint8 {
	DEFAULT		UMETA(DisplayName = "Defualt"),
	ADDITION	UMETA(DisplayName = "Addition"),
	TOTAL		UMETA(DisplayName = "Total"),
};

UENUM(BlueprintType)
enum class EPROJETILE_TYPE : uint8 {
	STRAIGHT	UMETA(DisplayName = "Straight"),
	HOMING		UMETA(DisplayName = "Homing"),
};

UENUM(BlueprintType)
enum class ECHARACTER_STATE : uint8 {
	NORMAL			UMETA(DisplayName = "Normal"),
	CASTING			UMETA(DisplayName = "Casting"),
	CHARGING		UMETA(DisplayName = "Charging"),
	STUN			UMETA(DisplayName = "Stun"),
	HIT				UMETA(DisplayName = "Hit"),
	IGNORE			UMETA(DisplayName = "Ignore"),
	INVINCIBILITY	UMETA(DisplayName = "Invincibility"),
	DEAD			UMETA(DisplayName = "Dead"),
};

UENUM(BlueprintType)
enum class ESKILL_TRACE_TYPE : uint8 {
	SPHERE			UMETA(DisplayName = "Sphere"),
	BOX				UMETA(DisplayName = "Box"),
	CAPSULE			UMETA(DisplayName = "Capsule"),
};

UENUM(BlueprintType)
enum class EMONSTER_TYPE : uint8 {
	MELEE			UMETA(DisplayName = "Melee"),
	PROJECTILE		UMETA(DisplayName = "Projectile"),
	CAST			UMETA(DisplayName = "Cast"),
	MIDDLE_BOSS		UMETA(DisplayName = "MiddleBoss"),
	FINAL_BOSS		UMETA(DisplayName = "FinalBoss"),
};

UENUM(BlueprintType)
enum class EMONSTER_TRACE_STATE : uint8 {
	IDLE			UMETA(DisplayName = "Idle"),
	PATROL			UMETA(DisplayName = "Patrol"),
	DAMAGE			UMETA(DisplayName = "Damage"),
	HEARING			UMETA(DisplayName = "Hearing"),
	SIGHT			UMETA(DisplayName = "Sight"),
	TRACE			UMETA(DisplayName = "Trace"),
	ATTACK			UMETA(DisplayName = "Attack"),
};