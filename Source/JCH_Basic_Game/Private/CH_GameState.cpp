#include "CH_GameState.h"
#include "CH_PlayerController.h"
#include "CH_GameInstance.h"
#include "CH_Character.h"
#include "Kismet/GameplayStatics.h"
#include "SpawnVolume.h"
#include "CoinItem.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "BaseItem.h"
#include "Particles/ParticleSystemComponent.h"

ACH_GameState::ACH_GameState()
{
	Score = 0;
	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;
	LevelDuration = 30.0f;
	CurrentLevelIndex = 0;
	MaxLevel = 3;
}

void ACH_GameState::BeginPlay()
{
	Super::BeginPlay();

	StartLevel();

	GetWorldTimerManager().SetTimer(
		HUDUpdateTimerHandle,
		this,
		&ACH_GameState::UpdateHUD,
		0.1f,
		true
	);
}
//점수 불러오기
int32 ACH_GameState::GetScore() const
{
	return Score;
}
//점수 추가
void ACH_GameState::AddScore(int32 Amount)
{
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UCH_GameInstance* CH_GameInstance = Cast<UCH_GameInstance>(GameInstance);
		if (CH_GameInstance)
		{
			CH_GameInstance->AddToScore(Amount);
		}
	}
}
//레벨 시작
void ACH_GameState::StartLevel()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ACH_PlayerController* CH_PlayerController = Cast<ACH_PlayerController>(PlayerController))
		{
			CH_PlayerController->ShowGameHUD();
		}
	}

	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UCH_GameInstance* CH_GameInstance = Cast<UCH_GameInstance>(GameInstance);
		if (CH_GameInstance)
		{
			CurrentLevelIndex = CH_GameInstance->CurrentLevelIndex;
		}
	}

	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;

	TArray<AActor*> FoundVolumes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundVolumes);

	const int32 ItemToSpawn = 40;
	for (int32 i = 0; i < ItemToSpawn; i++)
	{
		if (FoundVolumes.Num() > 0)
		{
			ASpawnVolume* SpawnVolume = Cast<ASpawnVolume>(FoundVolumes[0]);
			if (SpawnVolume)
			{
				AActor* SpawnedActor = SpawnVolume->SpawnRandomItem();
				if (SpawnedActor && SpawnedActor->IsA(ACoinItem::StaticClass()))
				{
					SpawnedCoinCount++;
				}
			}
		}
	}
	GetWorldTimerManager().SetTimer(
		LevelTimerHandle,
		this,
		&ACH_GameState::OnLevelTimeUp,
		LevelDuration,
		false
	);
}
//시간초과
void ACH_GameState::OnLevelTimeUp()
{
	DestroyAllParticleSystems();
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
	EndLevel();
}
//주운 코인 세기
void ACH_GameState::OnCoinCollected()
{
	CollectedCoinCount++;
	UE_LOG(LogTemp, Warning, TEXT("Coin Collected: %d / %d"), CollectedCoinCount, SpawnedCoinCount);

	if (SpawnedCoinCount > 0 && CollectedCoinCount > SpawnedCoinCount)
	{
		EndLevel();
	}
}
//레벨 종료
void ACH_GameState::EndLevel()
{
	GetWorldTimerManager().ClearTimer(LevelTimerHandle);

	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UCH_GameInstance* CH_GameInstance = Cast<UCH_GameInstance>(GameInstance);
		if (CH_GameInstance)
		{
			AddScore(Score);
			CurrentLevelIndex++;
			CH_GameInstance->CurrentLevelIndex = CurrentLevelIndex;
		}
	}

	if (CurrentLevelIndex >= MaxLevel)
	{
		OnGameOver();
		return;
	}
	
	if (LevelMapNames.IsValidIndex(CurrentLevelIndex))
	{
		UGameplayStatics::OpenLevel(GetWorld(), LevelMapNames[CurrentLevelIndex]);
	}
	else
	{
		OnGameOver();
	}
}
//게임 종료
void ACH_GameState::OnGameOver()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ACH_PlayerController* CH_PlayerController = Cast<ACH_PlayerController>(PlayerController))
		{
			CH_PlayerController->SetPause(true);
			CH_PlayerController->ShowMainMenu(true);
		}
	}
}
//HUD
void ACH_GameState::UpdateHUD()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ACH_PlayerController* CH_PlayerController = Cast<ACH_PlayerController>(PlayerController))
		{
			if (UUserWidget* HUDWidget = CH_PlayerController->GetHUDWidget())
			{
				//시간
				if (UTextBlock* TimeText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Time"))))
				{
					float RemainingTime = GetWorldTimerManager().GetTimerRemaining(LevelTimerHandle);
					TimeText->SetText(FText::FromString(FString::Printf(TEXT("Time: %.1f"), RemainingTime)));
				}
				//점수
				if (UTextBlock* ScoreText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Score"))))
				{
					if (UGameInstance* GameInstance = GetGameInstance())
					{
						UCH_GameInstance* CH_GameInstance = Cast<UCH_GameInstance>(GameInstance);
						if (CH_GameInstance)
						{
							ScoreText->SetText(FText::FromString(FString::Printf(TEXT("Score: %d"), CH_GameInstance->TotalScore)));
						}
					}
				}
				//레벨
				if (UTextBlock* LevelIndexText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Level"))))
				{
					LevelIndexText->SetText(FText::FromString(FString::Printf(TEXT("Level: %d"), CurrentLevelIndex+1)));
				}

				if (UProgressBar* HealthBar = Cast<UProgressBar>(HUDWidget->GetWidgetFromName(TEXT("HealthBar"))))
				{
					ACharacter* PlayerChar = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
					if (ACH_Character* CH_Player = Cast<ACH_Character>(PlayerChar))
					{
						float HealthPercent = CH_Player->GetHealth() / CH_Player->GetMaxHealth();
						HealthBar->SetPercent(HealthPercent);

						HealthBar->SetFillColorAndOpacity(FLinearColor::Red);
					}

				}
			}
		}
	}
}

void ACH_GameState::DestroyAllParticleSystems()
{
	TArray<UParticleSystemComponent*> ParticleComponents;
	GetComponents<UParticleSystemComponent>(ParticleComponents);

	// 현재 액터에 부착된 파티클 제거
	for (auto* Particle : ParticleComponents)
	{
		if (Particle && Particle->IsActive())
		{
			Particle->DestroyComponent();
		}
	}

	// 월드 내 모든 활성 파티클 제거
	for (TObjectIterator<UParticleSystemComponent> It; It; ++It)
	{
		if (UParticleSystemComponent* Particle = *It)
		{
			if (Particle->GetWorld() == GetWorld())
			{
				Particle->Deactivate();
				Particle->DestroyComponent();
			}
		}
	}
}