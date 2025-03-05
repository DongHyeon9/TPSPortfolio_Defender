#include "Component/Player/DC_Targeting.h"
#include "Actor/Player/DA_Player.h"
#include "Component/Share/DC_Stat.h"
#include "Actor/Monster/DA_Monster.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Camera/CameraComponent.h"

UDC_Targeting::UDC_Targeting()
{
	//틱활성화
	PrimaryComponentTick.bCanEverTick = true;

	//UPROPERTY변수 초기화
	targetTraceRadius = 500.f;
	targetDistance = 3000.f;
	lockOnInterpSpeed = 30.f;
	targetMonster = nullptr;
	playerCamera = nullptr;
}

void UDC_Targeting::LockOn()
{
	DFLOG_N(Display);
	//타겟이 있다면 타겟제거
	if (targetMonster != nullptr)
		SetTargetCharacter(nullptr);

	else {
		//타겟들을 트레이싱하고 타겟들의 위치에 따라 최종 타겟팅할 액터를 정합니다
		TArray<FHitResult> hitResults{};
		if (TraceTarget(hitResults)) {

			float nearestDistance = 1.f;
			float currentDistance;
			ADA_Monster* nearestTarget{};

			for (const auto& hitResult : hitResults) {

				currentDistance = FMath::Abs(CalculateScreenYDistance(hitResult.GetActor()));
				
				if (nearestDistance > currentDistance) {
					nearestDistance = currentDistance;
					nearestTarget = Cast<ADA_Monster>(hitResult.GetActor());
				}
			}
			SetTargetCharacter(nearestTarget);
		}
	}
}

void UDC_Targeting::TargetMoveRight()
{
	DFLOG_N(Display);
	if (targetMonster == nullptr) return;

	//타겟들을 트레이싱하고 타겟들의 위치에 따라 최종 타겟팅할 액터를 정합니다
	TArray<FHitResult> hitResults{};
	if (TraceTarget(hitResults)) {

		float nearestDistance = 1.f;
		float currentDistance;
		ADA_Monster* nearestTarget{};

		for (const auto& hitResult : hitResults) {
			currentDistance = CalculateScreenYDistance(hitResult.GetActor());
			bool bNearset =
				(currentDistance > 0.f) &&
				(currentDistance < nearestDistance);

			if (bNearset) {
				nearestDistance = currentDistance;
				nearestTarget = Cast<ADA_Monster>(hitResult.GetActor());
			}
		}
		SetTargetCharacter(nearestTarget);
	}
}

void UDC_Targeting::TargetMoveLeft()
{
	DFLOG_N(Display);
	if (targetMonster == nullptr) return;

	//타겟들을 트레이싱하고 타겟들의 위치에 따라 최종 타겟팅할 액터를 정합니다
	TArray<FHitResult> hitResults{};
	if (TraceTarget(hitResults)) {

		float nearestDistance = -1.f;
		float currentDistance;
		ADA_Monster* nearestTarget{};

		for (const auto& hitResult : hitResults) {
			currentDistance = CalculateScreenYDistance(hitResult.GetActor());
			bool bNearset =
				(currentDistance < 0.f) &&
				(currentDistance > nearestDistance);

			if (bNearset) {
				nearestDistance = currentDistance;
				nearestTarget = Cast<ADA_Monster>(hitResult.GetActor());
			}
		}
		SetTargetCharacter(nearestTarget);
	}
}

void UDC_Targeting::InitializeComponent()
{
	Super::InitializeComponent();

	auto player = Cast<ADA_Player>(GetOwnerCharacter());
	if (player != nullptr)
		playerCamera = player->GetPlayerCameraComponent();
}

void UDC_Targeting::TickComponent(float _DeltaTime, ELevelTick _TickType, FActorComponentTickFunction* _ThisTickFunction)
{
	Super::TickComponent(_DeltaTime, _TickType, _ThisTickFunction);

	//타겟이 있다면 타겟쪽으로 컨트롤 로테이션을 돌립니다
	if (targetMonster != nullptr) {
		FVector startLocation = playerCamera != nullptr ?
			playerCamera->GetComponentLocation() :
			GetOwnerCharacter()->GetActorLocation();

		FVector endLocation = targetMonster->GetLockOnArea();

		FRotator targetRotation =
			UKismetMathLibrary::FindLookAtRotation(
				startLocation,
				endLocation);

		GetOwnerCharacter()->GetController()->SetControlRotation(
			FMath::RInterpTo(GetOwnerCharacter()->GetControlRotation(),
				targetRotation,
				_DeltaTime,
				lockOnInterpSpeed));

		PRINTSTR(LOCK_ON_TARGET, 1.f, FColor::Red, TEXT("Owner : %s / Target : %s"), *GetOwnerCharacter()->GetName(), *targetMonster->GetName());
	}
}

