#include "Component/Share/DC_AnimMontage.h"
#include "Actor/DA_Character.h"
#include "Actor/DO_AnimInstance.h"

UDC_AnimMontage::UDC_AnimMontage()
{
}

UAnimMontage* UDC_AnimMontage::GetAnimMontage(const FString& _MontageKey)
{
	DFCHECK(animMontageMap.Contains(_MontageKey), nullptr);
	return animMontageMap[_MontageKey];
}

void UDC_AnimMontage::JumpToMontageSection(const FName& _SectionName)
{
	if (currentAnimMontage != nullptr)
		ownerAnimInstance->Montage_JumpToSection(_SectionName, currentAnimMontage);
}

void UDC_AnimMontage::SetNextSection(const FName& _PrevSection, const FName& _NextSection)
{
	if (currentAnimMontage != nullptr)
		ownerAnimInstance->Montage_SetNextSection(_PrevSection, _NextSection, currentAnimMontage);
}

bool UDC_AnimMontage::PlayAnimMontage(const FString& _MontageKey, float _PlayRate)
{
	DFLOG_N(Display);
	DFCHECK(ownerAnimInstance != nullptr, false);

	//현재 진행중이던 몽타주 재생을 멈추고 새로운 몽타주를 재생합니다
	StopAnimMontage();

	currentAnimMontage = GetAnimMontage(_MontageKey);
	DFCHECK(currentAnimMontage != nullptr, false);
	
	ownerAnimInstance->Montage_Play(currentAnimMontage, _PlayRate);
	return true;
}

bool UDC_AnimMontage::PlayAnimMontage(UAnimMontage* _MontageAnim, float _PlayRate)
{
	DFLOG_N(Display);
	DFCHECK(ownerAnimInstance != nullptr, false);
	DFCHECK(_MontageAnim != nullptr, false);

	//현재 진행중이던 몽타주 재생을 멈추고 새로운 몽타주를 재생합니다
	StopAnimMontage();

	currentAnimMontage = _MontageAnim;
	ownerAnimInstance->Montage_Play(currentAnimMontage, _PlayRate);

	return true;
}

void UDC_AnimMontage::StopAnimMontage()
{
	DFLOG_N(Display);
	DFCHECK(ownerAnimInstance != nullptr);
	DFCHECK(currentAnimMontage != nullptr);
	ownerAnimInstance->Montage_Stop(0.f);
	currentAnimMontage = nullptr;
}

bool UDC_AnimMontage::AddAnimMontage(const FString& _MontageKey, UAnimMontage* _AddMontage)
{
	DFCHECK(_AddMontage != nullptr, false);
	animMontageMap.Add(_MontageKey, _AddMontage);
	return true;
}

void UDC_AnimMontage::SetAnimMontage(TMap<FString, UAnimMontage*> _AnimMontageMap)
{
	animMontageMap = _AnimMontageMap;
}

void UDC_AnimMontage::SetOwnerAnimInstance(UDO_AnimInstance* _AnimInstance)
{
	//캐릭터의 애님인스턴스를 설정합니다
	if (ownerAnimInstance != nullptr) {
		ownerAnimInstance->OnMontageStarted.RemoveDynamic(this, &UDC_AnimMontage::StartAnimMontage);
	}
	ownerAnimInstance = _AnimInstance;
	DFCHECK(ownerAnimInstance != nullptr);
	ownerAnimInstance->OnMontageStarted.AddDynamic(this, &UDC_AnimMontage::StartAnimMontage);
	onAnimInstanceChange.Broadcast(ownerAnimInstance);
}

void UDC_AnimMontage::PauseAnimMontage()
{
	DFLOG_N(Display);
	DFCHECK(ownerAnimInstance != nullptr);
	DFCHECK(currentAnimMontage != nullptr);
	ownerAnimInstance->Montage_Pause(currentAnimMontage);
}

void UDC_AnimMontage::InitializeComponent()
{
	Super::InitializeComponent();

	SetOwnerAnimInstance(Cast<UDO_AnimInstance>(GetOwnerCharacter()->GetMesh()->GetAnimInstance()));
}

void UDC_AnimMontage::StartAnimMontage(UAnimMontage* _Montage)
{
	currentAnimMontage = _Montage;
}