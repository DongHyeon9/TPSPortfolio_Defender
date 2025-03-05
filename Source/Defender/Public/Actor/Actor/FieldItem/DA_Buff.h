#pragma once

#include "Actor/DA_Actor.h"
#include "Interface/DI_ObjectPoolingActor.h"
#include "DA_Buff.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBuffDeActiveDelegate);

class ADA_Player;

class USphereComponent;
class UStaticMeshComponent;
class UNiagaraComponent;

//플레이어에게 특수한 효과를 주는 액터
UCLASS(Abstract)
class DEFENDER_API ADA_Buff : 
	public ADA_Actor,
	public IDI_ObjectPoolingActor
{
	GENERATED_BODY()
public:
	//Delegate called when the player acquires a buff
	UPROPERTY(BlueprintAssignable)
	FOnBuffDeActiveDelegate onBuffDeActive;

protected:

private:
	//Basic root component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "Buff DefaultConfig", meta = (AllowPrivateAccess = true))
	USceneComponent* rootComp;

	//Collision to detect collision with player
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "Buff DefaultConfig", meta = (AllowPrivateAccess = true))
	USphereComponent* collisionComp;

	//Effect that visually shows the buff
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "Buff DefaultConfig", meta = (AllowPrivateAccess = true))
	UNiagaraComponent* niagaraComp;

	bool				bIsActive;				//버프의 활성상태

public:
	ADA_Buff();
	/*
	* Activates the buff
	* @param[in] _bIsActive Decide whether to activate or deactivate the buff.
	*/
	virtual void ActiveSetting_Implementation(bool _bIsActive)override;
	/*
	* Returns whether the buff is active
	* @return returns bool
	*/
	FORCEINLINE bool IsActive()const { return bIsActive; }

protected:
	virtual void BeginPlay()override;
	virtual void MeshActive_Implementation(bool _bIsActive)override;
	virtual void CollisionActive_Implementation(bool _bIsActive)override;

	/*
	* Function called when the player acquires a buff.
	* Implementation is done in a subclass.
	* @param[in] _BuffTarget Player character who received the buff
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, category = "Buff", meta = (HideSelfPin = true))
	void BuffOn(ADA_Player* _BuffTarget);
	void BuffOn_Implementation(ADA_Player* _BuffTarget);

private:
	UFUNCTION()
	void OnOverlapped(UPrimitiveComponent* _OverlappedComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex, bool _bFromSweep, const FHitResult& _SweepResult);
};
