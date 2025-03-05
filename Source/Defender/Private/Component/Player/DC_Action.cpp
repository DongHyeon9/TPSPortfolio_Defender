#include "Component/Player/DC_Action.h"
#include "Actor/Player/DA_Player.h"
#include "Actor/Player/Controller/DAPC_InGame.h"
#include "Component/Player/DC_Weapon.h"
#include "Component/Share/DC_Stat.h"
#include "Component/Share/DC_AnimMontage.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"

UDC_Action::UDC_Action()
{
	PrimaryComponentTick.bCanEverTick = true;

	//UPROPERTY변수 초기화
	zoomInArmLength = 100.f;
	zoomOutArmLength = 200.f;
	cameraArmLengthInterpSpeed = 20.f;
	rotationSpeed = 10.f;
	minParkourHeight = 50.f;
	maxParkourHeight = 170.f;
	parkourSpeed = 3.f;
	rollDistance = 300.f;

	//맴버변수 초기화
	sensitivity = 0.5f;
	controlYaw = 0.f;
	targetArmLength = zoomOutArmLength;
	currentCameraMode = ECAMERA_MODE::DEFAULT;
	bIsRightMoving = false;
	bIsForwardMoving = false;
	bIsMoving = false;
	bRotationSame = false;
	bBeforeSettingYaw = false;
	bIsZooming = false;
	bIsDashing = false;
	bCanAttack = true;
	bForceRotation = false;
	currentMovementMode = EPLAYER_MOVEMENT::IDLE;
	parkourProgress = 0.f;
	currentParkourState = EPARKOUR_STATE::NOT_PARKOUR;
	startLocation = FVector::ZeroVector;
	endLocation = FVector::ZeroVector;
	bSweep = false;
}

void UDC_Action::Interaction()
{
	onInteraction.Broadcast(ownerPlayer, ownerController);
}

void UDC_Action::Dash()
{
	DFLOG_N(Display);
	switch (currentState)
	{
	case ECHARACTER_STATE::NORMAL:{
		bIsDashing = !bIsDashing;
		if (bIsDashing)
			ownerPlayer->GetStatComponent()->SetAdditionMoveSpeed(TEXT("Dash"), ownerPlayer->GetStatComponent()->GetSpeed() * 0.5);
		else
			bIsDashing = !bIsDashing;
		break;
	}
	}

}

void UDC_Action::Walk()
{
	DFLOG_N(Display);
	bIsDashing = !bIsDashing;
	if (!bIsDashing)
		ownerPlayer->GetStatComponent()->RemoveMoveSpeed(TEXT("Dash"));
	else
		bIsDashing = !bIsDashing;
}

void UDC_Action::Crouch()
{
	DFLOG_N(Display);
	switch (currentState)
	{
	case ECHARACTER_STATE::NORMAL:
	case ECHARACTER_STATE::INVINCIBILITY:
	case ECHARACTER_STATE::CASTING:
	case ECHARACTER_STATE::CHARGING:
	case ECHARACTER_STATE::HIT:
	case ECHARACTER_STATE::IGNORE:
	case ECHARACTER_STATE::STUN: {
		if (playerMovement->IsFalling()) return;

		if (ownerPlayer->bIsCrouched) ownerPlayer->UnCrouch();
		else ownerPlayer->Crouch();
		break;
	}
	}
}

void UDC_Action::ZoomIn()
{
	DFLOG_N(Display);
	switch (currentState)
	{
	case ECHARACTER_STATE::NORMAL: {
		bIsZooming = true;
		targetArmLength = zoomInArmLength;
		break;
	}
	}
}

void UDC_Action::ZoomOut()
{
	DFLOG_N(Display);
	if (bIsZooming) {
		bIsZooming = false;
		targetArmLength = zoomOutArmLength;
	}
}

void UDC_Action::CameraModeFree()
{
	DFLOG_N(Display);
	if (currentState != ECHARACTER_STATE::DEAD) {
		ownerPlayer->bUseControllerRotationYaw = false;
		currentCameraMode = ECAMERA_MODE::FREE;
	}
}

void UDC_Action::CameraModeFix()
{
	DFLOG_N(Display);
	currentCameraMode = ECAMERA_MODE::RESTORE;
}

