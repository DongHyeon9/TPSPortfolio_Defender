#include "Actor/Actor/DA_Projectile.h"
#include "Actor/Actor/DA_HomingTarget.h"
#include "Actor/DA_Character.h"
#include "Server/DA_GameInstance.h"


#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

ADA_Projectile::ADA_Projectile()
{
	PrimaryActorTick.bCanEverTick = true;

	collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	tailEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("TailEffect"));
	projectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));

	RootComponent = collision;
	collision->SetCollisionProfileName(FName("Projectile"));

	Tags.Add(TEXT("Projectile"));

	mesh->SetupAttachment(RootComponent);
	tailEffect->SetupAttachment(RootComponent);
}

void ADA_Projectile::InitializeProjectile(const FProjectileInitializer& _Initalizer)
{
	//구조체별로 초기화 해줘야되는 내용이 다르기 때문에 구조체에 초기화를 위임한다
	// ex) 일반 발사체는 속도랑 이펙트 등 초기화하지만 유도발사체는 타겟까지 초기화해야된다
	_Initalizer.ApplySetting(this);
}

void ADA_Projectile::InitializeHoming(const FHomingInitializer& _Initalizer)
{
	//구조체별로 초기화 해줘야되는 내용이 다르기 때문에 구조체에 초기화를 위임한다
	// ex) 일반 발사체는 속도랑 이펙트 등 초기화하지만 유도발사체는 타겟까지 초기화해야된다	
	_Initalizer.ApplySetting(this);
}

void ADA_Projectile::BeginPlay()
{
	Super::BeginPlay();

	collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ADA_Projectile::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);

	timeLine.TickTimeline(_DeltaTime);
}

void ADA_Projectile::MeshActive_Implementation(bool _bIsActive)
{
	mesh->SetVisibility(_bIsActive, true);
	tailEffect->SetVisibility(_bIsActive, true);

#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	mesh->SetHiddenInGame(!_bIsActive);
	
#endif
	
}

void ADA_Projectile::CollisionActive_Implementation(bool _bIsActive)
{
	if (_bIsActive) {
		collision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		collision->OnComponentHit.AddDynamic(this, &ADA_Projectile::OnHit);
	}

	else {
		collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		collision->OnComponentHit.RemoveDynamic(this, &ADA_Projectile::OnHit);
	}
}

void ADA_Projectile::EndPlay(EEndPlayReason::Type _EndPlayReason)
{
	Super::EndPlay(_EndPlayReason);
	
	if (timeLine.IsPlaying())
		timeLine.Stop();

	if(onTimeLineUpdate.IsBound())
		onTimeLineUpdate.Unbind();

	if (onTimeLineFinish.IsBound())
		onTimeLineFinish.Unbind();
}

void ADA_Projectile::ActiveSetting_Implementation(bool _bIsActive)
{
	bIsActive = _bIsActive;
	if (_bIsActive) {
		projectileMovement->Velocity = FVector(1.f, 0.f, 0.f);
		
		//라이프타임이 있다면 라이프 타임뒤에 프로젝타일을 폭파시킨다
		if (lifeTime > 0.f) {

			auto gameInstance = GetGameInstance<UDA_GameInstance>();
			DFCHECK(gameInstance != nullptr);
			gameInstance->RegistTimerHandle(&lifeTimerHandle);

			GetWorld()->GetTimerManager().SetTimer(lifeTimerHandle, FTimerDelegate::CreateLambda([&]()
				{
					if (bIsActive)
						OnHit(nullptr, nullptr, nullptr, GetActorLocation(), FHitResult());
				}), lifeTime, false);
		}
		SetActorTickEnabled(true);
		projectileMovement->SetComponentTickEnabled(true);

		//유도발사체라면 타임라인을 셋팅해주고 활성화 한다
		switch (projectileType)
		{
		case EPROJETILE_TYPE::STRAIGHT: {

			break;
		}
		case EPROJETILE_TYPE::HOMING: {
			float minTime{}, maxTime{};
			onTimeLineUpdate.BindDynamic(this, &ADA_Projectile::TimeLineUpdate);
			onTimeLineFinish.BindDynamic(this, &ADA_Projectile::TimeLineFinish);

			timeLine.AddInterpFloat(floatCurve, onTimeLineUpdate);
			timeLine.SetTimelineFinishedFunc(onTimeLineFinish);

			floatCurve->GetTimeRange(minTime, maxTime);
			timeLine.SetTimelineLength(maxTime - minTime);

			auto gameInstance = GetGameInstance<UDA_GameInstance>();
			DFCHECK(gameInstance != nullptr);
			gameInstance->RegistTimerHandle(&homingTimerHandle);

			GetWorld()->GetTimerManager().SetTimer(homingTimerHandle, FTimerDelegate::CreateLambda([&]()
				{
					startRotation = GetActorRotation();
					lookAtLocation = targetActor != nullptr ? targetActor->GetActorLocation() : targetLocation;
					timeLine.PlayFromStart();
				}), homingDelay, false);
			break;
		}
		}
		if (projectileMovement->HasBeenInitialized())
			projectileMovement->UninitializeComponent();
		projectileMovement->InitializeComponent();
	}

	else {
		//활성화 할 때 초기화 해줬던 것들을 다시 사용 전으로 돌려준다
		switch (projectileType)
		{
			if (lifeTimerHandle.IsValid()) {
				GetWorld()->GetTimerManager().ClearTimer(lifeTimerHandle);
				lifeTimerHandle.Invalidate();
			}

		case EPROJETILE_TYPE::STRAIGHT: {

			break;
		}
		case EPROJETILE_TYPE::HOMING: {
			onTimeLineUpdate.Unbind();
			onTimeLineFinish.Unbind();
			projectileMovement->bIsHomingProjectile = false;
			projectileMovement->HomingTargetComponent = nullptr;
			if (homingTimerHandle.IsValid()) {
				GetWorld()->GetTimerManager().ClearTimer(homingTimerHandle);
				homingTimerHandle.Invalidate();
			}
			break;
		}
		}

		SetActorTickEnabled(false);
		projectileMovement->SetComponentTickEnabled(false);
		if (projectileMovement->HasBeenInitialized())
			projectileMovement->UninitializeComponent();
	}

	IDI_ObjectPoolingActor::Execute_MeshActive(this, _bIsActive);
	IDI_ObjectPoolingActor::Execute_CollisionActive(this, _bIsActive);
}

