#include "Actor/Player/DA_Player.h"
#include "Actor/Player/Controller/DAPC_InGame.h"
#include "Component/Player/DC_Weapon.h"
#include "Component/Player/DC_Targeting.h"
#include "Component/Player/DC_Inventory.h"
#include "Component/Share/DC_Stat.h"
#include "Component/Share/DC_AnimMontage.h"
#include "Component/Player/DC_Action.h"
#include "Component/Share/DC_Skill.h"
#include "Skills/Active/DO_ActiveBase.h"
#include "Actor/Player/DOA_Player.h"

#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerInput.h"
#include "Components/CapsuleComponent.h"
#include "Engine/AssetManager.h"

ADA_Player::ADA_Player()
{
	//컴포넌트 생성
	cameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	playerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	inventoryComp = CreateDefaultSubobject<UDC_Inventory>(TEXT("InventoryComp"));
	targetingComp = CreateDefaultSubobject<UDC_Targeting>(TEXT("TargetingComp"));
	weaponComp = CreateDefaultSubobject<UDC_Weapon>(TEXT("WeaponComp"));
	actionComp = CreateDefaultSubobject<UDC_Action>(TEXT("ActionComp"));

	Tags.Add(TEXT("Player"));

	cameraSpringArm->SetupAttachment(RootComponent);
	playerCamera->SetupAttachment(cameraSpringArm);
}

void ADA_Player::SetPlayerCharacter(int32 _CharacterIndex)
{
	//캐릭터 인덱스에 따라 캐릭터를 설정하고
	//에셋을 비동기로 로드한다
	DFCHECK(playerDataTable != nullptr);
	playerData = playerDataTable->FindRow<FDFPlayerData>(*FString::FromInt(_CharacterIndex), TEXT(""));
	DFCHECK(playerData != nullptr);
	characterAssetsToLoad.Add(playerData->characterSkeletalMesh.ToSoftObjectPath());
	characterAssetsToLoad.Add(playerData->animInstance.ToSoftObjectPath());
	characterAssetsToLoad.Add(playerData->characterIcon.ToSoftObjectPath());
	playerCharacterIndex = _CharacterIndex;

	auto montage = GetAnimMontageComponent();
	DFCHECK(montage != nullptr);
	montage->SetAnimMontage(playerData->animMontageMap);

	GetSkillComponent()->SetMotionIndex(playerCharacterIndex);
	assetStreamingHandle =
		UAssetManager::GetStreamableManager().RequestAsyncLoad(
			characterAssetsToLoad,
			FStreamableDelegate::CreateUObject(this,
				&ADA_Player::OnAssetLoadCompleted));
}

