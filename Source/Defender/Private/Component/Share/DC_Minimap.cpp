#include "Component/Share/DC_Minimap.h"
#include "Actor/DA_Character.h"
#include "Actor/Player/Controller/DAPC_InGame.h"

#include "Components/SceneCaptureComponent2D.h"
#include "GameFramework/SpringArmComponent.h"
#include "Paper2D/Classes/PaperSpriteComponent.h"
#include "Components/SkeletalMeshComponent.h"

UDC_Minimap::UDC_Minimap()
{
	minimapHeight = 1000.f;
	orthoWidth = 1000.f;
	spriteColor = FLinearColor::Red;
}

void UDC_Minimap::BeginPlay()
{
	Super::BeginPlay();

	//미니맵컴포넌트를 가진 캐릭터가 플레이어라면 미니맵에 표시할 수 있도록 셋팅해준다
	auto playerController = Cast<ADAPC_InGame>(GetOwnerCharacter()->GetController());
	if (playerController != nullptr) {
		minimapSpringArm = NewObject<USpringArmComponent>(this);
		minimapSpringArm->SetupAttachment(GetOwnerCharacter()->GetRootComponent());
		minimapSpringArm->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));
		minimapSpringArm->TargetArmLength = minimapHeight;
		minimapSpringArm->bInheritYaw = false;
		minimapSpringArm->bInheritRoll = false;
		minimapSpringArm->bInheritPitch = false;
		minimapSpringArm->bEnableCameraLag = true;
		minimapSpringArm->RegisterComponent();
		GetOwnerCharacter()->AddInstanceComponent(minimapSpringArm);

		minimapCamera = NewObject<USceneCaptureComponent2D>(this);
		minimapCamera->SetupAttachment(minimapSpringArm);
		minimapCamera->ProjectionType = ECameraProjectionMode::Orthographic;
		minimapCamera->OrthoWidth = orthoWidth;
		minimapCamera->TextureTarget = textureTarget;
		minimapCamera->RegisterComponent();
		GetOwnerCharacter()->AddInstanceComponent(minimapCamera);
	}
}

void UDC_Minimap::InitializeComponent()
{
	Super::InitializeComponent();

	//캐릭터가 미니맵에 표시될 수 있도록 스프라이트 컴포넌트를 생성한다
	GetOwnerCharacter()->GetMesh()->SetHiddenInSceneCapture(true);

	characterMark = NewObject<UPaperSpriteComponent>(this);
	characterMark->SetupAttachment(GetOwnerCharacter()->GetRootComponent());
	characterMark->SetVisibleInSceneCaptureOnly(true);
	characterMark->SetRelativeLocationAndRotation(FVector(0.f, 0.f, 100.f), FRotator(0.f, 270.f, 90.f));
	characterMark->SetRelativeScale3D(FVector(0.3f));
	characterMark->SetSprite(spriteSource);
	characterMark->SetSpriteColor(spriteColor);
	characterMark->RegisterComponent();
	GetOwnerCharacter()->AddInstanceComponent(characterMark);
}
