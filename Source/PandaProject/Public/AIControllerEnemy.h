// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIControllerEnemy.generated.h"

/**
 * 
 */
UCLASS()
class PANDAPROJECT_API AAIControllerEnemy : public AAIController
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
	
	AAIControllerEnemy();
	
	UFUNCTION()
	void OnPlayerCapacity();

protected:
	UPROPERTY(EditDefaultsOnly, Category = EnemyAI)
		class UBehaviorTree* EnemyBehavior = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = EnemyAI)
		FName TargetTag = "Tower";
	
	UPROPERTY(EditDefaultsOnly, Category = EnemyAI)
		FName TargetKey = "Tower";

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = EnemyAI)
		float TimeCapacityPlayer = 5.0f;
private:
	FTimerHandle TimerCapacityPlayer;

	void ClearValueBlackboard();
};
