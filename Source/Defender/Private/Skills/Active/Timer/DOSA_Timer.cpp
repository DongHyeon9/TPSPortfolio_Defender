#include "Skills/Active/Timer/DOSA_Timer.h"
#include "Actor/DA_Character.h"
#include "Actor/DO_AnimInstance.h"
#include "Component/Share/DC_Stat.h"
#include "Server/DA_GameInstance.h"

#include "Kismet/GameplayStatics.h"

UDOSA_Timer::UDOSA_Timer()
{
	
}

void UDOSA_Timer::UseSkill_Implementation()
{
	Super::UseSkill_Implementation();

	//사용후 지속시간이 끝나면 종료함수를 호출한다
	if (!skillTimerHandle.IsValid()) 
		GetWorld()->GetTimerManager().SetTimer(skillTimerHandle, FTimerDelegate::CreateLambda([&]()
			{
				SkillTimeOut();
				if (skillTimerHandle.IsValid()) {
					GetWorld()->GetTimerManager().ClearTimer(skillTimerHandle);
					skillTimerHandle.Invalidate();
				}
			}), skillDuration, false);
}

void UDOSA_Timer::SkillTimeOut_Implementation()
{
	DFLOG_N(Display);

	//종료후 쿨타임 적용이면 종료후에 적용한다
	if (afterCoolDownApply)
		Super::ApplyCoolDown();

	if (ownerStatComp->GetState() == GetCastState())
		ownerStatComp->SetState(ECHARACTER_STATE::NORMAL);
}

void UDOSA_Timer::SetOwnerCharacter(ADA_Character* _OwnerCharacter)
{
	Super::SetOwnerCharacter(_OwnerCharacter);

	ownerStatComp = _OwnerCharacter->GetStatComponent();

	auto gameInstance = Cast<UDA_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	DFCHECK(gameInstance != nullptr);
	gameInstance->RegistTimerHandle(&skillTimerHandle);
}

void UDOSA_Timer::CancelSkill_Implementation()
{
	UDO_ActiveBase::CancelSkill_Implementation();

	if (afterCoolDownApply)
		Super::ApplyCoolDown();
}

void UDOSA_Timer::ApplyCoolDown(float _CoolDown)
{
	Super::ApplyCoolDown(_CoolDown);

	if (afterCoolDownApply) 
		if (coolDownHandle.IsValid()) {
			GetWorld()->GetTimerManager().ClearTimer(coolDownHandle);
			coolDownHandle.Invalidate();
		}
}