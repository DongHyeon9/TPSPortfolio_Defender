#include "DF_Struct.h"
#include "Actor/Actor/DA_Projectile.h"
#include "Actor/DA_Character.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"

void FProjectileInitializer::ApplySetting(ADA_Projectile* _Projectile)const
{
	_Projectile->mesh->SetStaticMesh(mesh);
	_Projectile->mesh->SetRelativeTransform(meshRelativeTransform);
	_Projectile->projectileMovement->InitialSpeed = initializeSpeed;
	_Projectile->projectileMovement->MaxSpeed = maxSpeed;
	_Projectile->projectileMovement->ProjectileGravityScale = gravityScale;
	_Projectile->collision->SetSphereRadius(sphereRadius);
	_Projectile->projectileType = projectileType;
	_Projectile->damage = damage;
	_Projectile->damageRadius = damageRadius;
	_Projectile->lifeTime = lifeTime;
	_Projectile->ignoreTags = ignoreTags;
	_Projectile->ownerCharacter = ownerCharacter;
	_Projectile->tailEffect->SetAsset(tailEffect);
	_Projectile->tailEffect->SetRelativeScale3D(FVector(tailEffectSize));
	_Projectile->finishEffect = finishEffect;
	_Projectile->finishEffectSize = finishEffectSize;
}


void FHomingInitializer::ApplySetting(ADA_Projectile* _Projectile)const
{
	FProjectileInitializer::ApplySetting(_Projectile);

	_Projectile->floatCurve = floatCurve;
	_Projectile->targetActor = targetActor;
	_Projectile->targetLocation = targetLocation;
	_Projectile->homingDelay = homingDelay;
	_Projectile->homingSpeed = homingSpeed;
}