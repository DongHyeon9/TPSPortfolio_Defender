#include "Actor/Actor/FieldItem/DA_Interaction.h"
#include "Component/Interaction/DC_InteractionComponent.h"
#include "Actor/Player/DA_Player.h"
#include "Actor/Player/Controller/DAPC_InGame.h"

ADA_Interaction::ADA_Interaction()
{
	interactionComp = CreateDefaultSubobject<UDC_InteractionComponent>(TEXT("InteractionComp"));
}

void ADA_Interaction::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	interactionComp->onPlayerInteraction.AddDynamic(this, &ADA_Interaction::Interaction);
}

void ADA_Interaction::EndPlay(const EEndPlayReason::Type _EndPlayReason)
{
	Super::EndPlay(_EndPlayReason);

	if (interactionComp != nullptr)
		interactionComp->onPlayerInteraction.RemoveDynamic(this, &ADA_Interaction::Interaction);
}

void ADA_Interaction::Interaction_Implementation(ADA_Player* _InteractionCharacter, ADAPC_InGame* _InteractionPlayer)
{
	DFLOG_N(Display);
}