#pragma once

#include "Component/DC_ActorComponent.h"
#include "DC_InteractionComponent.generated.h"

class ADA_Player;
class ADAPC_InGame;

class USphereComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPlayerInteractionDelegate, ADA_Player*, _InteractionCharacter, ADAPC_InGame*, _InteractionPlayer);

//�÷��̾�� ��ȣ�ۿ��� �����ϵ��� ���ִ� ������Ʈ
UCLASS()
class DEFENDER_API UDC_InteractionComponent : public UDC_ActorComponent
{
	GENERATED_BODY()
public:
	//Delegate called when the player interacts
	UPROPERTY(BlueprintAssignable)
	FOnPlayerInteractionDelegate onPlayerInteraction;

protected:

private:
	//A collision component that represents the range within which the player can interact.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "Interaction", meta = (AllowPrivateAccess = true))
	USphereComponent* interactionCollision;

	//Range of collision
	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "Interaction", meta = (AllowPrivateAccess = true))
	float sphereRadius;

	//Relative position of collision
	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "Interaction", meta = (AllowPrivateAccess = true))
	FVector sphereRelativeLocation;

	TSet<ADA_Player*>			overlappedPlayers;			//���� ��ȣ�ۿ� ������ �÷��̾��

public:
	UDC_InteractionComponent();

protected:
	virtual void BeginPlay()override;
	virtual void InitializeComponent()override;
	virtual void EndPlay(const EEndPlayReason::Type _EndPlayReason)override;

private:
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* _OverlappedComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex, bool _bFromSweep, const FHitResult& _SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* _OverlappedComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex);

	UFUNCTION()
	void OnInteraction(ADA_Player* _InteractionCharacter, ADAPC_InGame* _InteractionPlayer);
};
