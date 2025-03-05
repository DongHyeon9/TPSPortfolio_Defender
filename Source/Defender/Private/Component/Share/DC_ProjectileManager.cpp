#include "Component/Share/DC_ProjectileManager.h"
#include "Interface/DI_ObjectPoolingActor.h"
#include "Actor/Actor/DA_Projectile.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

UDC_ProjectileManager::UDC_ProjectileManager()
{
	
}

ADA_Projectile* UDC_ProjectileManager::TakeProjectile(FVector _Location, FRotator _Rotation)
{
	//Ǯ�� ����ִٸ� Ȯ���մϴ�
	if (projectilePool.IsEmpty())
		Expand();
	
	//�߻�ü�� �����ɴϴ�
	auto projectile = *projectilePool.Peek();
	projectilePool.Pop();
	projectile->TeleportTo(_Location, _Rotation);
	return projectile;
}

void UDC_ProjectileManager::ResetProjectile()
{
	//��� �߻�ü�� �����մϴ�
	while (!projectilePool.IsEmpty()) {
		auto projectile = *projectilePool.Peek();
		projectilePool.Pop();
		if (projectile != nullptr)
			projectile->Destroy();
	}
	poolSize = 0;
}

void UDC_ProjectileManager::AddProjectile(int32 _NewSize)
{
	int32 currentSize = poolSize;
	poolSize += _NewSize;
	for (int32 i = currentSize; i < poolSize; ++i)
		SpawnProjectile();
}

void UDC_ProjectileManager::Expand()
{
	//�߻�ü�� �߰��� �����մϴ�
	int32 currentSize = poolSize++;
	poolSize *= 1.5;
	for (int32 i = currentSize; i < poolSize; ++i)
		SpawnProjectile();	
}

void UDC_ProjectileManager::SpawnProjectile()
{
	FTransform spawnTransform(FRotator::ZeroRotator, FVector::ZeroVector);
	auto projectile = GetWorld()->SpawnActorDeferred<ADA_Projectile>(ADA_Projectile::StaticClass(), spawnTransform);

	if (projectile != nullptr &&
		projectile->Implements<UDI_ObjectPoolingActor>()) {
		IDI_ObjectPoolingActor::Execute_ActiveSetting(projectile, false);
		projectile->onHit.AddDynamic(this, &UDC_ProjectileManager::OnHitEnd);
		projectile->OnEndPlay.AddDynamic(this, &UDC_ProjectileManager::OnProjectileEndPlay);
		projectilePool.Enqueue(projectile);
		projectile->FinishSpawning(spawnTransform);
	}
}

void UDC_ProjectileManager::BeginPlay()
{
	Super::BeginPlay();

	for (int32 i = 0; i < poolSize; ++i)
		SpawnProjectile();
}

void UDC_ProjectileManager::OnHitEnd(ADA_Projectile* _EndProjectile)
{
	//�Ҹ�� �߻�ü�� �ٽ� ť�� �����ɴϴ�
	DFLOG_N(Display);
	projectilePool.Enqueue(_EndProjectile);
}

void UDC_ProjectileManager::OnProjectileEndPlay(AActor* _Actor, EEndPlayReason::Type _EndPlayReason)
{
	DFLOG_N(Display);
	auto projectile = Cast<ADA_Projectile>(_Actor);
	if (projectile != nullptr) {
		projectile->onHit.RemoveDynamic(this, &UDC_ProjectileManager::OnHitEnd);
		projectile->OnEndPlay.RemoveDynamic(this, &UDC_ProjectileManager::OnProjectileEndPlay);
	}
}