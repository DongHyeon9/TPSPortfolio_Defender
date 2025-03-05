#include "Component/Player/DC_Weapon.h"
#include "Actor/Player/DA_Player.h"
#include "Actor/Player/Controller/DAPC_InGame.h"
#include "Component/Share/DC_AnimMontage.h"
#include "Component/Share/DC_Stat.h"
#include "Component/Player/DC_Action.h"
#include "Widget/InGame/DWP_Scope.h"
#include "Component/Share/DC_ProjectileManager.h"
#include "Actor/Actor/DA_Projectile.h"
#include "Actor/Player/DOA_Player.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Components/SphereComponent.h"
#include "Engine/AssetManager.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Kismet/GameplayStatics.h"

UDC_Weapon::UDC_Weapon()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UDC_Weapon::ZoomIn()
{
	switch (statComp->GetState())
	{
	case ECHARACTER_STATE::NORMAL:{
		if (currentScopeWidget != nullptr &&
			currentScopeWidget->ShowInViewport(VO_SCOPE)) {
			ownerPlayer->GetPlayerCameraComponent()->SetFieldOfView(30.f);
			ownerPlayer->GetMesh()->SetOwnerNoSee(true);
			weaponSkeletalComponent->SetOwnerNoSee(true);
			ownerPlayerController->RemoveMainWidget();
		}
		break;
	}				
	}
	ownerPlayer->GetActionComponent()->ZoomIn();
}

void UDC_Weapon::ZoomOut()
{
	if (currentScopeWidget != nullptr && currentScopeWidget->IsInViewport())
		RemoveScopeWidget();
	else
		ownerPlayer->GetActionComponent()->ZoomOut();
}

bool UDC_Weapon::SetWeapon(const FString& _WeaponName)
{
	DFLOG_N(Display);
	DFCHECK(weaponMap.Contains(_WeaponName), false);

	switch (statComp->GetState())
	{
	case ECHARACTER_STATE::NORMAL:{
	ownerPlayer->GetAnimMontageComponent()->StopAnimMontage();

	//만약 소유중이 아니라면 실패를 반환한다
	if (weaponMap[_WeaponName].ownedState == EWEAPON_OWNED_STATE::NONE_OWNED) return false;

	//셋팅하는 웨펀과 현재 인덱스를 맞춰준다
	onWeaponChange.Broadcast(CURRENT_WEAPON_NAME, EWEAPON_OWNED_STATE::OWNED);
	weaponMap[CURRENT_WEAPON_NAME].ownedState = EWEAPON_OWNED_STATE::OWNED;
	for (int32 i = 0; i < weaponDataArray.Num(); ++i)
		if (weaponDataArray[i]->weaponName == _WeaponName) {
			currentWeaponIndex = i;
			break;
		}

	//에셋과 데이터테이블의 웨펀 정보를 셋팅해주고
	//델리게이트를 호출해 준다
	//비동기로 에셋을 로드한다
	if (assetStreamingHandle.IsValid() &&
		assetStreamingHandle->IsLoadingInProgress()) {
		assetStreamingHandle->CancelHandle();
		assetStreamingHandle.Reset();
	}

	weaponAssetToLoad = CURRENT_WEAPON->weaponSkeletalMesh.ToSoftObjectPath();
	DFCHECK(weaponAssetToLoad.IsValid(), false);
	assetStreamingHandle =
		UAssetManager::GetStreamableManager().RequestAsyncLoad(
			weaponAssetToLoad,
			FStreamableDelegate::CreateUObject(this,
				&UDC_Weapon::OnAssetLoadCompleted));
	weaponMap[_WeaponName].ownedState = EWEAPON_OWNED_STATE::EQUIP;
	weaponCoolDown = 0.f;

	onWeaponChange.Broadcast(_WeaponName, EWEAPON_OWNED_STATE::EQUIP);
	onMagazineChange.Broadcast(weaponMap[_WeaponName]);
	onFireSelectorChange.Broadcast(weaponMap[_WeaponName].fireMode);

	//탄창의 현재 갯수가 0개라면 재장전해준다
	if (weaponMap[CURRENT_WEAPON_NAME].current == 0 &&
		weaponMap[CURRENT_WEAPON_NAME].total != 0)
		Reload();

	//스코프가 있는 무기라면 스코프를 만들어주고 없는 위젯이라면 일반 조준점을 만들어준다
	ZoomOut();
	RemoveScopeWidget();
	if (UKismetSystemLibrary::IsValidClass(CURRENT_WEAPON->weaponScope)) {
		currentScopeWidget = CreateWidget<UDWP_Scope>(ownerPlayerController, CURRENT_WEAPON->weaponScope);
		if (currentCrossHairWidget != nullptr) {
			currentCrossHairWidget->RemoveInViewport();
			currentCrossHairWidget = nullptr;
		}
	}
	else {
		currentScopeWidget = nullptr;
		if (currentCrossHairWidget == nullptr)
			currentCrossHairWidget = CreateWidget<UDW_PlayerWidget>(ownerPlayerController, crossHairWidget);
		currentCrossHairWidget->ShowInViewport(VO_CROSS_HAIR);
	}

	//스탯에서 추가데미지 항목중 무기항목의 데미지를 변경해준다
	statComp->SetAdditionAttackDamage(TEXT("Weapon"), CURRENT_WEAPON->weaponDamage);
	DFLOG(Display, TEXT("%s Weapon Change : %s"), *ownerPlayer->GetName(), *CURRENT_WEAPON_NAME);
	return true;
	}
	}
	return false;
}

