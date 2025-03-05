#include "Skills/Active/DO_ActiveBase.h"
#include "Actor/DA_Character.h"
#include "Server/DA_GameInstance.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

UDO_ActiveBase::UDO_ActiveBase()
{
	castState = ECHARACTER_STATE::CASTING;
}

void UDO_ActiveBase::SkillPreparation_Implementation()
{
	DFLOG_N(Display);
	if (bIsLineTraceSkill)
		bIsPreparation = true;
}

void UDO_ActiveBase::UseSkill_Implementation()
{
	DFLOG_N(Display);
	bIsPreparation = false;
}

void UDO_ActiveBase::CancelSkill_Implementation()
{
	DFLOG_N(Display);
	ApplyCoolDown();
	bIsPreparation = false;
}

void UDO_ActiveBase::EndSkill_Implementation()
{
	DFLOG_N(Display);
}

void UDO_ActiveBase::SkillNotify_Implementation()
{
	DFLOG_N(Display);
}

void UDO_ActiveBase::SkillTick_Implementation(float _DeltaTime, const FHitResult& _HitResult)
{
}

void UDO_ActiveBase::ApplyCoolDown(float _CoolDown)
{
	//현재 쿨타임을 설정하고
	//스킬쿨타임 델리게이트를 호출하고
	//스킬쿨타임이 끝날때까지 현재 쿨타임을 줄여가면서 델리게이트를 호출해준다
	if (_CoolDown >= 0.f)
		currentCoolDown = _CoolDown;
	else
		currentCoolDown = totalCoolDown;

	onCoolDown.Broadcast(totalCoolDown, currentCoolDown);

	cooldownCheckInterval = currentCoolDown * 0.01f;

	GetWorld()->GetTimerManager().SetTimer(coolDownHandle, FTimerDelegate::CreateLambda([&]()
		{
			currentCoolDown -= cooldownCheckInterval;
			onCoolDown.Broadcast(totalCoolDown, currentCoolDown);
			if (currentCoolDown <= 0.f && coolDownHandle.IsValid()) {
				GetWorld()->GetTimerManager().ClearTimer(coolDownHandle);
				coolDownHandle.Invalidate();
			}
		}), cooldownCheckInterval, true);
}

void UDO_ActiveBase::SetOwnerCharacter(ADA_Character* _OwnerCharacter)
{
	ownerCharacter = _OwnerCharacter;

	auto gameInstance = Cast<UDA_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	DFCHECK(gameInstance != nullptr);
	gameInstance->RegistTimerHandle(&coolDownHandle);
}

void UDO_ActiveBase::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
	//트레이싱을 하는 스킬일 경우 준비단계에 틱에서
	//라인트레이싱을 호출하고 결과를 반환하는 SkillTick 함수를 호출한다
	if (bIsLineTraceSkill && bIsPreparation) {
		FHitResult outResultTrace{};
		UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(),
			startLoctionTrace,
			endLoctionTrace,
			objectTypeTrace,
			false,
			ignoreActorsTrace,
			EDrawDebugTrace::ForOneFrame,
			outResultTrace,
			true);
		SkillTick(_DeltaTime, outResultTrace);
	}
}

UAnimMontage* UDO_ActiveBase::GetCastAction(int32 _Index) const
{
	if (castAction.Num() - 1 < _Index) return nullptr;

	return castAction[_Index];
}
