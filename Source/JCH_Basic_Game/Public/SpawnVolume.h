#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemSpawnRow.h"
#include "SpawnVolume.generated.h"

class UBoxComponent;

UCLASS()
class JCH_BASIC_GAME_API ASpawnVolume : public AActor
{
    GENERATED_BODY()

public:
    ASpawnVolume();

    UFUNCTION(BlueprintCallable, Category = "Spawning")
    AActor* SpawnRandomItem();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
    USceneComponent* Scene;
    // ���� ������ ����� �ڽ� ������Ʈ
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
    UBoxComponent* SpawningBox;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
    UDataTable* ItemDataTable;

    // ���� ���� ���ο��� ������ ��ǥ�� ������ �Լ�
    UFUNCTION(BlueprintCallable, Category = "Spawning")
    FVector GetRandomPointInVolume() const;
    // Ư�� ������ Ŭ������ �����ϴ� �Լ�
    UFUNCTION(BlueprintCallable, Category = "Spawning")
    AActor* SpawnItem(TSubclassOf<AActor> ItemClass);

    FItemSpawnRow* GetRandomItem() const;

};
