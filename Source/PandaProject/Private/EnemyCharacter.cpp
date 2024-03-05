// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "..\Public\EnemyCharacter.h"


#include "DogAniminstance.h"
#include "LifeComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PandaProject/PandaProjectCharacter.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    Life = CreateDefaultSubobject<ULifeComponent>(FName(TEXT("LifeComponent")));
	Particle = CreateDefaultSubobject<UParticleSystemComponent>(FName("Particle"));
	if (Particle != nullptr)
	{
		Particle->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	}
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
    MyPlayer = static_cast<APandaProjectCharacter *>(UGameplayStatics::GetPlayerPawn(this, 0));
    if(Life != nullptr)
    {
	    InitLife = Life->GetLife();
    }
	if (Particle != nullptr)
	{
		Particle->DeactivateSystem();
	}
}

float AEnemyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
    if(Life != nullptr)
    {
	    Life->Hit(DamageAmount);
	    
	    if (Life->GetLife() == 0)
	    {
		    UDogAniminstance* DogAniminstance = Cast<UDogAniminstance>(GetMesh()->GetAnimInstance());
		    if (DogAniminstance != nullptr)
		    {
			    DogAniminstance->IsDead = true;
			    MyPlayer->AddMana(ManaToPlayer);
			    SetActorEnableCollision(false);
			    Controller->StopMovement();
		    }
	    }
    }
    return DamageAmount;
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);
  UDogAniminstance* DogAniminstance = Cast<UDogAniminstance>(GetMesh()->GetAnimInstance());
  if (DogAniminstance != nullptr)
  {
	if(DogAniminstance->IsDead == true)
	{
	    TimeToDeath -= DeltaTime;
	    if (TimeToDeath <= 0.f)
	    {
	      Destroy();
	    }
	}
  }
  TimeSpentLevitating += DeltaTime;
}

void AEnemyCharacter::ActiveParticle()
{
	if (Particle != nullptr)
	{
		Particle->Activate(true);
	}
}

void AEnemyCharacter::StartAttack()
{
  UDogAniminstance* DogAniminstance = Cast<UDogAniminstance>(GetMesh()->GetAnimInstance());
  if (DogAniminstance != nullptr)
  {
	  DogAniminstance->IsAttacking = true;
  }
}

void AEnemyCharacter::StopAttack()
{
  UDogAniminstance* DogAniminstance = Cast<UDogAniminstance>(GetMesh()->GetAnimInstance());
  if (DogAniminstance != nullptr)
  {
	  DogAniminstance->IsAttacking = false;
  }
}

