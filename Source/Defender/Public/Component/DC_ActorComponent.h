#pragma once

#include "../Defender.h"
#include "Components/ActorComponent.h"
#include "DC_ActorComponent.generated.h"

//프로젝트에서 사용되는 컴포넌트
UCLASS(Abstract, NotBlueprintable)
class DEFENDER_API UDC_ActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:

protected:

private:

public:
	UDC_ActorComponent();

protected:
	/**
	 * Initializes the component.  Occurs at level startup or actor spawn. This is before BeginPlay (Actor or Component).
	 * All Components in the level will be Initialized on load before any Actor/Component gets BeginPlay
	 * Requires component to be registered, and bWantsInitializeComponent to be true.
	 */
	virtual void InitializeComponent()override;
	virtual void BeginPlay() override;

private:

};
