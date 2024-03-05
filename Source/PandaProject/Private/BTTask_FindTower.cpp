// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindTower.h"
#include "TowerCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

//I don't use this function anymore but I keep it as a souvenir
EBTNodeResult::Type UBTTask_FindTower::ExecuteTask(
    UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory) {
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATowerCharacter::StaticClass(), FoundActors);
    if(FoundActors[0] != nullptr)
	 OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), FoundActors[0]->GetActorLocation());

	return EBTNodeResult::InProgress;
}
