// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindPathPoint.h"
#include "AIController.h"
#include "TowerCharacter.h"

EBTNodeResult::Type UBTTask_FindPathPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ATowerCharacter* Tower = Cast<ATowerCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	//Tower->GetComponentsByTag()//todo
	return EBTNodeResult::InProgress;
}
