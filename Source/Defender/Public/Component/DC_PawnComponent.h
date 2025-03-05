#pragma once

#include "Component/DC_ActorComponent.h"
#include "DC_PawnComponent.generated.h"

class ADA_Character;

//Pawn의 하위클래스에 들어가는 컴포넌트를 만들때 기반이되는 컴포넌트
UCLASS()
class DEFENDER_API UDC_PawnComponent : public UDC_ActorComponent
{
	GENERATED_BODY()
public:

protected:

private:
	//component's owner character
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "OwnerConfig", meta = (AllowPrivateAccess))
	ADA_Character* ownerCharacter;

public:
	UDC_PawnComponent();

	/*
	* Return the owner of the component
	* @return Return the ADA_Character*
	*/
	FORCEINLINE ADA_Character* GetOwnerCharacter()const { return ownerCharacter; }

protected:
	/**
	 * Initializes the component.  Occurs at level startup or actor spawn. This is before BeginPlay (Actor or Component).
	 * All Components in the level will be Initialized on load before any Actor/Component gets BeginPlay
	 * Requires component to be registered, and bWantsInitializeComponent to be true.
	 */
	virtual void InitializeComponent()override;

private:

};
