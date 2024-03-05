// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TowerCharacter.generated.h"

class UBoxComponent;
class ULifeComponent;
UCLASS()
class PANDAPROJECT_API ATowerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATowerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Life")
		float InitLife;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Canon")
		UBoxComponent* Collision = nullptr;
	
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Life")
		ULifeComponent* Life = nullptr;
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Death")
	  float TimeAnimationDeath = 1.6f;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
private:
	void StopAttack(AActor* Enemy);
	
	FTimerHandle DeathTimer;
	void GameOver();
	
	float Delay = 0.5;
	
	UFUNCTION()
		void OnBeginOverlapCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnEndOverlapCollision(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
