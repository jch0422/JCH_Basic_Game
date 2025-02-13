#include "CoinItem.h"
#include "Engine/World.h"
#include "CH_GameState.h"
ACoinItem::ACoinItem()
{
	PointValue = 0;
	ItemType = "DefaultCoin";
}

void ACoinItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);

	if (Activator && Activator->ActorHasTag("Player"))
	{
		if (UWorld* World = GetWorld())
		{
			if (ACH_GameState* GameState = World->GetGameState<ACH_GameState>())
			{
				GameState->AddScore(PointValue);
				GameState->OnCoinCollected();
			}
		}
		FString CoinMessage = FString::Printf(TEXT("Damage: %d"), PointValue);
		GEngine->AddOnScreenDebugMessage(0, 1.0f, FColor::Yellow, CoinMessage);

		DestroyItem();
	}
}
