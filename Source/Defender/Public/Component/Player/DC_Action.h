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

//플레이어의 행동(이동, 카메라조절, 점프 , 파쿠르, 달리기 등)을 관리하는 컴포넌트
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

	float							parkourProgress;			//파쿠르가 진행된 퍼센트
	EPARKOUR_STATE					currentParkourState;		//현재 파쿠르 상태
	FVector							startLocation;				//파쿠르 시작 위치
	FVector							endLocation;				//파쿠르 종료 지점
	bool							bSweep;						//파쿠르중 충돌을 체크할건지 결정

	float							controlYaw;					//컨트롤 로테이션중에 요(Z)값

	ECAMERA_MODE					currentCameraMode;			//현재 카메라 모드

	bool							bIsRightMoving;				//좌우로 움직이고 있는가
	bool							bIsForwardMoving;			//전후로 움직이고 있는가
	bool							bIsMoving;					//현재 이동 중 인가
	bool							bRotationSame;				//캐릭터의 시점과 플레이어의 시점이 일치하는가
	bool							bBeforeSettingYaw;			//제자리 회전하기전에 요값을 설정했는가

	bool							bIsZooming;					//줌인 상태인가
	bool							bIsDashing;					//달리는 중인가
	bool							bCanAttack;					//공격이 가능한 상태인가
	bool							bForceRotation;				//캐릭터의 로테이션을 강제로 바꾼다
	EPLAYER_MOVEMENT				currentMovementMode;		//현재 무브먼트 모드

	ADA_Player*						ownerPlayer;				//소유한 플레이어 캐릭터
	ADAPC_InGame*					ownerController;			//소유한 플레이어 컨트롤러
	UCameraComponent*				playerCamera;				//플레이어의 카메라
	USpringArmComponent*			cameraSpringArm;			//플레이어의 카메라의 스프링암
	UCharacterMovementComponent*	playerMovement;				//플레이어의 무브먼트 컴포넌트
	ECHARACTER_STATE				currentState;				//플레이어의 현재 상태

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
	//내부 호출 함수


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
