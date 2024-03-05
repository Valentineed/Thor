// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WaveManager.generated.h"

class ASpawnerEnemy;

USTRUCT(BlueprintType)
struct FWave
{
	GENERATED_BODY()
public:
  UPROPERTY(EditInstanceOnly)
	TArray<ASpawnerEnemy*> Spawners;
};

UCLASS()
class PANDAPROJECT_API AWaveManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWaveManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Waves", meta = (clampMin = "0.0"))
		float TimeBetweenEachWave = 3;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Waves")
		TArray<FWave> Waves;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int CurrentWaveCount = 0;//Todo use this variable for the UI
private:
	void WaveIsRunning(TArray<ASpawnerEnemy*> CurrentWave);
	
	void StartWave();
	
	int WaveCount = 0;
	int SpawnerInWaveCount = 0;

	FTimerHandle WaveTimer;


};
