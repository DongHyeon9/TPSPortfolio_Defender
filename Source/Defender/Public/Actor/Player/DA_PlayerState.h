#pragma once

#include "../Defender.h"
#include "GameFramework/PlayerState.h"
#include "DA_PlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnScoreChangeDelegate, int32, _Current, int32, _High);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerNameChangeDelegate, const FString&, _Name);

//플레이어의 상태를 관리하는 액터
UCLASS()
class DEFENDER_API ADA_PlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	//Delegate called when score changes
	UPROPERTY(BlueprintAssignable)
	FOnScoreChangeDelegate onScoreChange;

	//Delegate called when name change
	UPROPERTY(BlueprintAssignable)
	FOnPlayerNameChangeDelegate onNameChange;

protected:

private:
	//Current player's points earned
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, category = "Score", meta = (AllowPrivateAccess = true))
	int32 currentScore;

	//Highest score earned by player
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, category = "Score", meta = (AllowPrivateAccess = true))
	int32 highScore;

public:
	ADA_PlayerState();
	/*
	* Add up the scores. If you exceed the highest score, the highest score changes.
	*/
	void AddScore(int32 _Score);
	/*
	* Get current score
	* @return Return int32
	*/
	FORCEINLINE int32 GetCurrentScore()const { return currentScore; }
	/*
	* Get high score
	* @return Return int32
	*/
	FORCEINLINE int32 GetHighScore()const { return highScore; }
	/*
	* Set the highest score
	* @param[in] _HighScore Highest score to set
	*/
	void SetHighScore(int32 _HighScore);
	/*
	* Set the current score
	* @param[in] _CurrentScore current score to set
	*/
	void SetCurrentScore(int32 _CurrentScore);
	/*
	* Set the player's name
	* @param[in] _Name Name to set
	*/
	virtual void SetPlayerName(const FString& _Name)override;
protected:

private:

};