void UDC_Weapon::FireModeChange()
{
	DFLOG_N(Display);

	switch (statComp->GetState())
	{
	case ECHARACTER_STATE::NORMAL:{
	//반복문을 돌면서 무기의 셋팅을 변경한다
	bool bSettingEnd = false;
	while (!bSettingEnd) {
		SetFireSelectorNextMode();
		switch (weaponMap[CURRENT_WEAPON_NAME].fireMode)
		{
		case EWEAPON_FIRE_SELECTOR::SEMI: {
			PRINTSTR(WEAPON_MODE_CHANGE, 5.f, FColor::Red, TEXT("CurrentWeaponMode : Semi"));
			bSettingEnd = true;
			DFLOG(Display, TEXT("Current Weapon Mode : Semi"));
			break;
		}
		case EWEAPON_FIRE_SELECTOR::BURST: {
			if (CURRENT_WEAPON->fireSelectorSetting.burstMode) {
				PRINTSTR(WEAPON_MODE_CHANGE, 5.f, FColor::Red, TEXT("CurrentWeaponMode : Burst"));
				bSettingEnd = true;
				DFLOG(Display, TEXT("Current Weapon Mode : Burst"));
			}
			break;
		}
		case EWEAPON_FIRE_SELECTOR::AUTO: {
			if (CURRENT_WEAPON->fireSelectorSetting.autoMode) {
				PRINTSTR(WEAPON_MODE_CHANGE, 5.f, FColor::Red, TEXT("CurrentWeaponMode : Auto"));
				bSettingEnd = true;
				DFLOG(Display, TEXT("Current Weapon Mode : Auto"));
			}
			break;
		}
		}
	}
	onFireSelectorChange.Broadcast(weaponMap[CURRENT_WEAPON_NAME].fireMode);
	break;
	}
	}
}

void UDC_Weapon::Reload()
{
	DFLOG_N(Display);
	switch (ownerPlayer->GetStatComponent()->GetState())
	{
	case ECHARACTER_STATE::NORMAL:{
		auto reloadMontage = ownerPlayer->GetWeaponComponent()->GetCurrentWeaponData().weaponData.reloadAnimation;
		int32 characterIndex = ownerPlayer->GetPlayerCharacterIndex();
		DFCHECK(characterIndex < reloadMontage.Num());
		ownerPlayer->GetAnimMontageComponent()->PlayAnimMontage(reloadMontage[characterIndex]);
		break;
	}
	}
}

