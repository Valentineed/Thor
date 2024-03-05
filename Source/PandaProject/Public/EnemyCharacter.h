// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

class USphereComponent;
class APandaProjectCharacter;
class ULifeComponent;

UCLASS()
class PANDAPROJECT_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ActiveParticle();

	UFUNCTION(BlueprintCallable)
	void StartAttack();

	UFUNCTION(BlueprintCallable)
	void StopAttack();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy")
		bool bCanAttack = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy")
		bool bCanCheckAttack = false;
protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
		float AttackDamage = 5.0f;
	
	UPROPERTY(EditAnywhere, Category = "Drop")
		float ManaToPlayer = 5.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
		float TimeToDeath = 7.f;

	UPROPERTY(BlueprintReadOnly, Category = "Enemy")
		float InitLife;

	UPROPERTY(VisibleAnywhere)
		UParticleSystemComponent* Particle = nullptr;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy")
	ULifeComponent* Life = nullptr;
	
private:
	float TimeSpentLevitating = 0.f;
	APandaProjectCharacter* MyPlayer;


};
