#include "Skills/Passive/DO_PassiveBase.h"
#include "Actor/DA_Character.h"

UDO_PassiveBase::UDO_PassiveBase()
{
}

void UDO_PassiveBase::RegistPassive_Implementation(ADA_Character* _OwnerCharacter)
{
	DFLOG_N(Display);
	//���� ĳ���Ͱ� �̹� �ִٸ� �����ϰ� �ٽ� ����Ѵ�
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
