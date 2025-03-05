#pragma once

#include "Component/DC_PawnComponent.h"
#include "DC_Action.generated.h"

class ADA_Player;
class ADAPC_InGame;
class UDC_Stat;

class UCameraComponent;
class USpringArmComponent;
class UCharacterMovementComponent;
class UCapsuleComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInteractionDelegate, ADA_Player*, _InteractionCharacter, ADAPC_InGame*, _InteractionPlayer);

//�÷��̾��� �ൿ(�̵�, ī�޶�����, ���� , ����, �޸��� ��)�� �����ϴ� ������Ʈ
UCLASS()
class DEFENDER_API UDC_Action : public UDC_PawnComponent
{
	GENERATED_BODY()

public:
	//Delegate called when interacting
	UPROPERTY(BlueprintAssignable)
	FOnInteractionDelegate onInteraction;

protected:

private:
	//Distance from player to camera when zoomed in
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAccess = true))
	float zoomInArmLength;

	//Distance from player to camera when zoomed out
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAccess = true))
	float zoomOutArmLength;

	//The speed at which the camera moves when switching between zooming in and zooming out
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAccess = true))
	float cameraArmLengthInterpSpeed;

	//Sensitivity of the player's mouse
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAccess = true))
	float sensitivity;

	//Distance between player character and camera
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAccess = true))
	float targetArmLength;

	//The speed of rotation when the player turns
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "PlayerMovement", meta = (AllowPrivateAccess = true))
	float rotationSpeed;

	//Minimum height for parkour
	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "ParkourSetting", meta = (AllowPrivateAccess = true))
	float minParkourHeight;

	//Maximum height for parkour
	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "ParkourSetting", meta = (AllowPrivateAccess = true))
	float maxParkourHeight;

	//Parkour progress speed
	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "ParkourSetting", meta = (AllowPrivateAccess = true))
	float parkourSpeed;

	//Parkour progress speed
	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "ParkourSetting", meta = (AllowPrivateAccess = true))
	float rollDistance;

	float							parkourProgress;			//������ ����� �ۼ�Ʈ
	EPARKOUR_STATE					currentParkourState;		//���� ���� ����
	FVector							startLocation;				//���� ���� ��ġ
	FVector							endLocation;				//���� ���� ����
	bool							bSweep;						//������ �浹�� üũ�Ұ��� ����

	float							controlYaw;					//��Ʈ�� �����̼��߿� ��(Z)��

	ECAMERA_MODE					currentCameraMode;			//���� ī�޶� ���

	bool							bIsRightMoving;				//�¿�� �����̰� �ִ°�
	bool							bIsForwardMoving;			//���ķ� �����̰� �ִ°�
	bool							bIsMoving;					//���� �̵� �� �ΰ�
	bool							bRotationSame;				//ĳ������ ������ �÷��̾��� ������ ��ġ�ϴ°�
	bool							bBeforeSettingYaw;			//���ڸ� ȸ���ϱ����� �䰪�� �����ߴ°�

	bool							bIsZooming;					//���� �����ΰ�
	bool							bIsDashing;					//�޸��� ���ΰ�
	bool							bCanAttack;					//������ ������ �����ΰ�
	bool							bForceRotation;				//ĳ������ �����̼��� ������ �ٲ۴�
	EPLAYER_MOVEMENT				currentMovementMode;		//���� �����Ʈ ���

	ADA_Player*						ownerPlayer;				//������ �÷��̾� ĳ����
	ADAPC_InGame*					ownerController;			//������ �÷��̾� ��Ʈ�ѷ�
	UCameraComponent*				playerCamera;				//�÷��̾��� ī�޶�
	USpringArmComponent*			cameraSpringArm;			//�÷��̾��� ī�޶��� ��������
	UCharacterMovementComponent*	playerMovement;				//�÷��̾��� �����Ʈ ������Ʈ
	ECHARACTER_STATE				currentState;				//�÷��̾��� ���� ����

public:
	UDC_Action();
	/*
	* Interact with interactable actors
	*/
	void Interaction();
	/*
	* player runs
	*/
	void Dash();
	/*
	* player walks
	*/
	void Walk();
	/*
	* If the player is crouching, he will stand up, and if he is standing up, he will crouch.
	*/
	void Crouch();
	/*
	* Zoom in. Bullet accuracy increases
	*/
	void ZoomIn();
	/*
	* Zoom out. Bullet accuracy is low
	*/
	void ZoomOut();
	/*
	* You can rotate the camera freely
	*/
	void CameraModeFree();
	/*
	* The camera is fixed at a certain range behind the player.
	*/
	void CameraModeFix();
	/*
	* Axis input forward and backward movement
	*/
	void MoveForward(float _Value);
	/*
	* Axis input left and right movement
	*/
	void MoveRight(float _Value);
	/*
	* Move camera up and down
	*/
	void Look(float _Value);
	/*
	* Left and right camera movement
	*/
	void Turn(float _Value);
	/*
	* Increase mouse sensitivity
	*/
	void SensitivityUp(float _Value);
	/*
	* Lower mouse sensitivity
	*/
	void SensitivityDown(float _Value);
	/*
	* Attack with a weapon
	*/
	void WeaponAttack(float _Value);
	/*
	* If parkour is possible, parkour. If not, jump.
	*/
	void Jump();
	/*
	* stop jumping
	*/
	void StopJump();
	/*
	* The player rolls. You become invincible while rolling.
	*/
	void Roll();
	/*
	* Get the Yaw value from the player controller's rotation.
	* @return Return float
	*/
	FORCEINLINE float GetControlYaw() const { return controlYaw; }
	/*
	* Return the player's current movement mode
	* @return player's current movement mode
	*/
	FORCEINLINE EPLAYER_MOVEMENT GetMovementMode()const { return currentMovementMode; }
	/*
	* Return the player's current camera mode
	* @return player's current camera mode
	*/
	FORCEINLINE ECAMERA_MODE GetCameraMode()const { return currentCameraMode; }
	/*
	* Return the player's current zooming state
	* @return player's current zooming state
	*/
	FORCEINLINE bool IsZooming()const { return bIsZooming; }
	/*
	* Gets the sensitivity of the current player
	* @return Returns float
	*/
	FORCEINLINE float GetSensitivity()const { return sensitivity; }
	/*
	* Sets the player's current sensitivity
	* @param[in] _NewSensitivity Sensitivity to set
	*/
	void SetSensitivity(float _NewSensitivity);

protected:
	virtual void InitializeComponent()override;
	virtual void TickComponent(float _DeltaTime, ELevelTick _TickType, FActorComponentTickFunction* _ThisTickFunction)override;
	virtual void BeginPlay()override;

private:
	//���� ȣ�� �Լ�


	UFUNCTION()
	void StateChange(ECHARACTER_STATE _CurrentState);

	void CameraMode_Default(float _DeltaTime);
	void CameraMode_Restore(float _DeltaTime);
	bool GetActorYawRnage(float _ControlRotationYaw, float _Min, float _Max)const;
	void SetActorRotationYaw(float _TargetRotationYaw, float _DeltaTime);
	void Fire();

	bool CanParkour();
	bool CanParkourToHit(const FHitResult& _HitResult, const UCapsuleComponent* _Capsule);
	bool CheckCapsuleCollision(const FVector& _Center, float _HalfHeight, float _Radius);
	void SetOwnerInput(bool _CanInput);
};
