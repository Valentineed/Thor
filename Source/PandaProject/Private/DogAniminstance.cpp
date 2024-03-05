// Fill out your copyright notice in the Description page of Project Settings.


#include "DogAniminstance.h"
#include "EnemyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
//I left this class because I'm leaving the animations for the end
void UDogAniminstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	//AEnemyCharacter* MyEnemy = Cast<AEnemyCharacter>(TryGetPawnOwner());

	//if(MyEnemy != nullptr)
	//{
	//	const float speed = MyEnemy->GetCharacterMovement()->Velocity.Size();
	//	IsWalking = !FMath::IsNearlyZero(speed);

	//	if(IsWalking)
	//	{
	//		//TODO
	//		//const float MaxSpeed = MyEnemy->GetCharacterMovement()->MaxWalkSpeed;
	//		//WalkBlendRation = (speed / MaxSpeed * 100.f);
	//		//WalkBlendRation = 0.0f;
	//	}
	//}
}
