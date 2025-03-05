#include "Component/DC_PawnComponent.h"
#include "Actor/DA_Character.h"

UDC_PawnComponent::UDC_PawnComponent()
{

}

void UDC_PawnComponent::InitializeComponent()
{
	Super::InitializeComponent();

	ownerCharacter = Cast<ADA_Character>(GetOwner());
	DFCHECK(ownerCharacter != nullptr);
}