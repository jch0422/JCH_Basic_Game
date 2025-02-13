#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "CH_GameState.generated.h"


UCLASS()
class JCH_BASIC_GAME_API ACH_GameState : public AGameState
{
    GENERATED_BODY()
public:
    ACH_GameState();

    virtual void BeginPlay() override;
    //����
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Score")
    int32 Score;

    //���� ����
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Coin")
    int32 SpawnedCoinCount;
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Coin")
    int32 CollectedCoinCount;

    //��������
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Level")
    float LevelDuration;
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Level")
    int32 CurrentLevelIndex;
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Level")
    int32 MaxLevel;
    //���̺� ����



    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
    TArray<FName> LevelMapNames;

    FTimerHandle LevelTimerHandle;
    FTimerHandle HUDUpdateTimerHandle;


    UFUNCTION(BlueprintPure, Category = "Score")
    int32 GetScore() const;
    UFUNCTION(BlueprintCallable, Category = "Score")
    void AddScore(int32 Amount);
    UFUNCTION(BlueprintCallable, Category = "Level")
    void OnGameOver();

    void StartLevel();
    void OnLevelTimeUp();
    void OnCoinCollected();
    void EndLevel();
    void UpdateHUD();
    void DestroyAllParticleSystems();
};
