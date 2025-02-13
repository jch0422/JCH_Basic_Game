#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CH_GameInstance.generated.h"

UCLASS()
class JCH_BASIC_GAME_API UCH_GameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UCH_GameInstance();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameData")
	int32 TotalScore;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameData")
	int32 CurrentLevelIndex;

	UFUNCTION(BlueprintCallable, Category = "GameData")
	void AddToScore(int32 Amount);
};
