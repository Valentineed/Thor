// Fill out your copyright notice in the Description page of Project Settings.


#include "AIControllerEnemy.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PandaProject/PandaProjectCharacter.h"
#include "Perception/AIPerceptionComponent.h"
AAIControllerEnemy::AAIControllerEnemy()
{
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(FName("Perception"));
}

void AAIControllerEnemy::OnPlayerCapacity()
{
	GetBlackboardComponent()->SetValueAsObject(FName("PlayerActor"),GetWorld()->GetFirstPlayerController()->GetPawn());
	GetBlackboardComponent()->SetValueAsBool("PlayerCapacity", true);
	GetWorldTimerManager().SetTimer(TimerCapacityPlayer, this,
		&AAIControllerEnemy::ClearValueBlackboard, TimeCapacityPlayer);
	
}

void AAIControllerEnemy::ClearValueBlackboard()
{
	GetBlackboardComponent()->ClearValue(FName("PlayerActor"));
	GetBlackboardComponent()->SetValueAsBool("PlayerCapacity", false);
}

void AAIControllerEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	APandaProjectCharacter* player = Cast<APandaProjectCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if(player != nullptr)
	{
		player->CapacityDelegate.AddDynamic(this, &AAIControllerEnemy::OnPlayerCapacity);
	}

	bool Success = RunBehaviorTree(EnemyBehavior);
	if(!Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Behavior Tree could not run"));
		return;
	}

	TArray<AActor*> ActorsWithTag;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), TargetTag, ActorsWithTag);

	if(ActorsWithTag.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("There is no actor with tag to target"));
		return;
	}

	AActor* AITarget = ActorsWithTag[0];
	if(AITarget != nullptr)
	{
		GetBlackboardComponent()->SetValueAsObject(TargetKey, AITarget);
	}
}