void UDC_Weapon::Fire(FVector _FireLocation, FVector _TracingEndLocation, bool _bIsZooming)
{
	switch (statComp->GetState())
	{
	case ECHARACTER_STATE::NORMAL:{
	FString currentWeaponName = CURRENT_WEAPON_NAME;

	//공격쿨타임이나 장전된 총알수에 따라 공격을 리턴한다
	if (weaponCoolDown >= 0 || weaponMap[currentWeaponName].current <= 0) return;

	DFCHECK(CURRENT_WEAPON != nullptr);

	ownerPlayer->GetAnimMontageComponent()->StopAnimMontage();

	//공격타입에 따라 공격법이 달라진다
	switch (CURRENT_WEAPON->weaponAttackType)
	{
	case EWEAPON_ATTACK_TYPE::TRACING: {
		//라인트레이싱으로 공격을 한다
		TracingAttack(
			_FireLocation,
			_TracingEndLocation,
			_bIsZooming);
		break;
	}
	case EWEAPON_ATTACK_TYPE::PROJECTILE: {
		//프로젝타일을 날린다
		FVector muzzleLocation = weaponSkeletalComponent->GetSocketLocation(TEXT("Muzzle"));
		
		FVector cameraLookLocation = ownerPlayer->GetPlayerCameraComponent()->GetForwardVector() * 3000.f;
		cameraLookLocation += ownerPlayer->GetPlayerCameraComponent()->GetComponentLocation();

		FRotator projectileRotation = UKismetMathLibrary::FindLookAtRotation(muzzleLocation, cameraLookLocation);

		auto spawnedProjectile = characterProjectileManager->TakeProjectile(
			muzzleLocation,
			projectileRotation);

		spawnedProjectile->SetActorLocation(spawnedProjectile->GetActorLocation() +
			UKismetMathLibrary::GetForwardVector(
				spawnedProjectile->GetActorRotation()) *
			spawnedProjectile->GetCollision()->GetScaledSphereRadius());

		FProjectileInitializer initializer = CURRENT_WEAPON->projectileSetting;
		initializer.ignoreTags.Add(TEXT("Player"));
		initializer.ownerCharacter = ownerPlayer;
		initializer.damage = statComp->GetDamage(ESTAT_DATA_TYPE::TOTAL);
		
		spawnedProjectile->InitializeProjectile(initializer);
		IDI_ObjectPoolingActor::Execute_ActiveSetting(spawnedProjectile, true);
		weaponSkeletalComponent->PlayAnimation(CURRENT_WEAPON->fireAnimation, false);
		break;
	}
	case EWEAPON_ATTACK_TYPE::TRACINGMULTI: {
		//라인트레이싱을 여러번 한다
		for (int i = 0; i < 10; ++i) {
			TracingAttack(
				_FireLocation,
				_TracingEndLocation,
				false);
		}
		break;
	}
	}
	//탄창 계산, 공격쿨타임 적용, 소리적용
	weaponCoolDown = CURRENT_WEAPON->attackInterval;
	CalculateMagazine(currentWeaponName);
	UAISense_Hearing::ReportNoiseEvent(GetWorld(),
		GetOwnerCharacter()->GetActorLocation(),
		CURRENT_WEAPON->loudness,
		GetOwnerCharacter(),
		CURRENT_WEAPON->soundRange);

	//만약 현재 공격모드가 버스트라면 공격쿨타임만큼 딜레이를 줘서 여러번 쏘게한다
	if (weaponMap[CURRENT_WEAPON_NAME].fireMode == EWEAPON_FIRE_SELECTOR::BURST) {
		weaponCoolDown *= 4;
		lastStartLocation = _FireLocation;
		lastEndLocation = _TracingEndLocation;
		bLastZooming = _bIsZooming;
		FTimerHandle waitHandle1{};
		FTimerHandle waitHandle2{};
		float waitTime = CURRENT_WEAPON->attackInterval;
		FTimerDelegate fire{};
		fire.BindLambda([&]() {
			if (weaponMap[CURRENT_WEAPON_NAME].current <= 0)
				return;
			TracingAttack(
				lastStartLocation,
				lastEndLocation,
				bLastZooming);
			CalculateMagazine(CURRENT_WEAPON_NAME);
			});
		GetWorld()->GetTimerManager().SetTimer(waitHandle1, fire, waitTime, false);
		GetWorld()->GetTimerManager().SetTimer(waitHandle2, fire, waitTime * 2, false);
	}
	break;
	}
	}
}

