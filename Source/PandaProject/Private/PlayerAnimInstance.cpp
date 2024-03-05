// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimInstance.h"


#include "GameFramework/CharacterMovementComponent.h"
#include "PandaProject/PandaProjectCharacter.h"

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	APandaProjectCharacter* MyPlayer = Cast<APandaProjectCharacter>(TryGetPawnOwner());

	if (MyPlayer != nullptr)
	{
		const float speed = MyPlayer->GetCharacterMovement()->Velocity.Size();
		IsWalking = !FMath::IsNearlyZero(speed);

		if (IsWalking)
		{
			const float MaxSpeed = MyPlayer->GetCharacterMovement()->MaxWalkSpeed;
			WalkBlendRatio = (speed / MaxSpeed * 100.f);
			//WalkBlendRation = (sp);
		}
	}
}
