#include "TowerCharacter.h"


#include "EnemyCharacter.h"
#include "LifeComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PandaProject/PandaProjectGameMode.h"
#include "Components/BoxComponent.h"
#include "PandaTowerAnimInstance.h"
// Sets default values
ATowerCharacter::ATowerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Life = CreateDefaultSubobject<ULifeComponent>(FName(TEXT("LifeComponent")));;
	Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	if(Collision != nullptr)
	{
		Collision->SetupAttachment(RootComponent);
	}
}

// Called when the game starts or when spawned
void ATowerCharacter::BeginPlay()
{
	Super::BeginPlay();
	if(Life != nullptr)
	{
		InitLife = Life->GetLife();
	}
	if(Collision != nullptr)
	{
		Collision->OnComponentBeginOverlap.AddDynamic(this, &ATowerCharacter::OnBeginOverlapCollision);
		Collision->OnComponentEndOverlap.AddDynamic(this, &ATowerCharacter::OnEndOverlapCollision);
	}
}

float ATowerCharacter::TakeDamage(float DamageAmount,
    FDamageEvent const &DamageEvent, AController *EventInstigator,
    AActor *DamageCauser) {

	
	if (Life != nullptr)
	{
		Life->Hit(DamageAmount);

		if (Life->GetLife() == 0)
		{
			if (Controller == nullptr || IsPendingKill())
			{
				return DamageAmount;
			}
			UPandaTowerAnimInstance* PandaAniminstance = Cast<UPandaTowerAnimInstance>(GetMesh()->GetAnimInstance());
			if(PandaAniminstance)
			{
				PandaAniminstance->IsDead = true;
			}
			Controller->UnPossess();
			GetWorldTimerManager().SetTimer(DeathTimer, this, &ATowerCharacter::GameOver, TimeAnimationDeath);
		}
	}
	return DamageAmount;
}

// Called every frame
void ATowerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATowerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ATowerCharacter::StopAttack(AActor* Enemy)
{
	if(Enemy != nullptr)
	{
		Cast<AEnemyCharacter>(Enemy)->bCanAttack = false;
	}
}

void ATowerCharacter::GameOver()
{
	APandaProjectGameMode* GameMode = Cast<APandaProjectGameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode) {
		GameMode->OnGameOver();
	}
}

void ATowerCharacter::OnBeginOverlapCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyCharacter::StaticClass(), FoundActors);
	if(FoundActors.Num() > 0)
	{
		for (AActor* Element : FoundActors)
		{
			if(Element != nullptr && OtherActor == Element)
			{
				if (OtherComp->GetName() != "DamageZone")
				{
					Cast<AEnemyCharacter>(OtherActor)->bCanAttack = true;
				}
			}
		}
	}
}

void ATowerCharacter::OnEndOverlapCollision(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyCharacter::StaticClass(), FoundActors);
	if (FoundActors.Num() > 0)
	{
		for (AActor* Element : FoundActors)
		{
			if (Element != nullptr && OtherActor == Element )
			{
				//FString test = OtherComp->GetName();
				if(OtherComp->GetName() != "DamageZone")
				{
					Cast<AEnemyCharacter>(OtherActor)->bCanAttack = false;
				}
				
			}
		}
	}
}