void UDC_Action::MoveForward(float _Value)
{
	DFCHECK(ownerPlayer->Controller != nullptr);

	switch (currentState)
	{
	case ECHARACTER_STATE::NORMAL:{
		bIsForwardMoving = _Value != 0.f;
		if (bIsForwardMoving) {
			FRotator targetRotation{};
			switch (currentCameraMode) {
			case ECAMERA_MODE::DEFAULT:
			case ECAMERA_MODE::RESTORE: {
				targetRotation = FRotator(0, ownerPlayer->GetControlRotation().Yaw, 0);
				break;
			}
			case ECAMERA_MODE::FREE: {
				targetRotation = FRotator(0, ownerPlayer->GetActorRotation().Yaw, 0);
				break;
			}
			}
			ownerPlayer->AddMovementInput(FRotationMatrix(targetRotation).GetUnitAxis(EAxis::X), _Value);

			PRINTSTR(PLAYER_SPEED, 1.f, FColor::Cyan, TEXT("MoveSpeed : %.0f"), playerMovement->MaxWalkSpeed);
		}
		break;
	}
	}
}

void UDC_Action::MoveRight(float _Value)
{
	DFCHECK(ownerPlayer->Controller != nullptr);

	switch (currentState)
	{
	case ECHARACTER_STATE::NORMAL:{
		bIsRightMoving = _Value != 0.f;
		if (bIsRightMoving) {
			FRotator targetRotation{};
			switch (currentCameraMode) {
			case ECAMERA_MODE::DEFAULT:
			case ECAMERA_MODE::RESTORE: {
				targetRotation = FRotator(0, ownerPlayer->GetControlRotation().Yaw, 0);
				break;
			}
			case ECAMERA_MODE::FREE: {
				targetRotation = FRotator(0, ownerPlayer->GetActorRotation().Yaw, 0);
				break;
			}
			}
			ownerPlayer->AddMovementInput(FRotationMatrix(targetRotation).GetUnitAxis(EAxis::Y), _Value);

			PRINTSTR(PLAYER_SPEED, 1.f, FColor::Cyan, TEXT("MoveSpeed : %.0f"), playerMovement->MaxWalkSpeed);
		}
		break;
	}
	}
}

void UDC_Action::Look(float _Value)
{
	ownerPlayer->AddControllerPitchInput(-_Value * sensitivity);
}

void UDC_Action::Turn(float _Value)
{
	ownerPlayer->AddControllerYawInput(_Value * sensitivity);
}

void UDC_Action::SensitivityUp(float _Value)
{
	static float acc = 0.f;
	if (_Value != 0.f) {
		acc += GetWorld()->GetDeltaSeconds();
		if (acc > 0.1f) {
			sensitivity = FMath::Clamp(sensitivity + 0.01f, 0.f, 1.f);
			PRINTSTR(PLAYER_SENSITIVITY, 5.f, FColor::Cyan, TEXT("Sensitivity : %.2f"), sensitivity);
			acc = 0.f;
		}
	}
	else acc = 0.f;
}

void UDC_Action::SensitivityDown(float _Value)
{
	static float acc = 0.f;
	if (_Value != 0.f) {
		acc += GetWorld()->GetDeltaSeconds();
		if (acc > 0.1f) {
			sensitivity = FMath::Clamp(sensitivity - 0.01f, 0.f, 1.f);
			PRINTSTR(PLAYER_SENSITIVITY, 5.f, FColor::Cyan, TEXT("Sensitivity : %.2f"), sensitivity);
			acc = 0.f;
		}
	}
	else acc = 0.f;
}

