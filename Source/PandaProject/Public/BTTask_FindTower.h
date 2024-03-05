// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_FindTower.generated.h"

/**
 * 
 */
UCLASS()
class PANDAPROJECT_API UBTTask_FindTower : public UBTTask_BlackboardBase
{
	//I left this class because I'm leaving
	GENERATED_BODY()
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory) override;
};
