#include "Actor/Player/DA_PlayerState.h"

ADA_PlayerState::ADA_PlayerState()
{

}

void ADA_PlayerState::AddScore(int32 _Score)
{
	DFLOG_N(Display);
	currentScore += _Score;
	if (highScore < currentScore)
		highScore = currentScore;
	onScoreChange.Broadcast(currentScore, highScore);
}

void ADA_PlayerState::SetHighScore(int32 _HighScore)
{
	highScore = _HighScore;
	onScoreChange.Broadcast(currentScore, highScore);
}

void ADA_PlayerState::SetCurrentScore(int32 _CurrentScore)
{
	currentScore = _CurrentScore;
	onScoreChange.Broadcast(currentScore, highScore);
}

void ADA_PlayerState::SetPlayerName(const FString& _Name)
{
	Super::SetPlayerName(_Name);

	onNameChange.Broadcast(_Name);
}