bool UDC_Weapon::DropWeapon(const FString& _WeaponName)
{
	DFLOG_N(Display);
	DFCHECK(weaponMap.Contains(_WeaponName), false);

	//현재무기가 기본무기(인덱스 0번)이라면 버릴수 없다
	if (_WeaponName == weaponDataArray[0]->weaponName) return false;

	//무기를 버리는 로직을 람다로 등록해놓는다
	auto DropFunc = [this](const FString& _WeaponName)->void {
		weaponMap[_WeaponName].ownedState = EWEAPON_OWNED_STATE::NONE_OWNED;
		weaponMap[_WeaponName].current = 0;
		weaponMap[_WeaponName].total = 0;
		onWeaponChange.Broadcast(_WeaponName, EWEAPON_OWNED_STATE::NONE_OWNED);
		onMagazineChange.Broadcast(weaponMap[_WeaponName]);
		};

	//현재 무기 상태에따라 버리는 로직을 실행한다
	switch (weaponMap[_WeaponName].ownedState) {
	case EWEAPON_OWNED_STATE::OWNED: {
		DropFunc(_WeaponName);
		return true;
	}
	case EWEAPON_OWNED_STATE::EQUIP: {
		WeaponIndexUp();
		DropFunc(_WeaponName);
		return true;
	}
	case EWEAPON_OWNED_STATE::NONE_OWNED: {
		return false;
	}
	}
	return false;
}

bool UDC_Weapon::AddWeapon(const FString& _WeaponName)
{
	DFLOG_N(Display);
	DFCHECK(weaponMap.Contains(_WeaponName), false);
	//무기의 현재 보유상태에 따라 획득로직을 실행한다
	switch (weaponMap[_WeaponName].ownedState) {
	case EWEAPON_OWNED_STATE::OWNED:
	case EWEAPON_OWNED_STATE::EQUIP:
		return false;
	case EWEAPON_OWNED_STATE::NONE_OWNED: {
		weaponMap[_WeaponName].ownedState = EWEAPON_OWNED_STATE::OWNED;
		onWeaponChange.Broadcast(_WeaponName, EWEAPON_OWNED_STATE::OWNED);
		return true;
	}
	}
	return false;
}

bool UDC_Weapon::AddMagazine(const FString& _WeaponName, int32 _BulletCount)
{
	DFLOG_N(Display);
	DFCHECK(weaponMap.Contains(_WeaponName), false);

	int32 total = weaponMap[_WeaponName].total;
	int32 current = weaponMap[_WeaponName].current;

	DFCHECK(total >= 0, false);

	if (total + current < -_BulletCount) return false;

	weaponMap[_WeaponName].total += _BulletCount;

	if (weaponMap[_WeaponName].total < 0) {
		weaponMap[_WeaponName].current -= weaponMap[_WeaponName].total;
		weaponMap[_WeaponName].total = 0;
	}

	onMagazineChange.Broadcast(weaponMap[_WeaponName]);
	return true;
}

void UDC_Weapon::SetOwnedState(const FString& _WeaponName, const FWeaponOwnedData& _WeaopnOwnedState)
{
	DFCHECK(weaponMap.Contains(_WeaponName));
	weaponMap[_WeaponName].current = _WeaopnOwnedState.current;
	weaponMap[_WeaponName].total = _WeaopnOwnedState.total;
	weaponMap[_WeaponName].fireMode = _WeaopnOwnedState.fireMode;
	weaponMap[_WeaponName].ownedState = _WeaopnOwnedState.ownedState;
	onWeaponChange.Broadcast(_WeaponName, weaponMap[_WeaponName].ownedState);
	onMagazineChange.Broadcast(weaponMap[_WeaponName]);
	if (_WeaponName == CURRENT_WEAPON_NAME)
		onFireSelectorChange.Broadcast(weaponMap[_WeaponName].fireMode);
}

void UDC_Weapon::WeaponIndexUp()
{
	DFLOG_N(Display);
	switch (statComp->GetState())
	{
	case ECHARACTER_STATE::NORMAL:{
	//무기의 인덱스에 따라 다음 순서의 보유중인 무기를 장착한다
	int32 weaponIndex = currentWeaponIndex;
	do {
		weaponIndex = CheckIndexRange(++weaponIndex);
		if (weaponMap[weaponDataArray[weaponIndex]->weaponName].ownedState == EWEAPON_OWNED_STATE::EQUIP) return;
	} while (!(weaponMap[weaponDataArray[weaponIndex]->weaponName].ownedState == EWEAPON_OWNED_STATE::OWNED));

	SetWeapon(weaponDataArray[weaponIndex]->weaponName);
	break;
	}
	}
}

