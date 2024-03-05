// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PandaTowerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PANDAPROJECT_API UPandaTowerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DogAnimation)
		bool IsWalking = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DogAnimation)
		bool IsDead = false;

	//virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};
