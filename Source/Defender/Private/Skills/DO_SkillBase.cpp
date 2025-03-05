#include "Skills/DO_SkillBase.h"

#include "GameFramework/WorldSettings.h"

UDO_SkillBase::UDO_SkillBase()
{
	bUseTick = false;
}

UWorld* UDO_SkillBase::GetWorld() const
{
	if (!HasAnyFlags(RF_ClassDefaultObject) && ensureMsgf(GetOuter(), TEXT("SkillBase : %s has a null OuterPrivate in UDO_ActiveBase::GetWorld()"), *GetFullName())
		&& !GetOuter()->HasAnyFlags(RF_BeginDestroyed) && !GetOuter()->IsUnreachable())
	{
		if (ULevel* Level = GetLevel())
		{
			return Level->OwningWorld;
		}
	}
	return nullptr;
}

ULevel* UDO_SkillBase::GetLevel() const
{
	return GetTypedOuter<ULevel>();
}

TStatId UDO_SkillBase::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UDO_SkillBase, STATGROUP_Tickables);
}

void UDO_SkillBase::Tick(float _DeltaTime)
{
	if (bUseTick&&
		GetWorldSettings() != nullptr)
		ReceiveTick(_DeltaTime);
}

AWorldSettings* UDO_SkillBase::GetWorldSettings() const
{
	UWorld* World = GetWorld();
	return (World ? World->GetWorldSettings() : nullptr);
}