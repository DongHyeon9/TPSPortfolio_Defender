#include "Actor/Actor/FieldItem/DA_BuffGenerator.h"
#include "Actor/Actor/FieldItem/DA_Buff.h"
#include "Interface/DI_ObjectPoolingActor.h"
#include "Server/DA_GameInstance.h"

#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"

ADA_BuffGenerator::ADA_BuffGenerator()
{
	rootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));

	RootComponent = rootComp;
	meshComp->SetupAttachment(rootComp);
}

void ADA_BuffGenerator::StartGenerator()
{
	//버프를 생성하고 델리게이트를 연결해준다
	spawnedBuff = GetWorld()->SpawnActor<ADA_Buff>(spawnBuffClass,
		GetActorLocation() + spawnRelativeLocation, FRotator::ZeroRotator);

	spawnedBuff->onBuffDeActive.AddDynamic(this, &ADA_BuffGenerator::BuffDeActive);
	spawnedBuff->OnEndPlay.AddDynamic(this, &ADA_BuffGenerator::BuffDestroy);

	BuffActive();
}

void ADA_BuffGenerator::StopGenerator()
{
	//버프를 삭제하고 타이머를 중지한다
	if (spawnedBuff != nullptr)
		spawnedBuff->Destroy();

	if (generatorTimerHandle.IsValid()) {
		GetWorld()->GetTimerManager().ClearTimer(generatorTimerHandle);
		generatorTimerHandle.Invalidate();
	}
}

void ADA_BuffGenerator::BeginPlay()
{
	Super::BeginPlay();

	auto gameInstance = GetGameInstance<UDA_GameInstance>();
	DFCHECK(gameInstance != nullptr);
	gameInstance->RegistTimerHandle(&generatorTimerHandle);

	StartGenerator();
}

void ADA_BuffGenerator::BuffActive()
{
	if (spawnedBuff == nullptr || spawnedBuff->IsActive()) return;

	IDI_ObjectPoolingActor::Execute_ActiveSetting(spawnedBuff, true);
}

void ADA_BuffGenerator::BuffDeActive()
{
	GetWorld()->GetTimerManager().SetTimer(generatorTimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			BuffActive();
		}), spawnCoolDown, false);
}

void ADA_BuffGenerator::BuffDestroy(AActor* _Actor, EEndPlayReason::Type _EndPlayReason)
{
	spawnedBuff->onBuffDeActive.RemoveDynamic(this, &ADA_BuffGenerator::BuffDeActive);
	spawnedBuff->OnEndPlay.RemoveDynamic(this, &ADA_BuffGenerator::BuffDestroy);
}