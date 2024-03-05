// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TowerAIController.generated.h"

/**
 * 
 */
UCLASS()
class PANDAPROJECT_API ATowerAIController : public AAIController
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
protected:
	UPROPERTY(EditDefaultsOnly, Category = TowerAI)
		class UBehaviorTree* TowerBehavior = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = TowerAI)
		FName TargetTag = "PatrolPath";

	UPROPERTY(EditDefaultsOnly, Category = TowerAI)
		FName TargetKey = "PatrolPath";
	
	UPROPERTY(EditDefaultsOnly, Category = TowerAI)
		FName BoleanPathKey = "PathLooping";
};
