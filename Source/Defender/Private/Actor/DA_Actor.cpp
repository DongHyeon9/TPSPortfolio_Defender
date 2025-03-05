#include "Actor/DA_Actor.h"

ADA_Actor::ADA_Actor()
{
	PrimaryActorTick.bCanEverTick = false;

}

void ADA_Actor::BeginPlay()
{
	Super::BeginPlay();
	DFLOG_N(Display);
	
}