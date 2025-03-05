#include "Actor/DO_AnimInstance.h"
#include "Actor/DA_Character.h"
#include "Component/Share/DC_Stat.h"
#include "Component/Share/DC_AnimMontage.h"

#include "GameFramework/CharacterMovementComponent.h"

UDO_AnimInstance::UDO_AnimInstance()
{
	//UPROPERTY���� �ʱ�ȭ
	ownerCharacter = nullptr;
	currentCharacterSpeed = 0.f;
	characterVelocity = FVector::ZeroVector;
	bIsInAir = false;
}

void UDO_AnimInstance::NativeUpdateAnimation(float _DeltaSeconds)
{
	Super::NativeUpdateAnimation(_DeltaSeconds);

	//ĳ���͸� �����´�
	ownerCharacter = Cast<ADA_Character>(TryGetPawnOwner());

	if (ownerCharacter != nullptr) {
		//ĳ���ͷκ��� ������ ������ �������ش�
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
