#include "Component/Interaction/DC_InteractionComponent.h"
#include "Actor/Player/DA_Player.h"
#include "Actor/Player/Controller/DAPC_InGame.h"
#include "Component/Player/DC_Action.h"

#include "Components/SphereComponent.h"

UDC_InteractionComponent::UDC_InteractionComponent()
{
	sphereRadius = 30.f;
}

void UDC_InteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	interactionCollision->OnComponentBeginOverlap.AddDynamic(this, &UDC_InteractionComponent::OnOverlapBegin);
	interactionCollision->OnComponentEndOverlap.AddDynamic(this, &UDC_InteractionComponent::OnOverlapEnd);
}

void UDC_InteractionComponent::InitializeComponent()
{
	Super::InitializeComponent();

	//컴포넌트 초기화 단계에서 부모 액터에 Sphere콜리전을 하나 만든다
	interactionCollision = NewObject<USphereComponent>(GetOwner());
	interactionCollision->SetupAttachment(GetOwner()->GetRootComponent());
	interactionCollision->SetSphereRadius(sphereRadius);
	interactionCollision->SetRelativeLocation(sphereRelativeLocation);
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	interactionCollision->SetHiddenInGame(false);
#endif
	interactionCollision->RegisterComponent();
	GetOwner()->AddInstanceComponent(interactionCollision);
}

void UDC_InteractionComponent::EndPlay(const EEndPlayReason::Type _EndPlayReason)
{
	Super::EndPlay(_EndPlayReason);

	for (auto player : overlappedPlayers) {
		auto playerController = Cast<ADAPC_InGame>(player->GetController());
		if (playerController != nullptr)
			playerController->RemoveInteractionWidget();
	}

	interactionCollision->OnComponentBeginOverlap.RemoveDynamic(this, &UDC_InteractionComponent::OnOverlapBegin);
	interactionCollision->OnComponentEndOverlap.RemoveDynamic(this, &UDC_InteractionComponent::OnOverlapEnd);
}

void UDC_InteractionComponent::OnOverlapBegin(UPrimitiveComponent* _OverlappedComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex, bool _bFromSweep, const FHitResult& _SweepResult)
{
	auto player = Cast<ADA_Player>(_OtherActor);
	if (player != nullptr &&
		!overlappedPlayers.Contains(player))
	{
		overlappedPlayers.Add(player);
		player->GetActionComponent()->onInteraction.AddDynamic(this, &UDC_InteractionComponent::OnInteraction);
		auto playerController = Cast<ADAPC_InGame>(player->GetController());
		if (playerController != nullptr)
			playerController->ShowInteractionWidget();
	}
}

void UDC_InteractionComponent::OnOverlapEnd(UPrimitiveComponent* _OverlappedComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex)
{
	auto player = Cast<ADA_Player>(_OtherActor);
	if (player != nullptr && overlappedPlayers.Contains(player)) {
		player->GetActionComponent()->onInteraction.RemoveDynamic(this, &UDC_InteractionComponent::OnInteraction);
		overlappedPlayers.Remove(player);
		auto playerController = Cast<ADAPC_InGame>(player->GetController());
		if (playerController != nullptr)
			playerController->RemoveInteractionWidget();
	}
}

void UDC_InteractionComponent::OnInteraction(ADA_Player* _InteractionCharacter, ADAPC_InGame* _InteractionPlayer)
{
	onPlayerInteraction.Broadcast(_InteractionCharacter, _InteractionPlayer);
}