bool UDC_Targeting::TraceTarget(TArray<FHitResult>& _OutHitResults)
{
	DFLOG_N(Display);
	DFCHECK(GetOwnerCharacter() != nullptr, false);
	//일정거리에 있는 적들을 타겟팅한다
	FVector startLocation = GetOwnerCharacter()->GetActorLocation();

	FVector forwardVector = playerCamera != nullptr ?
		playerCamera->GetForwardVector():
		UKismetMathLibrary::GetForwardVector(GetOwnerCharacter()->GetController()->GetControlRotation());

	startLocation += (forwardVector * 250.f);
	FVector endLocation = forwardVector * targetDistance;

	endLocation += startLocation;
	
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes{};
	TEnumAsByte<EObjectTypeQuery> pawn = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn);
	ObjectTypes.Add(pawn);
	TArray<AActor*> ignoreActors{};
	TArray<FHitResult> hitResults{};
	ignoreActors.Add(GetOwnerCharacter());
	bool result =
		UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(),
			startLocation,
			endLocation,
			targetTraceRadius,
			ObjectTypes,
			false,
			ignoreActors,
			EDrawDebugTrace::ForDuration,
			hitResults,
			true);

	for (const auto& hitResult : hitResults) {

		auto monster = Cast<ADA_Monster>(hitResult.GetActor());

		if (monster != nullptr &&
			monster != targetMonster &&
			monster->GetStatComponent()->GetState() != ECHARACTER_STATE::DEAD)
			_OutHitResults.Add(hitResult);
	}

	return result;
}

float UDC_Targeting::CalculateScreenYDistance(const AActor* _TargetActor)
{
	DFLOG_N(Display);
	//각 액터의 위치를 영점으로 맞추고 그 1사분면과 2사분면으로 회전시킨다
	//이후 정규화된 Y값(좌,우)을 반환한다
	FVector relativeLocation = _TargetActor->GetActorLocation();

	playerCamera != nullptr ?
		relativeLocation -= playerCamera->GetComponentLocation() :
		relativeLocation -= GetOwnerCharacter()->GetActorLocation();
	
	DFCHECK(relativeLocation.Normalize(), 1.f);

	relativeLocation.Z = 1.f;
	float angle = 360.f - GetOwnerCharacter()->GetControlRotation().Yaw;
	FVector transformedCoordinates =
		UKismetMathLibrary::RotateAngleAxis(
			relativeLocation,
			angle,
			FVector(0.f, 0.f, 1.f));

	DFLOG(Display, TEXT("%s distance : %f"), *_TargetActor->GetName(), transformedCoordinates.Y);
	
	return transformedCoordinates.Y;
}

void UDC_Targeting::SetTargetCharacter(ADA_Monster* _NewTarget)
{
	DFLOG_N(Display);
	//타겟된 적들을 선명하게 나타내기위해 렌더뎁스를 설정해준다
	if (targetMonster != nullptr) {
		targetMonster->GetMesh()->SetRenderCustomDepth(false);
		targetMonster->GetStatComponent()->onChangeState.RemoveDynamic(this, &UDC_Targeting::TargetDead);
	}

	if (_NewTarget != nullptr) {
		_NewTarget->GetMesh()->SetRenderCustomDepth(true);
		_NewTarget->GetStatComponent()->onChangeState.AddDynamic(this, &UDC_Targeting::TargetDead);
	}

	if (targetMonster != nullptr &&
		_NewTarget != nullptr)
		DFLOG(Warning, TEXT("Change Target %s : %s -> %s"), *GetOwnerCharacter()->GetName(), *targetMonster->GetName(), *_NewTarget->GetName());

	targetMonster = _NewTarget;
}

void UDC_Targeting::TargetDead(ECHARACTER_STATE _CharacterState)
{
	if (_CharacterState != ECHARACTER_STATE::DEAD) return;

	SetTargetCharacter(nullptr);
	LockOn();
}
