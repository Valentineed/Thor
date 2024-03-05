// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "DogAniminstance.generated.h"

/**
 * 
 */
UCLASS()
class PANDAPROJECT_API UDogAniminstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DogAnimation)
		bool IsWalking = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DogAnimation)
		bool IsAttacking = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DogAnimation)
		bool IsDead = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DogAnimation, 
		meta = (ClampMin = 0.0f, ClampMax = 100.0f))
		float WalkBlendRation = 0.0f;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
protected:
};
