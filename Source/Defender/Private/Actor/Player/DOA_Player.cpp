#include "Actor/Player/DOA_Player.h"
#include "Actor/Player/DA_Player.h"
#include "DF_Enum.h"
#include "Component/Player/DC_Weapon.h"
#include "Component/Player/DC_Action.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "KismetAnimationLibrary.h"

UDOA_Player::UDOA_Player()
{
	//UPROPERTY변수 초기화
	bIsCrouch = false;
	currentMovementMode = EPLAYER_MOVEMENT::IDLE;
	controlRotation = FRotator::ZeroRotator;
	playerRotation = FRotator::ZeroRotator;
	currentCharacterDirection = 0.f;
	aimingPitch = 0.f;
	aimingYaw = 0.f;
	//spineRotation = FRotator::ZeroRotator;
	leftHand_Effector = FVector::ZeroVector;
	leftHand_JointTarget = FVector::ZeroVector;
	rightHand_Effector = FVector::ZeroVector;
	rightHand_JointTarget = FVector::ZeroVector;
	poseSwapInterpSpeed = 10.f;

	//맴버변수 초기화
	currentCameraMode = ECAMERA_MODE::DEFAULT;
}

void UDOA_Player::SetChargingIndex(int32 _Index)
{
	chargingAnimationIndex = _Index;
}

void UDOA_Player::NativeUpdateAnimation(float _DeltaSeconds)
{
	Super::NativeUpdateAnimation(_DeltaSeconds);

	//캐릭터를 캐스팅한다
	auto playerCharacter = Cast<ADA_Player>(GetOwnerCharacter());
	DFCHECK(playerCharacter != nullptr);

	//각각의 변수를 셋팅해준다
	controlRotation = playerCharacter->GetControlRotation();
	currentMovementMode = playerCharacter->GetActionComponent()->GetMovementMode();
	playerRotation = playerCharacter->GetActorRotation();
	currentCharacterDirection = UKismetAnimationLibrary::CalculateDirection(GetOwnerCharacter()->GetVelocity(), GetOwnerCharacter()->GetActorRotation());
	currentCameraMode = playerCharacter->GetActionComponent()->GetCameraMode();
	bIsCrouch = playerCharacter->bIsCrouched;
	currentWeaponData = &playerCharacter->GetWeaponComponent()->GetCurrentWeaponData().weaponData;

	CalculateAiming(playerCharacter);

	if (currentWeaponData == nullptr)return;
	
	playerCharacter->GetActionComponent()->IsZooming() ?
		SetWeaponIK(currentWeaponData->zoomState, _DeltaSeconds) :
		SetWeaponIK(currentWeaponData->normalState, _DeltaSeconds);
}

void UDOA_Player::CalculateAiming(const ADA_Player* _PlayerCharacter)
{
	//카메라 모드가 Free면 리턴한다
	if (currentCameraMode == ECAMERA_MODE::FREE) return;

	//요값을 특정범위로 맵핑한다
	aimingYaw = UKismetMathLibrary::MapRangeClamped(playerRotation.Yaw - _PlayerCharacter->GetActionComponent()->GetControlYaw(), -90.f, 135.f, 0.8f, 0.f);

	//피치값을 셋팅해준다
	float controlPitch = controlRotation.Pitch;
	if (controlPitch > 180.f) controlPitch -= 360.f;

	aimingPitch = UKismetMathLibrary::Clamp(controlPitch, -90.f, 90.f);
}

void UDOA_Player::SetWeaponIK(const FWeaponIK& _TargetState, float _DeltaTime)
{
	//무기상태에 따라 손 위치를 선형이동한다
	leftHand_Effector = UKismetMathLibrary::VInterpTo(leftHand_Effector, _TargetState.leftHand_Effector, _DeltaTime, poseSwapInterpSpeed);
	leftHand_JointTarget = UKismetMathLibrary::VInterpTo(leftHand_JointTarget, _TargetState.leftHand_JointTarget, _DeltaTime, poseSwapInterpSpeed);
	rightHand_Effector = UKismetMathLibrary::VInterpTo(rightHand_Effector, _TargetState.rightHand_Effector, _DeltaTime, poseSwapInterpSpeed);
	rightHand_JointTarget = UKismetMathLibrary::VInterpTo(rightHand_JointTarget, _TargetState.rightHand_JointTarget, _DeltaTime, poseSwapInterpSpeed);
}

void UDOA_Player::AnimNotify_Reload()
{
	onReload.Broadcast();
}
