// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"



#include "PandaProjectCharacter.generated.h"

class USphereComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCpacityDelegate);
class UBoxComponent;
class ULifeComponent;
UCLASS(Blueprintable)
class APandaProjectCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APandaProjectCharacter();
	virtual void BeginPlay() override;
	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns JumpResultUI subobject **/
	FORCEINLINE class UDecalComponent* GetCursorToWorld() { return JumpResultUI; }

	//ATTACK
	UFUNCTION()
		void OnAttackBegin();
	UFUNCTION()
		void OnAttackEnd();
	void StartAttack();
	void StopAttack();

	//Movements
	void MoveForward(float Value);
	void MoveRight(float Value);
	//Jump
	void LoadJumpAttack();
	void JumpAttack();
	//Ultimate
	void Ultimate();
	UFUNCTION()
		void AddMana(float AmountManaAdd);
	//Capacity
	void Capacity();

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = Capacity)
		FCpacityDelegate CapacityDelegate;

	UPROPERTY(EditAnywhere, Category = CameraShake)
		TSubclassOf<UCameraShake> ShakeUltimate;

	UPROPERTY(EditAnywhere, Category = CameraShake)
		TSubclassOf<UCameraShake> ShakeJump;
private:
	enum class AttackSelector
	{
		DEFAULT = 1,
		SECOND,
		THIRD,
		END//Last Element
	};

	void ResetCombo();
	void SetupCombo();
	int ComboCount = 0;
	FTimerHandle ComboTimer;

	void UpdateJump(float DeltaSeconds);
	void CheckEnableImpulse();
	void ImpulseZone();
	void JumpUI(float DeltaSeconds);
	void JumpInterpolation(float Alpha);
	FTimerHandle JumpTimerAttack;
	FTimerHandle JumpTimerStopAttack;
	FTimerHandle JumpTimer;
	void HidenJumpSphere();
	bool JumpImpulse = false;
	float StartLoadJump;
	float EndLoadJump;
	float JumpVelocity;
	float DefaultGravity;
	bool bJumpPressed = false;
	bool bJumpRelease = false;

	TArray<FHitResult> HitActors;


	FTimerHandle InUltimateTimer;
	bool bInUltimate = false;
	UFUNCTION()
		void CanAddMana();
	float ElapseLifeRegen = 0.0f;
	void LifeRegen(float DeltaSeconds);
	float ElapseManaRegen = 0;
	void ManaRegen(float DeltaSeconds);

	void InitSounds();
	void InitParticles();
	void DeactivateParticles();
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** A decal that projects to the cursor location. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UDecalComponent* JumpResultUI;

	UFUNCTION()
		virtual void BeginOverlapJumpZone(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void EndOverlapJumpZone(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		virtual void BeginOverlapZoneFirst(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		virtual void BeginOverlapZoneSecond(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
protected:
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UPROPERTY(VisibleAnywhere)
		USceneComponent* Sounds = nullptr;

	UPROPERTY(VisibleAnywhere)
		UAudioComponent* SoundUltimate = nullptr;

	UPROPERTY(VisibleAnywhere)
		UAudioComponent* SoundJump = nullptr;

	UPROPERTY(VisibleAnywhere)
		UAudioComponent* SoundJumpCharge = nullptr;

	UPROPERTY(VisibleAnywhere)
		UAudioComponent* SoundCapacity = nullptr;

	UPROPERTY(VisibleAnywhere)
		USceneComponent* Particles = nullptr;

	UPROPERTY(VisibleAnywhere)
		UParticleSystemComponent* ParticleJump = nullptr;

	UPROPERTY(VisibleAnywhere)
		UParticleSystemComponent* ParticleUltimate = nullptr;

	UPROPERTY(VisibleAnywhere)
		UParticleSystemComponent* ParticleCapacity = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TowerRef")
		AActor* Tower = nullptr;

	UFUNCTION(BlueprintImplementableEvent, Category = "PandaProject")
		void ReceiveBeginAttack();
	UFUNCTION(BlueprintImplementableEvent, Category = "PandaProject")
		void ReceiveEndAttack();

	UPROPERTY(BlueprintReadOnly, Category = "Player")
		float InitLife;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player")
		ULifeComponent* Life = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player")
		float CurrentMana;

	UPROPERTY(EditAnywhere, Category = "Player")
		float AmountLifeRegen = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Ultimate")
		float DelayToAddMana = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Player")
		float TimeToAddLife = 5.0f;

	UPROPERTY(EditAnywhere, Category = "Player Combo")
		float DamageFirstZone = 5.0f;

	UPROPERTY(EditAnywhere, Category = "Player Combo")
		float DamageSecondZone = 5.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Combo")
		UBoxComponent* AttackZoneFirst = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Combo")
		UStaticMeshComponent* MeshZoneFirst = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Combo")
		UBoxComponent* AttackZoneSecond = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Combo")
		UStaticMeshComponent* MeshZoneSecond = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player")
		float ElapseJump;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "JumpAttack")
		UBoxComponent* JumpCollisionDown = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SphereJump")
		UStaticMeshComponent* SphereJump = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SphereJump")
		float TimingSphereJump = 0.8f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "JumpAttack", meta = (clampMin = "0.1"))
		float TimeChargeJump = 1.0f;

	UPROPERTY(EditAnywhere, Category = "JumpAttack")
		float ImpactRadius;

	UPROPERTY(EditAnywhere, Category = "JumpAttack")
		float TimeToActiveAnimAttack = 0.3f;

	UPROPERTY(EditAnywhere, Category = "JumpAttack")
		float TimeToStopAnimAttack = 0.3f;

	UPROPERTY(EditAnywhere, Category = "JumpAttack")
		float HeightForce = 10;

	UPROPERTY(EditAnywhere, Category = "JumpAttack")
		float DownForce = 10;

	UPROPERTY(EditAnywhere, Category = "JumpAttack")
		float RadialImpactForce;

	UPROPERTY(EditAnywhere, Category = "JumpAttack")
		float DamageJumpAttack = 2.0f;

	UPROPERTY(EditAnywhere, Category = "Ultimate")
		float TimeUltimateToAddMana = 3.0f;

	UPROPERTY(EditAnywhere, Category = "Ultimate")
		float AmountManaRegen = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ultimate")
		float MaxMana = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ultimate")
		bool bCanAttackAllEnemies = true;

	UPROPERTY(EditAnywhere, Category = "Ultimate")
		float DamageUltimate = 2.0f;

	UPROPERTY(EditAnywhere, Category = "Capacity")
		float AmountMana = 25.f;

	UPROPERTY(EditAnywhere, Category = CameraShake)
		bool bTestCameraShake = true;
};

