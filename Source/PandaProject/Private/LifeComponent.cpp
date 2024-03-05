// Fill out your copyright notice in the Description page of Project Settings.


#include "LifeComponent.h"
#include "..\Public\LifeComponent.h"

// Sets default values for this component's properties
ULifeComponent::ULifeComponent()
{
  // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
  // off to improve performance if you don't need them.
  PrimaryComponentTick.bCanEverTick = true;

  // ...
}


void ULifeComponent::AddLife(const float heal)
{
  Life += heal;
}

void ULifeComponent::Hit(const float damage)
{
  Life -= damage;
  if (Life < 0)
	  Life = 0;

  if(Life == 0) 
  {
	OnNoMoreHP.Broadcast();
  }
}

const float ULifeComponent::GetLife() const
{
  return Life;
}

float& ULifeComponent::MutateLife()
{
	return Life;
}

// Called when the game starts
void ULifeComponent::BeginPlay()
{
	Super::BeginPlay();
	
	InitLife = Life;
}


// Called every frame
void ULifeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

