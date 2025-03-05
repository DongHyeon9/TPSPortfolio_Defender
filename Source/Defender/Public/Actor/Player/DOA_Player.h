#pragma once

#include "Actor/DO_AnimInstance.h"
#include "DOA_Player.generated.h"

enum class EPLAYER_MOVEMENT : uint8;
class ADA_Player;

struct FWeaponIK;
struct FDFWeaponData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReloadDelegate);

//�÷��̾� ĳ���Ϳ� ���Ǵ� �ִ��ν��Ͻ�
UCLASS(Blueprintable, BlueprintType)
class DEFENDER_API UDOA_Player : public UDO_AnimInstance
{
	GENERATED_BODY()

public:
	//Delegate called when performing a reload
	UPROPERTY(BlueprintAssignable)
	FOnReloadDelegate onReload;

protected:

private:
	//Check if the player is crouched
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "CharacterState", meta = (AllowPrivateAccess = true))
	bool bIsCrouch;

	//Player's current movement state
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "CharacterState", meta = (AllowPrivateAccess = true))
	EPLAYER_MOVEMENT currentMovementMode;

	//Player's current control rotation
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "CharacterRotation", meta = (AllowPrivateAccess = true))
	FRotator controlRotation;

	//Rotation of Actors controlled by the player
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "CharacterRotation", meta = (AllowPrivateAccess = true))
	FRotator playerRotation;

	//Degree of the angle betwee velocity and Rotation forward vector The range of return will be from [-180, 180], and this can be used to feed blendspace directional value
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "CharacterRotation", meta = (AllowPrivateAccess = true))
	float currentCharacterDirection;

	//Pitch value in the current player's direction
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "CharacterAiming", meta = (AllowPrivateAccess = true))
	float aimingPitch;

	//Yaw value in the current player's direction
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "CharacterAiming", meta = (AllowPrivateAccess = true))
	float aimingYaw;

	//Position of the effector on the left arm
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "Weapon IK", meta = (AllowPrivateAccess = true))
	FVector leftHand_Effector;

	//Position of the joint target on the left arm
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "Weapon IK", meta = (AllowPrivateAccess = true))
	FVector leftHand_JointTarget;

	//Position of the effector on the right arm
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "Weapon IK", meta = (AllowPrivateAccess = true))
	FVector rightHand_Effector;

	//Position of the joint target on the right arm
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "Weapon IK", meta = (AllowPrivateAccess = true))
	FVector rightHand_JointTarget;

	//The speed at which the arm moves when zooming in or zooming out
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "Weapon IK", meta = (AllowPrivateAccess = true))
	float poseSwapInterpSpeed;

	//Index of preparatory animation
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "Skill Cast", meta = (AllowPrivateAccess = true))
	int32 chargingAnimationIndex;

	ECAMERA_MODE			currentCameraMode;			//���� �÷��̾��� ī�޶� ���
	const FDFWeaponData*	currentWeaponData;			//���� ������ ������ ���̺� ��
	FScriptDelegate			weaponChange;				//���Ⱑ �ٲ� ȣ��Ǵ� ��������Ʈ

public:
	UDOA_Player();

	/*
	* Set up the preparatory animation
	* @param[in] _Index Index to set
	*/
	void SetChargingIndex(int32 _Index);

protected:
	virtual void NativeUpdateAnimation(float _DeltaSeconds)override;

private:
	//���� ȣ�� �Լ�

	void CalculateAiming(const ADA_Player* _PlayerCharacter);
	void SetWeaponIK(const FWeaponIK& _TargetState, float _DeltaTime);

	UFUNCTION()
	void AnimNotify_Reload();
};
