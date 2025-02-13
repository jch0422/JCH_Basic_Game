#include "CH_GameInstance.h"

UCH_GameInstance::UCH_GameInstance()
{
	TotalScore = 0;
	CurrentLevelIndex = 0;
}

void UCH_GameInstance::AddToScore(int32 Amount)
{
	TotalScore += Amount;
	UE_LOG(LogTemp, Warning, TEXT("Total Score Updated: %d"), TotalScore);
}