// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack.h"

#include "PandaProject/PandaProjectCharacter.h"
#include "AIController.h"
#include "EnemyCharacter.h"

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemyCharacter* Character = Cast<AEnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if(Character != nullptr)
	{
		Character->StartAttack();
	}
	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UBTTask_Attack::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemyCharacter* Character = Cast<AEnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (Character != nullptr)
	{
		Character->StopAttack();
	}
	return EBTNodeResult::Aborted;
}
