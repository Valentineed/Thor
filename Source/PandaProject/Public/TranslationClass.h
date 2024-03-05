// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TranslationClass.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PANDAPROJECT_API UTranslationClass : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTranslationClass();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation")
	FVector RotationDir{0,0,0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation")
	float RotationPerSecond = 10.f;
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	FRotator Rotator(FVector rotationDir, float velocity);
};
