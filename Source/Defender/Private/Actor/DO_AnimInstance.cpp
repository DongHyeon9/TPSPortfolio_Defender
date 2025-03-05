#include "Actor/DO_AnimInstance.h"
#include "Actor/DA_Character.h"
#include "Component/Share/DC_Stat.h"
#include "Component/Share/DC_AnimMontage.h"

#include "GameFramework/CharacterMovementComponent.h"

UDO_AnimInstance::UDO_AnimInstance()
{
	//UPROPERTY변수 초기화
	ownerCharacter = nullptr;
	currentCharacterSpeed = 0.f;
	characterVelocity = FVector::ZeroVector;
	bIsInAir = false;
}

void UDO_AnimInstance::NativeUpdateAnimation(float _DeltaSeconds)
{
	Super::NativeUpdateAnimation(_DeltaSeconds);

	//캐릭터를 가져온다
	ownerCharacter = Cast<ADA_Character>(TryGetPawnOwner());

	if (ownerCharacter != nullptr) {
		//캐릭터로부터 각각의 변수를 셋팅해준다
		characterVelocity = ownerCharacter->GetVelocity();
		currentCharacterSpeed = characterVelocity.Size();
		bIsInAir = ownerCharacter->GetCharacterMovement()->IsFalling();
		characterState = ownerCharacter->GetStatComponent()->GetState();
	}
}

void UDO_AnimInstance::AnimNotify_CastingStart()
{
	onCastingStart.Broadcast();
}

void UDO_AnimInstance::AnimNotify_SkillLoopCheck()
{
	onSkillLoopCheck.Broadcast();
}

void UDO_AnimInstance::AnimNotify_UseSkill()
{
	onUseSkill.Broadcast();
}

void UDO_AnimInstance::AnimNotify_SkillNotify()
{
	onSkillNotify.Broadcast();
}

void UDO_AnimInstance::AnimNotify_EndSkill()
{
	onEndSkill.Broadcast();
}

void UDO_AnimInstance::AnimNotify_HitStart()
{
	onHitStart.Broadcast();
}

void UDO_AnimInstance::AnimNotify_HitEnd()
{
	onHitEnd.Broadcast();
}

void UDO_AnimInstance::AnimNotify_StunEnd()
{
	onStunEnd.Broadcast();
}

void UDO_AnimInstance::AnimNotify_ComboAttack()
{
	onComboAttack.Broadcast();
}

void UDO_AnimInstance::AnimNotify_DeadEnd()
{
	onDeadEnd.Broadcast();
}
