// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_SensePlayer.h"
#include "Perception/AIPerceptionComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

void UBTService_SensePlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	AAIController* MyController = OwnerComp.GetAIOwner();
	UAIPerceptionComponent* AIPerception = nullptr;
	if (MyController != nullptr)
	{
		AIPerception = MyController->FindComponentByClass<UAIPerceptionComponent>();
	}
	
	if (AIPerception == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("AI controller does not have perception component !"));
		return;
	}
	TArray<AActor*>PerceivedActors;
	AIPerception->GetCurrentlyPerceivedActors(PerceivedSense, PerceivedActors);
	bool bPlayerCapacity = OwnerComp.GetBlackboardComponent()->GetValueAsBool("PlayerCapacity");
	if(bPlayerCapacity)
	{
		return;
	}
	for (auto Actor : PerceivedActors) 
	{
		APawn* MyPawn = Cast<APawn>(Actor);
	  if(MyPawn != nullptr && MyPawn->IsPlayerControlled())
	  {
			OwnerComp.GetBlackboardComponent()->SetValueAsObject("PlayerActor", Actor);
			bIsSense = true;
			break;
	  }
	  else if(bIsSense)
	  {
		 OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());
		 bIsSense = false;
	  }
	}

}
