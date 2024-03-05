// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_CheckCapacity.generated.h"

/**
 * 
 */
UCLASS()
class PANDAPROJECT_API UBTService_CheckCapacity : public UBTService_BlackboardBase
{
  
  virtual void TickNode(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory, float DeltaSeconds) override;
	GENERATED_BODY()
	
};
