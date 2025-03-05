#pragma once

#include "Actor/DA_Actor.h"
#include "Interface/DI_ObjectPoolingActor.h"
#include "DA_HomingTarget.generated.h"

class USphereComponent;

//�߻�ü�� ��������� �����ϴ� ��ġ
UCLASS()
class DEFENDER_API ADA_HomingTarget :
	public ADA_Actor,
	public IDI_ObjectPoolingActor
{
	GENERATED_BODY()

public:

protected:

private:
	//Collision of homing target
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "DefaultConfig", meta = (AllowPrivateAccess = true))
	USphereComponent* collision;

	bool			bIsActive;			//������ǥ�� Ȱ������
	
public:
	ADA_HomingTarget();
	/*
	* Change the active state of a Homing Target
	* @parma[in] _bIsActive If true, HomingTarget is activated.
	*/
	virtual void ActiveSetting_Implementation(bool _bIsActive)override;

protected:
	virtual void CollisionActive_Implementation(bool _bIsActive)override;

private:

};