void UDC_Weapon::WeaponIndexDown()
{
	DFLOG_N(Display);
	switch (statComp->GetState())
	{
	case ECHARACTER_STATE::NORMAL:{
	//무기의 인덱스에 따라 다음 순서의 보유중인 무기를 장착한다
	int32 weaponIndex = currentWeaponIndex;
	do {
		weaponIndex = CheckIndexRange(--weaponIndex);
		if (weaponMap[weaponDataArray[weaponIndex]->weaponName].ownedState == EWEAPON_OWNED_STATE::EQUIP) return;
	} while (!(weaponMap[weaponDataArray[weaponIndex]->weaponName].ownedState == EWEAPON_OWNED_STATE::OWNED));

	SetWeapon(weaponDataArray[weaponIndex]->weaponName);
	break;
	}
	}
}

TArray<FWeaponOwnedData> UDC_Weapon::GetAllWeaponData() const
{
	TArray<FWeaponOwnedData> result{};
	weaponMap.GenerateValueArray(result);
	return result;
}

void UDC_Weapon::NewStartSetting()
{
	//기본 무기를 설정한다
	FString	basicWeapon = CURRENT_WEAPON_NAME;
	weaponMap[basicWeapon].ownedState = EWEAPON_OWNED_STATE::OWNED;
	weaponMap[basicWeapon].current = CURRENT_WEAPON->maganize;
	weaponMap[basicWeapon].total = -1;

	SetWeapon(basicWeapon);
}

void UDC_Weapon::SetPlayerController(ADAPC_InGame* _NewPlayerController)
{
	ownerPlayerController = _NewPlayerController;
}

void UDC_Weapon::InitializeComponent()
{
	Super::InitializeComponent();

	//오너 캐릭터에 스켈레탈메시 컴포넌트를 생성하고 셋팅해준다
	weaponSkeletalComponent = NewObject<USkeletalMeshComponent>(this);
	weaponSkeletalComponent->SetupAttachment(GetOwnerCharacter()->GetMesh(), attachSocketName);
	weaponSkeletalComponent->SetHiddenInSceneCapture(true);
	weaponSkeletalComponent->RegisterComponent();
	GetOwnerCharacter()->AddInstanceComponent(weaponSkeletalComponent);
	weaponSkeletalComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//무기 정보의 데이터테이블을 배열형태로 가져온다
	//배열을 순회하면서 Map에 등록한다
	weaponDataTable->GetAllRows<FDFWeaponData>(TEXT("GetAllRows"), weaponDataArray);

	for (auto result : weaponDataArray) {
		FWeaponOwnedData data{};
		data.weaponData = *result;
		weaponMap.Add(TTuple<FString, FWeaponOwnedData>(result->weaponName, data));
	}

	ownerPlayer = Cast<ADA_Player>(GetOwnerCharacter());
	DFCHECK(ownerPlayer != nullptr);

	statComp = ownerPlayer->GetStatComponent();
	characterProjectileManager = ownerPlayer->GetProjectileManagerComponent();
	ownerPlayer->GetAnimMontageComponent()->onAnimInstanceChange.AddDynamic(this, &UDC_Weapon::ChangeAnimInstance);
}

void UDC_Weapon::BeginPlay()
{
	Super::BeginPlay();

	if (ownerPlayerController == nullptr)
		ownerPlayerController = Cast<ADAPC_InGame>(ownerPlayer->GetController());
	DFCHECK(ownerPlayer != nullptr);
}

void UDC_Weapon::TickComponent(float _DeltaTime, ELevelTick _TickType, FActorComponentTickFunction* _ThisTickFunction)
{
	Super::TickComponent(_DeltaTime, _TickType, _ThisTickFunction);

	//무기 쿨타임을 계산한다
	if (weaponCoolDown >= 0)
		weaponCoolDown -= _DeltaTime;
}

