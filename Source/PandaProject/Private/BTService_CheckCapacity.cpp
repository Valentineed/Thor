// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_CheckCapacity.h"

#include "Kismet/GameplayStatics.h"
#include "PandaProject/PandaProjectCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

void UBTService_CheckCapacity::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	APandaProjectCharacter* MyPlayer = static_cast<APandaProjectCharacter*>(UGameplayStatics::GetPlayerPawn(this, 0));
	//(MyPlayer->bCapacity)
	UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();
	if(MyBlackboard != nullptr)
	{
		bool bCapacity = MyBlackboard->GetValueAsBool(FName("PlayerCapacity"));
		
	}
}
