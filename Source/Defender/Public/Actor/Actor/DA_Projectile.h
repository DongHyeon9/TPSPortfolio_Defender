#pragma once

#include "Actor/DA_Actor.h"
#include "Interface/DI_ObjectPoolingActor.h"
#include "Components/TimeLineComponent.h"
#include "DA_Projectile.generated.h"

class ADA_HomingTarget;
class ADA_Character;

class UStaticMeshComponent;
class USphereComponent;
class UProjectileMovementComponent;
class UNiagaraComponent;
class UNiagaraSystem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHitDelegate, ADA_Projectile*, _HitProjectile);

//������Ʈ���� ���� �߻�ü
UCLASS()
class DEFENDER_API ADA_Projectile :
	public ADA_Actor,
	public IDI_ObjectPoolingActor
{
	GENERATED_BODY()

public:
	//Delegate called when a projectile touches another object
	UPROPERTY(BlueprintAssignable)
	FOnHitDelegate onHit;

protected:

private:
	//Projectile collision determination
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "Projectile", meta = (AllowPrivateAccess = true))
	USphereComponent* collision;

	//projectile mesh
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "Projectile", meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* mesh;

	//Effects while the projectile is flying
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "Projectile", meta = (AllowPrivateAccess = true))
	UNiagaraComponent* tailEffect;

	//Projectile movement component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "Projectile", meta = (AllowPrivateAccess = true))
	UProjectileMovementComponent* projectileMovement;

	//type of projectile
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "CommonSetting", meta = (AllowPrivateAccess = true))
	EPROJETILE_TYPE projectileType;

	//The character who fired the projectile
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "CommonSetting", meta = (AllowPrivateAccess = true))
	ADA_Character* ownerCharacter;

	//Projectile Damage
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "CommonSetting", meta = (AllowPrivateAccess = true))
	float damage;

	//Relative Transform of Projectile Mesh
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "CommonSetting", meta = (AllowPrivateAccess = true))
	FTransform meshRelativeTransform;

	//Area where the projectile deals damage
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "CommonSetting", meta = (AllowPrivateAccess = true))
	float damageRadius;

	//projectile retention time
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "CommonSetting", meta = (AllowPrivateAccess = true))
	float lifeTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "CommonSetting", meta = (AllowPrivateAccess = true))
	TArray<FName> ignoreTags;

	//Effect to be triggered when a projectile collides
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "CommonSetting", meta = (AllowPrivateAccess = true))
	UNiagaraSystem* finishEffect;

	//Size of effect upon collision
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "CommonSetting", meta = (AllowPrivateAccess = true))
	float finishEffectSize;

	//The curve that will be used when the projectile's warhead turns when aiming at the opponent.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HomingSetting", Meta = (AllowPrivateAccess = true))
	UCurveFloat* floatCurve;

	//Actor the projectile will follow
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HomingSetting", Meta = (AllowPrivateAccess = true))
	AActor* targetActor;

	//target point of the projectile
	//If there is a target actor, it is ignored.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HomingSetting", Meta = (AllowPrivateAccess = true))
	FVector targetLocation;

	//Time until the homing function is activated
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HomingSetting", Meta = (AllowPrivateAccess = true))
	float homingDelay;

	//speed of homing
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HomingSetting", Meta = (AllowPrivateAccess = true))
	float homingSpeed;

	UPROPERTY()
	ADA_HomingTarget*		homingTarget;			//�߻�ü�� ���� ��ġ

	FTimeline				timeLine;				//�ε巯�� ȸ���� ���� Ÿ�Ӷ���
	FOnTimelineFloat		onTimeLineUpdate;		//Ÿ�Ӷ��� ������Ʈ �� ���� ��������Ʈ
	FOnTimelineEvent		onTimeLineFinish;		//Ÿ�Ӷ��� ���� �� ���� ��������Ʈ 

	FTimerHandle			homingTimerHandle;		//��������� Ȱ��ȭ�Ǵ� Ÿ�̸� �ڵ�
	FTimerHandle			lifeTimerHandle;		//�߻�ü�� �����ð� Ÿ�̸� �ڵ�

	FVector					lookAtLocation;			//�߻�ü�� �ٶ� ��
	FRotator				startRotation;			//�߻�ü�� ���������� �����ִ� �����̼�

	bool					bIsActive;				//�߻�ü�� Ȱ��ȭ ����

public:
	ADA_Projectile();
	/*
	* Initialize the projectile
	* @param[in] _Initalizer Information required for initialization
	*/
	UFUNCTION(BlueprintCallable, category = "Projectile")
	virtual void InitializeProjectile(const FProjectileInitializer& _Initalizer);

	/*
	* Function for initializing a projectile with a homing function in its blueprint.
	* @param[in] _Initalizer Information required for initialization
	*/
	UFUNCTION(BlueprintCallable, category = "Projectile")
	virtual void InitializeHoming(const FHomingInitializer& _Initalizer);
	/*
	* Activate the projectile according to the settings.
	* Be sure to call InitializeProjectile before use.
	* @param[in] _bIsActive Adjust the status of the projectile to active or inactive.
	*/
	virtual void ActiveSetting_Implementation(bool _bIsActive)override;
	/*
	* Get the collision of the projectile
	* @return return the USphereComponent*
	*/
	FORCEINLINE USphereComponent* GetCollision()const { return collision; }

protected:
	virtual void BeginPlay()override;
	virtual void Tick(float _DeltaTime)override;
	virtual void MeshActive_Implementation(bool _bIsActive)override;
	virtual void CollisionActive_Implementation(bool _bIsActive)override;
	virtual void EndPlay(EEndPlayReason::Type _EndPlayReason)override;

private:
	UFUNCTION()
	void OnHit(UPrimitiveComponent* _HitComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, FVector _NormalImpulse, const FHitResult& _Hit);

	UFUNCTION()
	void TimeLineUpdate(float _OutPut);

	UFUNCTION()
	void TimeLineFinish();

	friend struct FProjectileInitializer;
	friend struct FHomingInitializer;
};

