// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LifeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FNoMoreHP);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PANDAPROJECT_API ULifeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULifeComponent();

	UFUNCTION(BlueprintCallable)
		void AddLife(const float heal);

	UFUNCTION(BlueprintCallable)
		void Hit(const float damage);

	UFUNCTION(BlueprintCallable)
		const float GetLife() const;

	float& MutateLife();
	
	float InitLife = 0.0f;
	
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = Life)
	FNoMoreHP OnNoMoreHP;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Life Config")
		float Life = 10.f;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