void UDC_Action::WeaponAttack(float _Value)
{
	//입력이 들어오지않거나 카메라를 돌리고 있으면 값을 초기화해주고 리턴한다
	if (_Value == 0.f ||
		currentCameraMode != ECAMERA_MODE::DEFAULT) {
		bCanAttack = true;
		return;
	}

	switch (currentState)
	{
	case ECHARACTER_STATE::NORMAL:{
		//플레이어 회전을 설정해준다
		if (!bRotationSame)	bForceRotation = true;

		//무기의 발사모드에 따라 Fire함수 호출
		if (bCanAttack && bRotationSame) {
			PRINTSTR(PLAYER_ATTACK_INPUT, 1.f, FColor::Cyan, TEXT("Attack!!"));
			Fire();

			switch (ownerPlayer->GetWeaponComponent()->GetCurrentWeaponData().fireMode) {
			case EWEAPON_FIRE_SELECTOR::SEMI: {
				bCanAttack = false;
				break;
			}
			case EWEAPON_FIRE_SELECTOR::BURST: {
				bCanAttack = false;
				break;
			}
			case EWEAPON_FIRE_SELECTOR::AUTO: {

				break;
			}
			}
		}
		break;
	}
	}
}

void UDC_Action::Jump()
{
	DFLOG_N(Display);
	switch (currentState)
	{
	case ECHARACTER_STATE::NORMAL:{
		//파쿠르가 가능하다면 파쿠르하고 불가능하다면 점프를 한다
		if (CanParkour()) {
			currentParkourState = EPARKOUR_STATE::PARKOURING;
			parkourProgress = 0.f;
			startLocation = GetOwnerCharacter()->GetActorLocation();
			bSweep = false;
			GetOwnerCharacter()->GetAnimMontageComponent()->PlayAnimMontage(TEXT("Parkour"), parkourSpeed * 0.5);
			SetOwnerInput(false);
		}
		else
			GetOwnerCharacter()->Jump();
		break;
	}
	}
}

void UDC_Action::StopJump()
{
	DFLOG_N(Display);
	if (currentParkourState != EPARKOUR_STATE::PARKOURING)
		GetOwnerCharacter()->StopJumping();
}

void UDC_Action::Roll()
{
	DFLOG_N(Display);
	switch (currentState)
	{
	case ECHARACTER_STATE::NORMAL:{
		currentParkourState = EPARKOUR_STATE::PARKOURING;
		parkourProgress = 0.f;
		startLocation = GetOwnerCharacter()->GetActorLocation();
		bSweep = true;

		//캐릭터의 이동방향과 보고있는방향에 따라 구르기 모션을 다르게 한다
		float direction = (UKismetMathLibrary::Normal(GetOwnerCharacter()->GetCharacterMovement()->Velocity) - GetOwnerCharacter()->GetActorForwardVector()).Length();
		if (direction > 1.f) {
			GetOwnerCharacter()->GetAnimMontageComponent()->PlayAnimMontage(TEXT("BackRoll"), parkourSpeed * 0.5);
			endLocation = (GetOwnerCharacter()->GetActorForwardVector() * -rollDistance) + startLocation;
		}
		else {
			GetOwnerCharacter()->GetAnimMontageComponent()->PlayAnimMontage(TEXT("Roll"), parkourSpeed * 0.5);
			endLocation = (GetOwnerCharacter()->GetActorForwardVector() * rollDistance) + startLocation;
		}

		SetOwnerInput(false);
		ownerPlayer->GetStatComponent()->SetState(ECHARACTER_STATE::INVINCIBILITY);
		break;
	}
	}
}

void UDC_Action::SetSensitivity(float _NewSensitivity)
{
	sensitivity = _NewSensitivity;
}

void UDC_Action::InitializeComponent()
{
	Super::InitializeComponent();

	ownerPlayer = Cast<ADA_Player>(GetOwner());
	DFCHECK(ownerPlayer != nullptr);

	playerCamera = ownerPlayer->GetPlayerCameraComponent();
	cameraSpringArm = ownerPlayer->GetCameraSpringArmComponent();
	playerMovement = ownerPlayer->GetCharacterMovement();
	ownerPlayer->GetStatComponent()->onChangeState.AddDynamic(this, &UDC_Action::StateChange);

	//캐릭터 무브먼트 관련 설정
	ownerPlayer->bUseControllerRotationPitch = false;
	ownerPlayer->bUseControllerRotationYaw = true;
	ownerPlayer->bUseControllerRotationRoll = false;
	playerMovement->bOrientRotationToMovement = false;
	playerMovement->GetNavAgentPropertiesRef().bCanCrouch = true;
	playerMovement->SetCrouchedHalfHeight(60.f);
	playerMovement->MaxWalkSpeed = 400.f;

	//카메라 설정
	cameraSpringArm->TargetArmLength = 200.f;
	cameraSpringArm->SetRelativeLocation(FVector(0.f, 0.f, 65.f));
	cameraSpringArm->bUsePawnControlRotation = true;

	playerCamera->bUsePawnControlRotation = false;
	playerCamera->SetRelativeLocation(FVector(0.f, 50.f, 0.f));
}

