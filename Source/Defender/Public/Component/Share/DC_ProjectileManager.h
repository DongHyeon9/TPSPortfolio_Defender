#pragma once

#include "Component/DC_PawnComponent.h"
#include "DC_ProjectileManager.generated.h"

class ADA_Projectile;

//캐릭터의 발사체를 오브젝트 풀링으로 관리하는 컴포넌트
UCLASS(ClassGroup = (DC_Share), meta = (BlueprintSpawnableComponent))
class DEFENDER_API UDC_ProjectileManager : public UDC_PawnComponent
{
	GENERATED_BODY()
public:

protected:

private:
	//Number of projectiles to generate
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, category = "ProjectileManager", meta = (AllowPrivateAccess = true))
	int32 poolSize;

	TQueue<ADA_Projectile*>		projectilePool;		//생성된 발사체를 담는 큐

public:
	UDC_ProjectileManager();
	/*
	* The programmer brings a projectile to the desired location.
	* @param[in] _Location Where to get it
	* @param[in] _Rotation rotation of the projectile
	* @return Pointer to get projectile
	*/
	UFUNCTION(BlueprintCallable, category = "ProjectileManager")
	ADA_Projectile* TakeProjectile(FVector _Location, FRotator _Rotation);
	/*
	* Resets the created projectile
	*/
	UFUNCTION(BlueprintCallable, category = "ProjectileManager")
	void ResetProjectile();
	/*
	* Creates additional projectiles as desired.
	* @param[in] _NewSize Number of additional projectiles to be created
	*/
	void AddProjectile(int32 _NewSize);

protected:
	virtual void BeginPlay() override;

private:
	void Expand();
	void SpawnProjectile();

	UFUNCTION()
	void OnHitEnd(ADA_Projectile* _EndProjectile);

	UFUNCTION()
	void OnProjectileEndPlay(AActor* _Actor, EEndPlayReason::Type _EndPlayReason);
};

