// Fill out your copyright notice in the Description page of Project Settings.


#include "TowerAIController.h"

#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

void ATowerAIController::BeginPlay()
{
	Super::BeginPlay();

	bool sucess = RunBehaviorTree(TowerBehavior);
	if(!sucess)
	{
		UE_LOG(LogTemp, Warning, TEXT("Behavior Tree could not run"));
		return;
	}
	TArray<AActor*> ActorsWithTag;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), TargetTag, ActorsWithTag);

	if (ActorsWithTag.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("There is no actor with tag to target"));
		return;
	}

	AActor* AITarget = ActorsWithTag[0];
	if (AITarget != nullptr)
	{
		GetBlackboardComponent()->SetValueAsObject(TargetKey, AITarget);
		GetBlackboardComponent()->SetValueAsBool(BoleanPathKey, true);
	}
}
