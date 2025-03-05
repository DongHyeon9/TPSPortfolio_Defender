#include "Server/DA_GameMode.h"
#include "Actor/Level/DA_LevelScript.h"

void ADA_GameMode::BeginPlay()
{
	Super::BeginPlay();

	GEngine->ForceGarbageCollection();
	levelScript = Cast<ADA_LevelScript>(GetWorld()->GetLevelScriptActor());
	DFCHECK(levelScript != nullptr);
}
