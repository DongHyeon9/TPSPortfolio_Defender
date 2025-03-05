#include "Skills/Passive/DO_PassiveBase.h"
#include "Actor/DA_Character.h"

UDO_PassiveBase::UDO_PassiveBase()
{
}

void UDO_PassiveBase::RegistPassive_Implementation(ADA_Character* _OwnerCharacter)
{
	DFLOG_N(Display);
	//소유 캐릭터가 이미 있다면 삭제하고 다시 등록한다
	if (ownerCharacter != nullptr) {
		RemovePassive();
		ownerCharacter = nullptr;
	}
	ownerCharacter = _OwnerCharacter;
}

void UDO_PassiveBase::RemovePassive_Implementation()
{
	DFLOG_N(Display);
}