void UDC_Action::TickComponent(float _DeltaTime, ELevelTick _TickType, FActorComponentTickFunction* _ThisTickFunction)
{
	Super::TickComponent(_DeltaTime, _TickType, _ThisTickFunction);

	//플레이어부터 카메라의 거리 셋팅
	cameraSpringArm->TargetArmLength = UKismetMathLibrary::FInterpTo(cameraSpringArm->TargetArmLength, targetArmLength, _DeltaTime, cameraArmLengthInterpSpeed);

	//크라우치 상태 체크
	if (playerMovement->IsFalling() && ownerPlayer->bIsCrouched) ownerPlayer->UnCrouch();

	//이동중인지 확인
	bIsMoving = (bIsRightMoving || bIsForwardMoving);
	//캐릭터의 시선과 플레이어의 카메라 방향이 같은지 확인
	bRotationSame =
		(UKismetMathLibrary::GetRightVector(ownerPlayer->GetControlRotation())
			- UKismetMathLibrary::GetRightVector(ownerPlayer->GetActorRotation())
			).Length() < 0.1;
	if (currentState == ECHARACTER_STATE::DEAD) return;
	//현재 카메라모드에 따라 함수 실행
	switch (currentCameraMode) {
	case ECAMERA_MODE::DEFAULT: {
		CameraMode_Default(_DeltaTime);
		break;
	}
	case ECAMERA_MODE::RESTORE: {
		CameraMode_Restore(_DeltaTime);
		break;
	}
	}

	//캐릭터의 로테이션을 보고있는 방향으로 바꿔준다
	if (bForceRotation) {
		SetActorRotationYaw(controlYaw, _DeltaTime);
		if (bRotationSame) bForceRotation = false;
	}

	if (currentParkourState == EPARKOUR_STATE::PARKOURING) {
		//파쿠르 진행상태에 따라 캐릭터의 파쿠르를 진행한다
		parkourProgress = UKismetMathLibrary::FClamp(parkourProgress + _DeltaTime * parkourSpeed, 0.f, 1.f);
		GetOwnerCharacter()->SetActorLocation(UKismetMathLibrary::VLerp(startLocation, endLocation, parkourProgress), bSweep);

		if (parkourProgress >= 1.f) {
			currentParkourState = EPARKOUR_STATE::NOT_PARKOUR;
			SetOwnerInput(true);
			ownerPlayer->GetStatComponent()->SetState(ECHARACTER_STATE::NORMAL);
		}
	}
}

void UDC_Action::BeginPlay()
{
	Super::BeginPlay();

	ownerController = Cast<ADAPC_InGame>(ownerPlayer->GetController());
	DFCHECK(ownerController != nullptr);
}

void UDC_Action::StateChange(ECHARACTER_STATE _CurrentState)
{
	currentState = _CurrentState;
	switch (_CurrentState)
	{
	case ECHARACTER_STATE::CASTING:
	case ECHARACTER_STATE::DEAD:
	case ECHARACTER_STATE::STUN:
	case ECHARACTER_STATE::HIT:
	case ECHARACTER_STATE::CHARGING:
		if (ownerPlayer->bIsCrouched) ownerPlayer->UnCrouch();
		ownerPlayer->GetStatComponent()->RemoveMoveSpeed(TEXT("Dash"));
		bIsDashing = false;
		ZoomOut();
		break;
	}
}

