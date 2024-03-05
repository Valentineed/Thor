#include "Canon.h"

#include "EnemyCharacter.h"
#include "LifeComponent.h"
#include "TowerCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
// Sets default values
ACanon::ACanon()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    Life = CreateDefaultSubobject<ULifeComponent>(FName(TEXT("LifeComponentCanon")));
    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(FName(TEXT("Mesh")));
    Mesh2 = CreateDefaultSubobject<USkeletalMeshComponent>(FName(TEXT("Mesh2")));
    RangeAttack = CreateDefaultSubobject<USphereComponent>(TEXT("RangeAttack"));
    Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
    Particle = CreateDefaultSubobject<UParticleSystemComponent>(FName(TEXT("ParticleSystem")));
	LaserParticule = CreateDefaultSubobject<UParticleSystemComponent>("LaserParticles");
	SoundExplosion = CreateDefaultSubobject<UAudioComponent>(FName(TEXT("SoundExplosion")));
	SoundDeath = CreateDefaultSubobject<UAudioComponent>(FName(TEXT("SoundDeath")));
    if (Collision && Mesh && Mesh2 && RangeAttack && Particle )
    {
	    SetRootComponent(Collision);
	    RootComponent = Collision;
	    Mesh->SetupAttachment(RootComponent);
	    Mesh2->SetupAttachment(RootComponent);
	    RangeAttack->SetupAttachment(RootComponent);
	    Particle->SetupAttachment(RootComponent);
	    Particle->AttachTo(RootComponent);
	    Particle->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

    }
	if(SoundExplosion)
	{
		SoundExplosion->SetupAttachment(RootComponent);
		SoundExplosion->bAutoActivate = false;
		SoundDeath->SetupAttachment(RootComponent);
		SoundDeath->bAutoActivate = false;
	}
    
    
    TArray<AActor*> Actors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATowerCharacter::StaticClass(), Actors);
    if (Actors.Num() > 0) {
	    if (Actors[0] != nullptr)
	    {
		    Tower = Actors[0];
	    }

    }
}

// Called when the game starts or when spawned
void ACanon::BeginPlay()
{
    Super::BeginPlay();
    if(RangeAttack != nullptr) 
    {
	RangeAttack->OnComponentBeginOverlap.AddDynamic(this, &ACanon::BeginOverlapAttack);
	RangeAttack->OnComponentEndOverlap.AddDynamic(this, &ACanon::EndOverlapAttack);
    }
    if(Particle != nullptr)
    {
	    Particle->SetWorldLocation(GetActorLocation());
		Particle->DeactivateSystem();
    	//Particle->setsi
    }
    if(Life != nullptr)
    {
	    InitLife = Life->GetLife();
    }
}

float ACanon::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
  if (Life != nullptr)
  {
	  Life->Hit(DamageAmount);

	  if (Life->GetLife() == 0)
	  {
		  SoundDeath->Activate(true);
		  SoundDeath->Play(0.0f);
		  Destroy();
	  }
  }
  return DamageAmount;
}

// Called every frame
void ACanon::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);
  if(Target != nullptr ) 
  {
  	if(LaserParticule != nullptr)
  	{
		LaserParticule->SetBeamSourcePoint(0, GetActorLocation(), 0);
		LaserParticule->SetBeamTargetPoint(0, Target->GetActorLocation(), 0);
  	}
  	if(bCallTimerAttack)
  	{
		GetWorldTimerManager().SetTimer(AttackTimer, this, &ACanon::TryDamageZone, TimeToAttack, false);
		bCallTimerAttack = false;
  	}
  }
  else
  {
	  LaserParticule->SetBeamTargetPoint(0, GetActorLocation(), 0);
  }
}

void ACanon::TryDamageZone()
{
  if(Target != nullptr)
  {
  	if(SoundExplosion)
  	{
		SoundExplosion->Activate(true);
		SoundExplosion->Play(0.0f);
  	}
  	if(Particle)
  	{
		Particle->SetWorldLocation(Target->GetActorLocation());
		Particle->Activate(true);
  	}
	  TArray<AActor*>IgnoreActors;
	  UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyCharacter::StaticClass(), IgnoreActors);
	  UGameplayStatics::ApplyRadialDamage(GetWorld(), Damage, Target->GetActorLocation(),
		  RadiusDamage, UDamageType::StaticClass(), IgnoreActors, this);
  }
  bCallTimerAttack = true;
}

void ACanon::BeginOverlapAttack(UPrimitiveComponent *OverlappedComponent,
    AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult &SweepResult) {

    UCharacterMovementComponent* MovementComponent = OtherActor->FindComponentByClass<UCharacterMovementComponent>();
    if (MovementComponent)
    {
	    FString OtherComponent = OtherComp->GetName();
	    if (OtherComponent == "BoxCollisionJump" || OtherComponent == "AttackZoneFirst"
		    || OtherComponent == "BoxCollisionSecound" || OtherComponent == "Collision")
	    {
		    return;
	    }
	    UE_LOG(LogTemp, Warning, TEXT("Collision: %s"), *OtherComponent);

	    APawn* TargetInRange = MovementComponent->GetPawnOwner();

	    if (TargetInRange == Tower)
	    {
		    UE_LOG(LogTemp, Warning, TEXT("Begin Target ==Tower"));
		    Target = TargetInRange;
	    }
	    else if (TargetInRange->IsPlayerControlled())
	    {
		    UE_LOG(LogTemp, Warning, TEXT("Begin PlaYer"));
		    if (Target == nullptr)
		    {


			    UE_LOG(LogTemp, Warning, TEXT("Begin PlaYer Target ==nullptr"));
			    Target = TargetInRange;

		    }
		    Player = TargetInRange;
	    }
    }
}


void ACanon::EndOverlapAttack(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {

  UCharacterMovementComponent* MovementComponent = OtherActor->FindComponentByClass<UCharacterMovementComponent>();
  if (MovementComponent)
  {
	FString OtherComponent = OtherComp->GetName();
	if (OtherComponent == "BoxCollisionJump" || OtherComponent == "AttackZoneFirst" ||
		OtherComponent == "BoxCollisionSecound" || OtherComponent == "Collision")
	{
		return;
	}

	APawn* TargetInRange = MovementComponent->GetPawnOwner();
	if (TargetInRange == Tower && Player != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("End Tower -> Player == Target"));
		Target = Player;
		Player = nullptr;
	}
	else if (TargetInRange == Tower)
	{
		UE_LOG(LogTemp, Warning, TEXT("End Tower "));
		Target = nullptr;
	}
	else if (TargetInRange->IsPlayerControlled() && Target != Tower)
	{
		UE_LOG(LogTemp, Warning, TEXT("End Player Target != Tower "));

		Target = nullptr;
		Player = nullptr;
	}
  }
}
