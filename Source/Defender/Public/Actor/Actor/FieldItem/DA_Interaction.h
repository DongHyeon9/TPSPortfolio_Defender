#pragma once

#include "Actor/DA_Actor.h"
#include "DA_Interaction.generated.h"

class UDC_InteractionComponent;
class ADA_Player;
class ADAPC_InGame;

//플레이어와 상호작용 가능한 액터의 최상위 클래스
UCLASS()
class DEFENDER_API ADA_Interaction : public ADA_Actor
{
	GENERATED_BODY()
public:

protected:

private:
	//Components needed to interact with the player
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "InteractionActor DefaultConfig", meta = (AllowPrivateAccess=true))
	UDC_InteractionComponent* interactionComp;

public:
	ADA_Interaction();

protected:
	virtual void PostInitializeComponents()override;
	virtual void EndPlay(const EEndPlayReason::Type _EndPlayReason)override;

	/*
	* Function called when interacting with the player. 
	* Implementation is done in a subclass
	* @param[in] _InteractionCharacter Characters interacted with
	* @param[in] _InteractionPlayer Players Interacted
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, category = "Interaction", meta = (HideSelfPin = true))
	void Interaction(ADA_Player* _InteractionCharacter, ADAPC_InGame* _InteractionPlayer);
	void Interaction_Implementation(ADA_Player* _InteractionCharacter, ADAPC_InGame* _InteractionPlayer);

private:

};