void UDC_Action::CameraMode_Default(float _DeltaTime)
{
	//언리얼에서 액터의 요값을 계산할 때 180도가 넘어가면
	//-180도로 돌아가기때문에(쿼터니언 연산 때문)
	// 그 값과 맞춰주기 위해 컨트롤 로테이션의 요값을 따로 설정해준다
	controlYaw = ownerPlayer->GetControlRotation().Yaw;
	if (controlYaw > 180.f)	controlYaw = -(360.f - controlYaw);
	ownerPlayer->bUseControllerRotationYaw = (bIsMoving && bRotationSame);

	//캐릭터의 시선과 플레이어의 카메라가 보는곳이 같다면 무브먼트 모드만 바꿔준다
	if (bRotationSame) {
		if (bIsMoving) {
			currentMovementMode = EPLAYER_MOVEMENT::MOVING;
			PRINTSTR(PLAYER_MOVEMENTMODE, 5.f, FColor::Cyan, TEXT("Moving"));
		}
		else {
			currentMovementMode = EPLAYER_MOVEMENT::IDLE;
			PRINTSTR(PLAYER_MOVEMENTMODE, 5.f, FColor::Cyan, TEXT("Idle"));
		}
		bBeforeSettingYaw = true;
	}

	//다르면서 움직이고 있을땐 그 방향으로 회전해주고
	//움직이지 않을땐 일정 시야각 이상 벗어났을때 회전해준다
	else {
		if (bIsMoving) {
			currentMovementMode = EPLAYER_MOVEMENT::MOVING;
			PRINTSTR(PLAYER_MOVEMENTMODE, 5.f, FColor::Cyan, TEXT("Moving"));
			SetActorRotationYaw(controlYaw, _DeltaTime);
			bBeforeSettingYaw = true;
		}
		else {
			if (GetActorYawRnage(controlYaw, -70.f, 90.f) || !bBeforeSettingYaw) {
				currentMovementMode = EPLAYER_MOVEMENT::ROTATION;
				PRINTSTR(PLAYER_MOVEMENTMODE, 5.f, FColor::Cyan, TEXT("Rotation"));
				if (bBeforeSettingYaw) bBeforeSettingYaw = false;
				else SetActorRotationYaw(controlYaw, _DeltaTime);
			}
		}
	}
}

void UDC_Action::CameraMode_Restore(float _DeltaTime)
{
	//카메라의 로테이션을 원래 위치로 바꿔놓는다
	ownerController->SetControlRotation(UKismetMathLibrary::RInterpTo(ownerPlayer->GetControlRotation(), ownerPlayer->GetActorRotation(), _DeltaTime, 20.f));
	if (bRotationSame)
		currentCameraMode = ECAMERA_MODE::DEFAULT;
}

bool UDC_Action::GetActorYawRnage(float _ControlRotationYaw, float _Min, float _Max) const
{
	//액터의 로테이션 요값이 범위안에 있는지 확인한다
	return (_ControlRotationYaw - ownerPlayer->GetActorRotation().Yaw > _Max) || (_ControlRotationYaw - ownerPlayer->GetActorRotation().Yaw < _Min);
}

void UDC_Action::SetActorRotationYaw(float _TargetRotationYaw, float _DeltaTime)
{
	//액터의 로테이션 요값을 돌려준다
	FRotator targetRotation = FRotator(ownerPlayer->GetActorRotation().Pitch, _TargetRotationYaw, ownerPlayer->GetActorRotation().Roll);
	ownerPlayer->SetActorRotation(UKismetMathLibrary::RInterpTo(ownerPlayer->GetActorRotation(), targetRotation, _DeltaTime, rotationSpeed));
}

void UDC_Action::Fire()
{
	FVector fireLocation = playerCamera->GetComponentLocation();
	FVector tracingEndLocation = (playerCamera->GetForwardVector() * 10000.f) + playerCamera->GetComponentLocation();

	ownerPlayer->GetWeaponComponent()->Fire(fireLocation, tracingEndLocation, bIsZooming);
}

