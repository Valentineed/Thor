// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnerEnemy.generated.h"

/**
 * 
 */
UCLASS()
class PANDAPROJECT_API ASpawnerEnemy : public AActor
{
	GENERATED_BODY()
public:
	void StartSpawner();
	FTimerHandle SpawnTimer;
	bool bSpawnAllActors = false;
	
protected:
	virtual void BeginPlay() override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;

	UPROPERTY(EditAnywhere, Category = SpawnerEnemy)
		TSubclassOf<AActor> SpawnedActorType = nullptr;


	/*Radius to spawn around the Spawner*/
	UPROPERTY(EditAnywhere, Category = SpawnerEnemy, meta = (clampMin = "0.0"))
		float Radius = 250.f;

	UPROPERTY(EditAnywhere, Category = SpawnerEnemy, meta = (clampMin = "0.1"))
		float Frequecy = 3;

	UPROPERTY(EditAnywhere, Category = SpawnerEnemy, meta = (clampMin = "1"))
		unsigned int NumbrEnemy = 5;

	UPROPERTY(EditAnywhere, Category = SpawnerEnemy, meta = (clampMin = "1"))
		unsigned int TotalEnemiesInSpawner = 15;
 private:
	 void SpawnEnemy();
	 bool IsOverlapped = false;
	 unsigned int EnemyCount = 0;
};
