// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"
#include "Canon.generated.h"

class USphereComponent;
class UBoxComponent;
class ULifeComponent;
class UParticleSystemComponent;
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FComponentEndOverlapSignature, class AActor*, OtherActor, class UPrimitiveComponent*, OtherComp, int32, OtherBodyIndex);
UCLASS()
class PANDAPROJECT_API ACanon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACanon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Canon")
		UBoxComponent* Collision = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Canon")
		USphereComponent* RangeAttack = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Canon")
		float InitLife;
	
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* Mesh = nullptr;

	UPROPERTY(VisibleAnywhere)
		USkeletalMeshComponent* Mesh2 = nullptr;
	
	UPROPERTY(VisibleAnywhere)
		class UParticleSystemComponent* Particle;

	UPROPERTY(VisibleAnywhere)
		UAudioComponent* SoundExplosion = nullptr;

	UPROPERTY(VisibleAnywhere)
		UAudioComponent* SoundDeath = nullptr;
	
	UPROPERTY(EditDefaultsOnly)
		UParticleSystemComponent* LaserParticule = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Canon")
		ULifeComponent* Life = nullptr;

	UPROPERTY(EditAnywhere, Category = "Canon")
		float Damage = 2;
	
	UPROPERTY(EditAnywhere, Category = "Canon")
		float RadiusDamage = 100;

	UPROPERTY(EditAnywhere, Category = "Canon")
		float TimeToAttack = 0.5;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TowerRef")
		AActor* Tower = nullptr;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
private:
	UFUNCTION()
		void BeginOverlapAttack(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void EndOverlapAttack(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	AActor* Target = nullptr;
	AActor* Player = nullptr;
	bool bCallTimerAttack = true;

	FTimerHandle AttackTimer;
	void TryDamageZone();
};
