#include "Component/DC_ActorComponent.h"

UDC_ActorComponent::UDC_ActorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
}

void UDC_ActorComponent::InitializeComponent()
{
	Super::InitializeComponent();
	DFLOG_N(Display);
}

void UDC_ActorComponent::BeginPlay()
{
	Super::BeginPlay();
	DFLOG_N(Display);
	
}
