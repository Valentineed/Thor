// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAttackBeginDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAttackEndDelegate);
/**
 * 
 */
UCLASS()
class PANDAPROJECT_API UPlayerAnimInstance : public UAnimInstance
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
		float WalkBlendRatio = 0.0f;
	
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = Attack)
		FAttackBeginDelegate BeginAttack;
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = Attack)
		FAttackEndDelegate EndAttack;
	
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};