bool UDC_Action::CanParkour()
{
	DFLOG_N(Display);
	//라인트레이스로 캐릭터 앞쪽에 위에서 아래로 라인을 쏴서 파쿠르할 Actor가 있는지 확인하고
	//Actor가 있다면 그 Actor가 적절한 조건을 가지고 있는지 체크한다
	if (currentParkourState != EPARKOUR_STATE::NOT_PARKOUR) return false;

	const UCapsuleComponent* ownerCapsule = GetOwnerCharacter()->GetCapsuleComponent();

	float CapsuleScaledHalfHeight = ownerCapsule->GetScaledCapsuleHalfHeight();
	FVector lineLocation =
		GetOwnerCharacter()->GetActorLocation() +
		(GetOwnerCharacter()->GetActorForwardVector() *
			(ownerCapsule->GetScaledCapsuleRadius() * 2));
	FVector lineStartLoc = lineLocation + FVector(0.f, 0.f, CapsuleScaledHalfHeight);
	FVector lineEndLoc = lineLocation - FVector(0.f, 0.f, CapsuleScaledHalfHeight);
	FHitResult hitResult{};

	if (!UKismetSystemLibrary::LineTraceSingle(
		GetWorld(),
		lineStartLoc,
		lineEndLoc,
		UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility),
		false,
		TArray<AActor*>{GetOwnerCharacter()},
		EDrawDebugTrace::ForDuration,
		hitResult,
		true)) return false;

	return CanParkourToHit(hitResult, ownerCapsule);
}

bool UDC_Action::CanParkourToHit(const FHitResult& _HitResult, const UCapsuleComponent* _Capsule)
{
	DFLOG_N(Display);
	FVector hitLocation = _HitResult.Location;
	FVector hitTraceEnd = _HitResult.TraceEnd;
	FVector hitNormal = _HitResult.Normal;
	float halfHeight = _Capsule->GetScaledCapsuleHalfHeight();
	float radius = _Capsule->GetScaledCapsuleRadius();

	float rangeValue = hitLocation.Z - hitTraceEnd.Z;
	//Actor의 높이가 적절한지 확인한다
	if (!UKismetMathLibrary::InRange_FloatFloat(rangeValue, minParkourHeight, maxParkourHeight)) return false;

	//Actor가 너무 기울어 지진 않았는지 체크한다
	if (hitNormal.Z < GetOwnerCharacter()->GetCharacterMovement()->GetWalkableFloorZ()) return false;

	endLocation = hitLocation + FVector(0.f, 0.f, halfHeight);

	//파쿠르 하고 난 뒤의 공간에 현재 캐릭터가 들어갈만한 공간이 있는지 확인한다
	return !CheckCapsuleCollision(endLocation + FVector(0.f, 0.f, radius), halfHeight, radius);
}

bool UDC_Action::CheckCapsuleCollision(const FVector& _Center, float _HalfHeight, float _Radius)
{
	DFLOG_N(Display);
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	UKismetSystemLibrary::DrawDebugCapsule(
		GetWorld(),
		_Center,
		_HalfHeight,
		_Radius,
		FRotator::ZeroRotator,
		FLinearColor::White,
		5.f,
		1.f);
#endif
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes{};
	TEnumAsByte<EObjectTypeQuery> worldStatic = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic);
	TEnumAsByte<EObjectTypeQuery> pawn = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn);
	TEnumAsByte<EObjectTypeQuery> physicsBody = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody);
	TEnumAsByte<EObjectTypeQuery> vehicle = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Vehicle);
	TEnumAsByte<EObjectTypeQuery> destructible = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Destructible);
	ObjectTypes.Add(worldStatic);
	ObjectTypes.Add(pawn);
	ObjectTypes.Add(physicsBody);
	ObjectTypes.Add(vehicle);
	ObjectTypes.Add(destructible);

	TArray<AActor*> ignoreActors{};
	ignoreActors.Add(GetOwnerCharacter());

	TArray<AActor*> outActors{};

	return UKismetSystemLibrary::CapsuleOverlapActors(
		GetWorld(),
		_Center,
		_Radius,
		_HalfHeight,
		ObjectTypes,
		AActor::StaticClass(),
		ignoreActors,
		outActors);
}

void UDC_Action::SetOwnerInput(bool _CanInput)
{
	DFLOG_N(Display);
	//파쿠르 하는동안 인풋을 할 수 없도록 한다
	if (ownerController == nullptr)return;

	_CanInput ?
		GetOwnerCharacter()->EnableInput(ownerController) :
		GetOwnerCharacter()->DisableInput(ownerController);
}
