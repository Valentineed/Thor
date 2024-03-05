// Fill out your copyright notice in the Description page of Project Settings.


#include "WaveManager.h"

#include "Canon.h"
#include "SpawnerEnemy.h"
#include "Kismet/GameplayStatics.h"
#include "EnemyCharacter.h"
#include "PandaProject/PandaProjectGameMode.h"

// Sets default values
AWaveManager::AWaveManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWaveManager::BeginPlay()
{
	Super::BeginPlay();
	if (Waves.Num() > 0)
	{
		if (Waves[0].Spawners[0] != nullptr)
		{
			for (ASpawnerEnemy* Spawner : Waves[0].Spawners)
			{
				if (Spawner != nullptr)
				{
					Spawner->StartSpawner();
				}
			}
		}
	}
}

// Called every frame
void AWaveManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    if(WaveCount == Waves.Num()) 
    {
        APandaProjectGameMode* GameMode = Cast<APandaProjectGameMode>(GetWorld()->GetAuthGameMode());
        if (GameMode)
	    {
	        GameMode->OnWinGame();
		    return;
        }
    }
	WaveIsRunning(Waves[WaveCount].Spawners);	
}

void AWaveManager::WaveIsRunning(TArray<ASpawnerEnemy *> CurrentWave)
{
	if (CurrentWave.Num() > 0)
	{
		bool NextWave = false;
		for (ASpawnerEnemy* Element : CurrentWave)
		{
			if (Element) 
			{
				if (Element->bSpawnAllActors) 
				{
					NextWave = true;
				}
				else {
					return;
				}
			}
		}
		if (NextWave)
		{
			TArray<AActor*> FoundDogs;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyCharacter::StaticClass(), FoundDogs);
			TArray<AActor*> FoundCanons;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACanon::StaticClass(), FoundCanons);
			if (FoundDogs.Num() > 0 || FoundCanons.Num() > 0)
			{
				return;
			}
			WaveCount++;
			GetWorldTimerManager().SetTimer(WaveTimer, this, &AWaveManager::StartWave,
				TimeBetweenEachWave);

			for (ASpawnerEnemy* Element : CurrentWave)
			{
				if (Element)
				{
					Element->bSpawnAllActors = false;

				}
			}
			//TODO ADD UI
		}
		
	}
}

void AWaveManager::StartWave()
{
    //GetWorldTimerManager().ClearTimer(WaveTimer);
    if(Waves.Num() > WaveCount)
    {
	  if(Waves[WaveCount].Spawners.Num() > 0) 
	  {
		  for (ASpawnerEnemy* Element : Waves[WaveCount].Spawners)
		  {
			  if (Element != nullptr)
			  {
				  Element->StartSpawner();
			  }
		  }
	  }
	  else 
	  {
		  WaveCount++;
	  }
	  
    }
}


