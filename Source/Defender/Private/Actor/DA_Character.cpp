#include "Actor/DA_Character.h"
#include "Component/Share/DC_Stat.h"
#include "Component/Share/DC_AnimMontage.h"
#include "Component/Share/DC_Skill.h"
#include "Component/Share/DC_Minimap.h"
#include "Actor/Player/Controller/DAPC_InGame.h"
#include "Widget/InGame/Monster/DWP_FloatingDamage.h"
#include "Component/Share/DC_ProjectileManager.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Components/CapsuleComponent.h"

ADA_Character::ADA_Character()
{
	PrimaryActorTick.bCanEverTick = true;

	//UPROPERTY변수 초기화
	characterStartLevel = MIN_LEVEL;

	//컴포넌트 셋팅
	animMontageComp = CreateDefaultSubobject<UDC_AnimMontage>(TEXT("AnimMontageComp"));
	statComp = CreateDefaultSubobject<UDC_Stat>(TEXT("StatComp"));
	skillComp = CreateDefaultSubobject<UDC_Skill>(TEXT("SkillComp"));
	minimapComp = CreateDefaultSubobject<UDC_Minimap>(TEXT("MinimapComp"));
	projectileManagerComp = CreateDefaultSubobject<UDC_ProjectileManager>(TEXT("ProjectileManagerComp"));

	//메쉬에 대한 설정
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -88.f), FRotator(0.f, -90.f, 0.f));
	GetMesh()->bReceivesDecals = false;

	//캐릭터 무브먼트 기본설정
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 400.f;

	AutoPossessAI = EAutoPossessAI::Disabled;

	static ConstructorHelpers::FClassFinder<UDWP_FloatingDamage> WG_FLOATING_DAMAGE(TEXT("/Game/01_Blueprint/Widget/InGame/Monster/WG_FloatingDamage"));
	if (WG_FLOATING_DAMAGE.Succeeded())
		floatingDamageWidgetClass = WG_FLOATING_DAMAGE.Class;
}

void ADA_Character::ApplyDamage(float _DamageAmount, AActor* _DamageCauser, AController* _DamageInstigator, FVector _HitPoint)
{
	if (statComp->AddCurrentHP(-_DamageAmount, _DamageInstigator)) {
		UAISense_Damage::ReportDamageEvent(GetWorld(),
			this,
			_DamageCauser,
			_DamageAmount,
			_DamageCauser->GetActorLocation(),
			GetActorLocation());
		onApplyDamage.Broadcast(_DamageAmount, _DamageCauser, _DamageInstigator);
		if (floatingDamageWidgetClass != nullptr &&
			_DamageInstigator != nullptr &&
			_DamageInstigator->IsPlayerController()) {
			auto playerController = Cast<ADAPC_InGame>(_DamageInstigator);
			auto floatingDamageWidget = CreateWidget<UDWP_FloatingDamage>(playerController, floatingDamageWidgetClass);
			FVector2D screenPostion{};
			if (UGameplayStatics::ProjectWorldToScreen(playerController, _HitPoint, screenPostion)) {
				floatingDamageWidget->InitializeFloatingDamage(screenPostion, _DamageAmount);
				floatingDamageWidget->ShowInViewport(VO_FLOATING_DAMAGE);
			}
		}
	}
}

void ADA_Character::CharacterInitialize()
{
	DFLOG_N(Display);
	GetMesh()->AddTickPrerequisiteActor(this);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	animMontageComp->AddTickPrerequisiteActor(this);
	skillComp->AddTickPrerequisiteActor(this);
	statComp->AddTickPrerequisiteActor(this);
	minimapComp->AddTickPrerequisiteActor(this);
	projectileManagerComp->AddTickPrerequisiteActor(this);
}

void ADA_Character::SetStartLevel(int32 _StartLevel)
{
	characterStartLevel = _StartLevel;
}

bool ADA_Character::GetFloorLocation(FVector& _OutFloorLocation)
{
	FVector startLocation = GetActorLocation();
	FVector endLocation = GetActorUpVector() * -1000.f + startLocation;

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes{};
	TEnumAsByte<EObjectTypeQuery> worldStatic = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic);
	ObjectTypes.Add(worldStatic);

	FHitResult hitResult{};

	if (UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(),
		startLocation,
		endLocation,
		ObjectTypes,
		false,
		TArray<AActor*>(),
		EDrawDebugTrace::ForDuration,
		hitResult,
		true)) {
		_OutFloorLocation = hitResult.Location;
		return true;
	}

	return false;
}

void ADA_Character::BeginPlay()
{
	DFLOG_N(Display);
	skillComp->ApplyPassive();
	CharacterInitialize();
	statComp->SetLevel(FMath::Clamp(characterStartLevel, MIN_LEVEL, MAX_LEVEL));

	Super::BeginPlay();

#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	GetCapsuleComponent()->SetHiddenInGame(false);

#endif
}
