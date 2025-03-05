#include "Item/DA_Item.h"
#include "Component/Player/DC_Inventory.h"
#include "Actor/DA_Character.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

ADA_Item::ADA_Item()
{
	rootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	staticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	sphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));

	RootComponent = rootComp;
	staticMeshComp->SetupAttachment(rootComp);
	sphereCollision->SetupAttachment(rootComp);

	itemCount = 1;
}

void ADA_Item::UseItem_Implementation(ADA_Character* _UsedCharacter)
{
	//인터페이스로 아이템 사용로직을 구현한다
	DFLOG(Display, TEXT("%s used %s"), *_UsedCharacter->GetName(), *GetName());
}

void ADA_Item::ActiveSetting_Implementation(bool _bIsActive)
{
	DFLOG_N(Display);
	bIsActive = _bIsActive;

	IDI_ObjectPoolingActor::Execute_MeshActive(this, _bIsActive);
	IDI_ObjectPoolingActor::Execute_CollisionActive(this, _bIsActive);
}

void ADA_Item::BeginPlay()
{
	Super::BeginPlay();

	//아이템 이름으로 테이터테이블에서 아이템의 정보를 셋팅해준다
	currentItemData = itemDataTable->FindRow<FDFItem>(*itemName, TEXT(""));

	DFCHECK(currentItemData != nullptr);

	staticMeshComp->SetStaticMesh(currentItemData->itemMesh);
	for (int32 i = 0; i < currentItemData->itemColor.Num(); ++i)
		staticMeshComp->SetMaterial(i, currentItemData->itemColor[i]);
	staticMeshComp->SetRelativeTransform(currentItemData->itemRelativeTransform);
	sphereCollision->SetSphereRadius(currentItemData->itemCollisionRadius);
	sphereCollision->OnComponentBeginOverlap.AddDynamic(this, &ADA_Item::BeginOverlapEvent);

#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	sphereCollision->SetHiddenInGame(false);

#endif
}

void ADA_Item::MeshActive_Implementation(bool _bIsActive)
{
	staticMeshComp->SetVisibility(_bIsActive, true);
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	sphereCollision->SetHiddenInGame(!_bIsActive);

#endif
}

void ADA_Item::CollisionActive_Implementation(bool _bIsActive)
{
	if (_bIsActive) {
		staticMeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		sphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	else {
		staticMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		sphereCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void ADA_Item::BeginOverlapEvent(UPrimitiveComponent* _OverlappedComp, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex, bool _bFromSweep, const FHitResult& _SweepResult)
{
	DFLOG_N(Display);
	//인벤토리 컴포넌트가 있는 캐릭터라면 아이템을 획득한다
	auto inventory = _OtherActor->FindComponentByClass<UDC_Inventory>();

	if (inventory != nullptr) {
		inventory->AddItem(itemName, itemCount);
		DFLOG(Display, TEXT("%s Get Item : %s"), *_OtherActor->GetName(), *itemName);
		IDI_ObjectPoolingActor::Execute_ActiveSetting(this, false);
	}
}