void ADA_Projectile::OnHit(UPrimitiveComponent* _HitComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, FVector _NormalImpulse, const FHitResult& _Hit)
{
	//부딛힌 액터와 태그를 확인한다
	if (_OtherActor != nullptr) {
		for (auto ignoreTag : ignoreTags)
			if (_OtherActor->Tags.Contains(ignoreTag))
				return;
	}

	//폭팔 이펙트를 활성화 한다
	if (finishEffect != nullptr)
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), finishEffect, GetActorLocation(), GetActorRotation(), FVector(finishEffectSize));

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes{};
	TEnumAsByte<EObjectTypeQuery> pawn = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn);
	ObjectTypes.Add(pawn);
	TArray<FHitResult> hitResults{};

	//폭팔범위내의 캐릭터들의 태그를 확인하고 데미지를 준다
	if (UKismetSystemLibrary::SphereTraceMultiForObjects(
		GetWorld(),
		GetActorLocation(),
		GetActorLocation(),
		damageRadius,
		ObjectTypes,
		false,
		TArray<AActor*>(),
		EDrawDebugTrace::ForDuration,
		hitResults,
		true
	)) {
		for (auto& hitResult : hitResults) {
			auto character = Cast<ADA_Character>(hitResult.GetActor());
			if (character != nullptr) {
				for (auto ignoreTag : ignoreTags) {
					bool bIsIgnore{};
					if (character->Tags.Contains(ignoreTag)) {
						bIsIgnore = true;
						break;
					}
					if (!bIsIgnore)
						character->ApplyDamage(damage, ownerCharacter, ownerCharacter->GetController(), character->GetActorLocation());
				}
			}
		}
	}

	//소환한 유도타겟을 비활성화 한다
	if (homingTarget != nullptr)
		IDI_ObjectPoolingActor::Execute_ActiveSetting(homingTarget, false);
	IDI_ObjectPoolingActor::Execute_ActiveSetting(this, false);
	onHit.Broadcast(this);
}

void ADA_Projectile::TimeLineUpdate(float _OutPut)
{
	//발사체의 방향을 타겟쪽으로 돌린다
	FRotator lootAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), lookAtLocation);
	FRotator targetRotation = UKismetMathLibrary::RLerp(startRotation, lootAtRotation, _OutPut, true);
	SetActorRotation(targetRotation);
}

void ADA_Projectile::TimeLineFinish()
{
	//유도 기능을 활성화한다
	if (targetActor != nullptr) targetLocation = targetActor->GetActorLocation();

	if (homingTarget != nullptr)
		homingTarget->SetActorLocation(targetLocation);

	else
		homingTarget = GetWorld()->SpawnActor<ADA_HomingTarget>(targetLocation, FRotator::ZeroRotator);

	IDI_ObjectPoolingActor::Execute_ActiveSetting(homingTarget, true);

	projectileMovement->HomingTargetComponent = homingTarget->GetRootComponent();
	projectileMovement->SetVelocityInLocalSpace(FVector::ZeroVector);
	projectileMovement->ProjectileGravityScale = 0.f;
	projectileMovement->HomingAccelerationMagnitude = homingSpeed;
	projectileMovement->bIsHomingProjectile = true;
}

