#include "Actor/Actor/DA_HomingTarget.h"

#include "Components/SphereComponent.h"

ADA_HomingTarget::ADA_HomingTarget()
{
	collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	RootComponent = collision;
	collision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

void ADA_HomingTarget::ActiveSetting_Implementation(bool _bIsActive)
{
	bIsActive = _bIsActive;
	DFLOG(Display, TEXT("%s : %s"), *GetName(), bIsActive ? "Active" : "Deactive");
	IDI_ObjectPoolingActor::Execute_CollisionActive(this, _bIsActive);
}

void ADA_HomingTarget::CollisionActive_Implementation(bool _bIsActive)
{
	if (_bIsActive)
		collision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	else
		collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}