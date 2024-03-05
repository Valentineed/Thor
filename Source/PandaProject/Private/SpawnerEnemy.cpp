// Fill out your copyright notice in the Description page of Project Settings.

#include "PandaProject/Public/SpawnerEnemy.h"
#include "PandaProject/Public/EnemyCharacter.h"
#include "Canon.h"

void ASpawnerEnemy::BeginPlay()
{
	//I Use the Begin for the Debuging
	//GetWorldTimerManager().SetTimer(SpawnTimer, this, &ASpawnerEnemy::SpawnEnemy,
	//	Frequency, true, 0.f);
}

void ASpawnerEnemy::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	if(IsOverlapped == true)
		return;
	IsOverlapped = true;	
}

void ASpawnerEnemy::NotifyActorEndOverlap(AActor* OtherActor)
{
	IsOverlapped = false;
}

void ASpawnerEnemy::StartSpawner()
{
	EnemyCount = 0;
	GetWorldTimerManager().SetTimer(SpawnTimer, this, &ASpawnerEnemy::SpawnEnemy,
		Frequecy, true, 0.f);
}

void ASpawnerEnemy::SpawnEnemy()
{
	
	if (SpawnedActorType == nullptr)
		return;

	UWorld* MyWorld = GetWorld();
	if (MyWorld == nullptr)
		return;

	if (EnemyCount >= TotalEnemiesInSpawner)
	{
		GetWorldTimerManager().ClearTimer(SpawnTimer);
		bSpawnAllActors = true;
		return;
	}
	
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = this;
	SpawnParameters.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	for (unsigned int index = 0; index < NumbrEnemy; index++)
	{
		//const FVector2D RandomPoint = FMath::RandPointInCircle(Radius);

		const FVector SpawnLocation = GetActorLocation() + index;
		AActor* MyEnemy = MyWorld->SpawnActor<AActor>(SpawnedActorType,
			SpawnLocation, FRotator::ZeroRotator, SpawnParameters);
		if (MyEnemy == nullptr)
			return;
		EnemyCount++;
	}
}