void UDC_Weapon::ChangeAnimInstance(UDO_AnimInstance* _AnimInstance)
{
	if (currentAnimInstance != nullptr)
		currentAnimInstance->onReload.RemoveDynamic(this, &UDC_Weapon::ReloadEnd);

	currentAnimInstance = Cast<UDOA_Player>(_AnimInstance);
	if (currentAnimInstance != nullptr)
		currentAnimInstance->onReload.AddDynamic(this, &UDC_Weapon::ReloadEnd);
}

void UDC_Weapon::StateChange(ECHARACTER_STATE _CurrentState)
{
	switch (_CurrentState)
	{
	case ECHARACTER_STATE::CASTING:
	case ECHARACTER_STATE::DEAD:
	case ECHARACTER_STATE::STUN:
	case ECHARACTER_STATE::HIT:
	case ECHARACTER_STATE::CHARGING:
		ZoomOut();
		break;
	}
}

void UDC_Weapon::ReloadEnd()
{
	DFLOG_N(Display);

	switch (statComp->GetState())
	{
	case ECHARACTER_STATE::NORMAL:{
		int32 currentWeaponMaganize = CURRENT_WEAPON->maganize;

		//탄창의 총 갯수가 0개라면 리턴한다
		if (weaponMap[CURRENT_WEAPON_NAME].total == 0) return;

		//총알의 총 갯수가 양수라면 현재 총알의 갯수를 전체 총알갯수에 더하고
		//현재 총알수를 0으로 한다 ex) 30/120 -> 0/150
		else if (weaponMap[CURRENT_WEAPON_NAME].total > 0) {
			weaponMap[CURRENT_WEAPON_NAME].total += weaponMap[CURRENT_WEAPON_NAME].current;
			weaponMap[CURRENT_WEAPON_NAME].current = 0;
		}

		//총 갯수가 음수(무한)라면 최대갯수로 채워준다
		else weaponMap[CURRENT_WEAPON_NAME].current = currentWeaponMaganize;
		//현재 가진 총알이 최대장전량 이상이라면
		//총갯수에서 최대장전량만큼 빼고 현재총알에 최대장전량만큼 더한다
		if (weaponMap[CURRENT_WEAPON_NAME].total >= currentWeaponMaganize) {
			weaponMap[CURRENT_WEAPON_NAME].total -= currentWeaponMaganize;
			weaponMap[CURRENT_WEAPON_NAME].current += currentWeaponMaganize;
		}

		//최대장전량 이하라면 현재 가진 총알만큼만 가져오고 가진 총알을 0으로 한다
		else if (weaponMap[CURRENT_WEAPON_NAME].total >= 0 && weaponMap[CURRENT_WEAPON_NAME].total < currentWeaponMaganize) {
			weaponMap[CURRENT_WEAPON_NAME].current = weaponMap[CURRENT_WEAPON_NAME].total;
			weaponMap[CURRENT_WEAPON_NAME].total = 0;
		}

		onMagazineChange.Broadcast(weaponMap[CURRENT_WEAPON_NAME]);
		break;
	}
	}
}

void UDC_Weapon::SetFireSelectorNextMode()
{
	switch (weaponMap[CURRENT_WEAPON_NAME].fireMode)
	{
	case EWEAPON_FIRE_SELECTOR::SEMI: {
		weaponMap[CURRENT_WEAPON_NAME].fireMode = EWEAPON_FIRE_SELECTOR::BURST;
		break;
	}
	case EWEAPON_FIRE_SELECTOR::BURST: {
		weaponMap[CURRENT_WEAPON_NAME].fireMode = EWEAPON_FIRE_SELECTOR::AUTO;
		break;
	}
	case EWEAPON_FIRE_SELECTOR::AUTO: {
		weaponMap[CURRENT_WEAPON_NAME].fireMode = EWEAPON_FIRE_SELECTOR::SEMI;
		break;
	}
	}
}

