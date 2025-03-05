#include "Widget/InGame/Monster/DW_MonsterHP.h"
#include "Actor/Monster/DA_Monster.h"
#include "Component/Share/DC_Stat.h"

#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UDW_MonsterHP::InitializeWidget()
{
	Super::InitializeWidget();

	auto monster = Cast<ADA_Monster>(GetOwnerCharacter());
	DFCHECK(monster != nullptr);

	//몬스터의 정보를 가져와 이름과 레벨을 정보창에 표시한다
	//프로그레스바를 설정한다

	auto statComp = monster->GetStatComponent();
	auto currentStat = statComp->GetCharacterCurrentStat();
	auto currentDataTable = statComp->GetCurrentDataTable();

	statComp->onChangeHP.RemoveDynamic(this, &UDW_MonsterHP::OnChangeHP);
	statComp->onChangeHP.AddDynamic(this, &UDW_MonsterHP::OnChangeHP);
	
	FString monsterStatusString = TEXT("LV.")
		+ FString::FromInt(currentStat.characterLevel)
		+ TEXT(" ")
		+ monster->GetMonsterData()->monsterName;

	FText monsterStatusText = FText::FromString(monsterStatusString);
	txt_MonsterStatus->SetText(monsterStatusText);	

	pb_HPBar->SetPercent(currentStat.currentHP / currentDataTable.characterHP);
}

void UDW_MonsterHP::NativeConstruct()
{
	Super::NativeConstruct();

}

void UDW_MonsterHP::NativeDestruct()
{
	Super::NativeDestruct();

	auto monster = Cast<ADA_Monster>(GetOwnerCharacter());
	DFCHECK(monster != nullptr);

	monster->GetStatComponent()->onChangeHP.RemoveDynamic(this, &UDW_MonsterHP::OnChangeHP);
}

void UDW_MonsterHP::SetMonsterImage(UTexture2D* _NewTexture)
{
	img_Monster->SetBrushFromTexture(_NewTexture);
}

void UDW_MonsterHP::OnChangeHP(float _MaxHP, float _CurrentHP)
{
	pb_HPBar->SetPercent(_CurrentHP / _MaxHP);
}
