#include "Actor/Actor/FieldItem/DA_Buff.h"
#include "Actor/Player/DA_Player.h"

#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"

ADA_Buff::ADA_Buff()
{
	rootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	collisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	niagaraComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComp"));

	RootComponent = rootComp;
	collisionComp->SetupAttachment(rootComp);
	niagaraComp->SetupAttachment(rootComp);
}

void ADA_Buff::ActiveSetting_Implementation(bool _bIsActive)
{
	bIsActive = _bIsActive;

	DFLOG(Display, TEXT("%s %s"), *GetName(), bIsActive ? "Active" : "Deactive");

	IDI_ObjectPoolingActor::Execute_CollisionActive(this, _bIsActive);
	IDI_ObjectPoolingActor::Execute_MeshActive(this, _bIsActive);
}

void ADA_Buff::BeginPlay()
{
	Super::BeginPlay();

	collisionComp->OnComponentBeginOverlap.AddDynamic(this, &ADA_Buff::OnOverlapped);

#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	collisionComp->SetHiddenInGame(false);

#endif
}

void ADA_Buff::BuffOn_Implementation(ADA_Player* _BuffTarget)
{
	DFLOG_N(Display);
}

void ADA_Buff::MeshActive_Implementation(bool _bIsActive)
{
	IDI_ObjectPoolingActor::MeshActive_Implementation(_bIsActive);

	niagaraComp->SetVisibility(_bIsActive);
}

void ADA_Buff::CollisionActive_Implementation(bool _bIsActive)
{
	IDI_ObjectPoolingActor::CollisionActive_Implementation(_bIsActive);

	_bIsActive ?
		collisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly) :
		collisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	collisionComp->SetHiddenInGame(!_bIsActive);

#endif
}

void ADA_Buff::OnOverlapped(UPrimitiveComponent* _OverlappedComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex, bool _bFromSweep, const FHitResult& _SweepResult)
{
	//충돌한 액터가 플레이어면 버프를 활성화 한다
	auto player = Cast<ADA_Player>(_OtherActor);
	if (player != nullptr) {
		DFLOG(Display, TEXT("%s Take : %s"), *player->GetName(), *GetName());
		BuffOn(player);
		onBuffDeActive.Broadcast();
		IDI_ObjectPoolingActor::Execute_ActiveSetting(this, false);
	}
}