void ADA_Player::SetupPlayerInputComponent(UInputComponent* _PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(_PlayerInputComponent);

	//Input Setting 할당
	DFCHECK(actionComp != nullptr);
	_PlayerInputComponent->BindAxis(TEXT("MoveForward"), actionComp, &UDC_Action::MoveForward);
	_PlayerInputComponent->BindAxis(TEXT("MoveRight"), actionComp, &UDC_Action::MoveRight);
	_PlayerInputComponent->BindAxis(TEXT("Look"), actionComp, &UDC_Action::Look);
	_PlayerInputComponent->BindAxis(TEXT("Turn"), actionComp, &UDC_Action::Turn);
	_PlayerInputComponent->BindAxis(TEXT("BaseAttack"), actionComp, &UDC_Action::WeaponAttack);

	_PlayerInputComponent->BindAxis(TEXT("SensitivityUp"), actionComp, &UDC_Action::SensitivityUp);
	_PlayerInputComponent->BindAxis(TEXT("SensitivityDown"), actionComp, &UDC_Action::SensitivityDown);

	_PlayerInputComponent->BindAction(TEXT("Dash"), IE_Pressed, actionComp, &UDC_Action::Dash);
	_PlayerInputComponent->BindAction(TEXT("Dash"), IE_Released, actionComp, &UDC_Action::Walk);

	_PlayerInputComponent->BindAction(TEXT("Crouch"), IE_Pressed, actionComp, &UDC_Action::Crouch);
	_PlayerInputComponent->BindAction(TEXT("Crouch"), IE_Released, actionComp, &UDC_Action::Crouch);

	_PlayerInputComponent->BindAction(TEXT("ChangeCameraMode"), IE_Pressed, actionComp, &UDC_Action::CameraModeFree);
	_PlayerInputComponent->BindAction(TEXT("ChangeCameraMode"), IE_Released, actionComp, &UDC_Action::CameraModeFix);

	_PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, actionComp, &UDC_Action::Jump);
	_PlayerInputComponent->BindAction(TEXT("Jump"), IE_Released, actionComp, &UDC_Action::StopJump);
	_PlayerInputComponent->BindAction(TEXT("Roll"), IE_Pressed, actionComp, &UDC_Action::Roll);

	_PlayerInputComponent->BindAction(TEXT("Interaction"), IE_Pressed, actionComp, &UDC_Action::Interaction);

	if (weaponComp != nullptr) {
		_PlayerInputComponent->BindAction(TEXT("ModeChange"), IE_Pressed, weaponComp, &UDC_Weapon::FireModeChange);
		_PlayerInputComponent->BindAction(TEXT("MouseScrollUp"), IE_Pressed, weaponComp, &UDC_Weapon::WeaponIndexUp);
		_PlayerInputComponent->BindAction(TEXT("MouseScrollDown"), IE_Pressed, weaponComp, &UDC_Weapon::WeaponIndexDown);
		_PlayerInputComponent->BindAction(TEXT("Reload"), IE_Pressed, weaponComp, &UDC_Weapon::Reload);

		_PlayerInputComponent->BindAction(TEXT("Zoom"), IE_Pressed, weaponComp, &UDC_Weapon::ZoomIn);
		_PlayerInputComponent->BindAction(TEXT("Zoom"), IE_Released, weaponComp, &UDC_Weapon::ZoomOut);
		
	}

	if (targetingComp != nullptr) {
		_PlayerInputComponent->BindAction(TEXT("LockOn"), IE_Pressed, targetingComp, &UDC_Targeting::LockOn);
		_PlayerInputComponent->BindAction(TEXT("TargetMoveRight"), IE_Pressed, targetingComp, &UDC_Targeting::TargetMoveRight);
		_PlayerInputComponent->BindAction(TEXT("TargetMoveLeft"), IE_Pressed, targetingComp, &UDC_Targeting::TargetMoveLeft);
	}

	if (inventoryComp != nullptr) {
		_PlayerInputComponent->BindAction(TEXT("Potion"), IE_Pressed, inventoryComp, &UDC_Inventory::UseItem);
	}

	UDC_Skill* skill = GetSkillComponent();

	if (skill != nullptr) {
		_PlayerInputComponent->BindAction(TEXT("Skill"), IE_Pressed, skill, &UDC_Skill::SkillRegistration);
		_PlayerInputComponent->BindAction(TEXT("Skill"), IE_Released, skill, &UDC_Skill::SkillActivation);
	}
}

void ADA_Player::CharacterInitialize()
{
	Super::CharacterInitialize();

	inventoryComp->AddTickPrerequisiteActor(this);
	targetingComp->AddTickPrerequisiteActor(this);
	weaponComp->AddTickPrerequisiteActor(this);
	actionComp->AddTickPrerequisiteActor(this);
}

void ADA_Player::PossessedBy(AController* _NewController)
{
	Super::PossessedBy(_NewController);

	auto playerController = Cast<ADAPC_InGame>(_NewController);
	DFCHECK(playerController != nullptr);
	playerController->SetPrevLevelData();
	playerController->CreateInGameWidgets();
}

void ADA_Player::OnAssetLoadCompleted()
{
	//에셋 로딩이 끝났다면 각각에 맞는 데이터를 설정해주고
	//델리게이트를 호출해줍니다
	DFLOG_N(Display);
	TArray<UObject*> loadedAssets{};
	assetStreamingHandle->GetLoadedAssets(loadedAssets);
	assetStreamingHandle.Reset();

	for (auto loadedAsset : loadedAssets) {

		auto sk_mesh = Cast<USkeletalMesh>(loadedAsset);
		if (sk_mesh != nullptr)
			GetMesh()->SetSkeletalMesh(sk_mesh);

		auto abp_AnimInst = Cast<UAnimBlueprintGeneratedClass>(loadedAsset);
		if (abp_AnimInst != nullptr) {
			GetMesh()->SetAnimInstanceClass(abp_AnimInst);
			auto playAnim = CastChecked<UDO_AnimInstance>(GetMesh()->GetAnimInstance());
			GetAnimMontageComponent()->SetOwnerAnimInstance(playAnim);
			auto playerController = Cast<ADAPC_InGame>(GetController());
			DFCHECK(playerController != nullptr);
			playAnim->onDeadEnd.AddDynamic(playerController, &ADAPC_InGame::ShowDefeatedWidget);
		}

		auto playerIcon = Cast<UTexture2D>(loadedAsset);
		if (playerIcon != nullptr)
			onCharacterChange.Broadcast(playerIcon);
		
	}
	onAssetLoadCompleted.Broadcast();
}
