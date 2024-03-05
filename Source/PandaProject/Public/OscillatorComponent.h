// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OscillatorComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PANDAPROJECT_API UOscillatorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOscillatorComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	FComponentReference CompToOscillate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Oscillator")
	FVector MoveAmplitude {0, 0, 0};
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