void UDC_Weapon::TracingAttack(FVector _StartLocation, FVector _EndLocation, bool _bIsZooming)
{
	//발사 오차를 계산한다
	float widthError{};
	float heightError{};
	_bIsZooming ? widthError = FMath::FRandRange(-100.f, 100.f) : widthError = FMath::FRandRange(-350.f, 350.f);
	_bIsZooming ? heightError = FMath::FRandRange(-100.f, 100.f) : heightError = FMath::FRandRange(-350.f, 350.f);

	//라인트레이싱으로 공격한다
	FHitResult hitResult{};
	FVector endLocation = _EndLocation + FVector(widthError, widthError, heightError);
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes{};
	TEnumAsByte<EObjectTypeQuery> worldStatic = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic);
	TEnumAsByte<EObjectTypeQuery> pawn = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn);
	ObjectTypes.Add(worldStatic);
	ObjectTypes.Add(pawn);
	TArray<AActor*> ignoreActors{};
	ignoreActors.Add(ownerPlayer);

	if (UKismetSystemLibrary::LineTraceSingleForObjects(
		GetWorld(),
		_StartLocation,
		endLocation,
		ObjectTypes,
		false,
		ignoreActors,
		EDrawDebugTrace::ForDuration,
		hitResult,
		false)) {

		PRINTSTR(PLAYER_ATTACK_HIT, 1.f, FColor::Black, TEXT("Hit Actor : %s"), *hitResult.GetActor()->GetName());
		auto hitActor = Cast<ADA_Character>(hitResult.GetActor());
		if (hitActor != nullptr)
			hitActor->ApplyDamage(ownerPlayer->GetStatComponent()->GetDamage(ESTAT_DATA_TYPE::TOTAL), ownerPlayer, ownerPlayerController, hitResult.ImpactPoint);
		if (sparkEffect != nullptr) {
			FRotator rotation = UKismetMathLibrary::FindLookAtRotation(GetOwnerCharacter()->GetActorLocation(), hitResult.ImpactPoint);
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), sparkEffect, hitResult.ImpactPoint, rotation, true);
		}
	}

	//반동 계산
	float minRecoil = -CURRENT_WEAPON->minRecoil;
	float maxRecoil = -CURRENT_WEAPON->maxRecoil;

	float pitchRecoil = FMath::FRandRange(minRecoil, maxRecoil);
	float yawRecoil = FMath::FRandRange(maxRecoil * 0.2, maxRecoil * -0.2f);

	ownerPlayer->AddControllerPitchInput(pitchRecoil);
	ownerPlayer->AddControllerYawInput(yawRecoil);

	//발사 애니메이션을 재생한다
	DFCHECK(CURRENT_WEAPON->fireAnimation != nullptr);
	weaponSkeletalComponent->PlayAnimation(CURRENT_WEAPON->fireAnimation, false);
}

int32 UDC_Weapon::CheckIndexRange(int32 _Index)
{
	if (_Index > weaponDataArray.Num() - 1)
		return 0;
	else if (_Index < 0)
		return weaponDataArray.Num() - 1;
	else
		return _Index;
}

void UDC_Weapon::CalculateMagazine(const FString& _CurrentWeaponName)
{
	//현재 남은 탄창을 계산하고 현재 총알수가 0이라면 재장전한다
	--weaponMap[_CurrentWeaponName].current;
	if (weaponMap[_CurrentWeaponName].current == 0 && weaponMap[_CurrentWeaponName].total != 0)
		Reload();
	onMagazineChange.Broadcast(weaponMap[_CurrentWeaponName]);
}

void UDC_Weapon::OnAssetLoadCompleted()
{
	DFLOG_N(Display);
	DFCHECK(assetStreamingHandle.IsValid());
	auto assetLoaded = Cast<USkeletalMesh>(assetStreamingHandle->GetLoadedAsset());
	DFCHECK(assetLoaded != nullptr);
	assetStreamingHandle.Reset();
	weaponSkeletalComponent->SetSkeletalMesh(assetLoaded);
}

void UDC_Weapon::RemoveScopeWidget()
{
	DFLOG_N(Display);
	DFCHECK(currentScopeWidget != nullptr);
	currentScopeWidget->RemoveInViewport();
	
	ownerPlayer->GetPlayerCameraComponent()->SetFieldOfView(90.f);
	ownerPlayer->GetMesh()->SetOwnerNoSee(false);
	weaponSkeletalComponent->SetOwnerNoSee(false);
	ownerPlayerController->ShowMainWidget();
}
