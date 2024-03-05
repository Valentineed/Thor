// Copyright Epic Games, Inc. All Rights Reserved.

#include "PandaProjectCharacter.h"


#include "Canon.h"
#include "DrawDebugHelpers.h"
#include "EnemyCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Components/AudioComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "PlayerAnimInstance.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "LifeComponent.h"
#include "PandaProjectGameMode.h"
#include "TowerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

APandaProjectCharacter::APandaProjectCharacter()
{
    Life = CreateDefaultSubobject<ULifeComponent>(FName(TEXT("LifeComponent")));
    AttackZoneFirst = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackZoneFirst"));
    AttackZoneSecond = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollisionSecound"));
    JumpCollisionDown = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollisionJump"));
    SphereJump = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SphereJump"));
    MeshZoneFirst = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshZoneFirst"));
    MeshZoneSecond = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshZoneSecond"));

    InitSounds();
    InitParticles();

    if (AttackZoneFirst)
    {
        AttackZoneFirst->SetupAttachment(RootComponent);
        AttackZoneFirst->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
        if (MeshZoneFirst != nullptr) 
        {
            MeshZoneFirst->SetupAttachment(AttackZoneFirst);
        }
    }
    
    if (AttackZoneSecond)
    {
        AttackZoneSecond->SetupAttachment(RootComponent);
        AttackZoneSecond->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
        if (MeshZoneSecond != nullptr)
        {
            MeshZoneSecond->SetupAttachment(AttackZoneSecond);
        }
    }

    if (JumpCollisionDown)
    {
        JumpCollisionDown->SetupAttachment(RootComponent);
        JumpCollisionDown->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
    }
    if(SphereJump)
    {
      SphereJump->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
    }

    // Set size for player capsule
    GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

    // Don't rotate character to camera direction
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    // Configure character movement
    GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
    GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
    GetCharacterMovement()->bConstrainToPlane = true;
    GetCharacterMovement()->bSnapToPlaneAtStart = true;

    // Create a camera boom...
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
    CameraBoom->TargetArmLength = 800.f;
    CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
    CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

    // Create a camera...
    TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
    TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

    // Create a decal in the world to show the cursor's location
    JumpResultUI = CreateDefaultSubobject<UDecalComponent>("JumpResultUI");
    JumpResultUI->SetupAttachment(RootComponent);
    static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAsset(TEXT("Material'/Game/TopDownCPP/Blueprints/M_Cursor_Decal.M_Cursor_Decal'"));
    if (DecalMaterialAsset.Succeeded())
    {
        JumpResultUI->SetDecalMaterial(DecalMaterialAsset.Object);
    }
    JumpResultUI->DecalSize = FVector(16.0f, 32.0f, 32.0f);
    JumpResultUI->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());
    // Activate ticking in order to update the cursor every frame.
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.bStartWithTickEnabled = true;

    ImpactRadius = 300.f;
    RadialImpactForce = 5000.f;
}

void APandaProjectCharacter::BeginPlay()
{
    Super::BeginPlay();
    UPlayerAnimInstance* PlayerAnim = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());
    if (PlayerAnim != nullptr)
    {
        PlayerAnim->BeginAttack.AddDynamic(this, &APandaProjectCharacter::OnAttackBegin);
        PlayerAnim->EndAttack.AddDynamic(this, &APandaProjectCharacter::OnAttackEnd);
    }
    if (JumpCollisionDown != nullptr)
    {
        JumpCollisionDown->OnComponentBeginOverlap.AddDynamic(this, &APandaProjectCharacter::BeginOverlapJumpZone);
        JumpCollisionDown->OnComponentEndOverlap.AddDynamic(this, &APandaProjectCharacter::EndOverlapJumpZone);
        JumpCollisionDown->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }
    if (AttackZoneFirst != nullptr && AttackZoneSecond != nullptr)
    {
        AttackZoneFirst->OnComponentBeginOverlap.AddDynamic(this, &APandaProjectCharacter::BeginOverlapZoneFirst);
        AttackZoneFirst->SetCollisionEnabled(ECollisionEnabled::NoCollision);

        AttackZoneSecond->OnComponentBeginOverlap.AddDynamic(this, &APandaProjectCharacter::BeginOverlapZoneSecond);
        AttackZoneSecond->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }
    if(SphereJump) 
    {
        SphereJump->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        SphereJump->SetVisibility(false);
    }
    if(MeshZoneFirst)
    {
        MeshZoneFirst->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        MeshZoneFirst->SetVisibility(false);
      //MeshZoneFirst->SetStaticMesh()
    }
    if(MeshZoneSecond)
    {
        MeshZoneSecond->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        MeshZoneSecond->SetVisibility(false);
    }

    TArray<AActor*> Actors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATowerCharacter::StaticClass(), Actors);
    if (Actors.Num() > 0)
    {
        if (Actors[0] != nullptr)
        {
            Tower = Actors[0];
        }
    }
    if (Life != nullptr)
    {
        InitLife = Life->GetLife();
    }
    DeactivateParticles();
    JumpVelocity = GetCharacterMovement()->JumpZVelocity;
    DefaultGravity = GetCharacterMovement()->GravityScale;
}

void APandaProjectCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    UpdateJump(DeltaSeconds);

    ManaRegen(DeltaSeconds);

    LifeRegen(DeltaSeconds);

}

//------------Sounds----------//
void APandaProjectCharacter::InitSounds()
{
    //Create Default Subobject 
    {
        Sounds = CreateDefaultSubobject<USceneComponent>(FName("Sounds"));
        SoundUltimate = CreateDefaultSubobject<UAudioComponent>(FName(TEXT("SoundUltimate")));
        SoundJump = CreateDefaultSubobject<UAudioComponent>(FName(TEXT("SoundJump")));
        SoundJumpCharge = CreateDefaultSubobject<UAudioComponent>(FName(TEXT("SoundJumpCharge")));
        SoundCapacity = CreateDefaultSubobject<UAudioComponent>(FName(TEXT("SoundCapacity")));
    }

    //SetupAttachment and init Config
    {
        if (Sounds != nullptr)
        {
            Sounds->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
            if (SoundUltimate)
            {
                SoundUltimate->SetupAttachment(Sounds);
                SoundUltimate->bAutoActivate = false;
            }
            if (SoundJump)
            {
                SoundJump->SetupAttachment(Sounds);
                SoundJump->bAutoActivate = false;
            }
            if (SoundJumpCharge)
            {
                SoundJumpCharge->SetupAttachment(Sounds);
                SoundJumpCharge->bAutoActivate = false;
            }
            if (SoundCapacity)
            {
                SoundCapacity->SetupAttachment(Sounds);
                SoundCapacity->bAutoActivate = false;
            }
        }
    }
}

//------------Particles----------//
void APandaProjectCharacter::InitParticles()
{
    //Create Default Suboject
    {
        Particles = CreateDefaultSubobject<USceneComponent>(FName("Particles"));
        ParticleJump = CreateDefaultSubobject<UParticleSystemComponent>(FName(TEXT("ParticleSystem")));
        ParticleUltimate = CreateDefaultSubobject<UParticleSystemComponent>(FName(TEXT("ParticleUltimate")));
        ParticleCapacity = CreateDefaultSubobject<UParticleSystemComponent>(FName(TEXT("ParticleCapacity")));
    }

    //SetupAttachment and init Config
    {
        if (Particles != nullptr)
        {
            Particles->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

            if (ParticleJump != nullptr)
            {
                ParticleJump->AttachToComponent(Particles, FAttachmentTransformRules::KeepRelativeTransform);

            }
            if (ParticleUltimate != nullptr)
            {
                ParticleUltimate->AttachToComponent(Particles, FAttachmentTransformRules::KeepRelativeTransform);

            }
            if (ParticleCapacity != nullptr)
            {
                ParticleCapacity->AttachToComponent(Particles, FAttachmentTransformRules::KeepRelativeTransform);
            }
        }
    }
}

void APandaProjectCharacter::DeactivateParticles()
{
    if (ParticleJump != nullptr)
    {
        ParticleJump->DeactivateSystem();
    }
    if (ParticleUltimate != nullptr)
    {
        ParticleUltimate->DeactivateSystem();
    }
    if (ParticleCapacity != nullptr)
    {
        ParticleCapacity->DeactivateSystem();
    }
}

//------------Life/Mana----------//
float APandaProjectCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
    AController* EventInstigator, AActor* DamageCauser)
{
    if (Life != nullptr)
    {
        Life->Hit(DamageAmount);
        ResetCombo();
        if (Life->GetLife() == 0)
        {
            UPlayerAnimInstance* PlayerAniminstance = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());
            if (PlayerAniminstance != nullptr)
            {
                PlayerAniminstance->IsDead = true;
            }
            APandaProjectGameMode* GameMode = Cast<APandaProjectGameMode>(GetWorld()->GetAuthGameMode());
            if (GameMode)
            {
                GameMode->OnGameOver();
            }
        }
    }
    return DamageAmount;
}

//------------Movements-----------//
void APandaProjectCharacter::MoveForward(float Value)
{
    if (Value != 0.0f)
    {
        // find out which way is forward
        const FRotator Rotation = GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        // get forward vector
        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        AddMovementInput(Direction, Value);
    }
}

void APandaProjectCharacter::MoveRight(float Value)
{
    if (Value != 0.0f)
    {
        // find out which way is right
        const FRotator Rotation = GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        // get right vector 
        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
        // add movement in that direction
        AddMovementInput(Direction, Value);
    }
}

void APandaProjectCharacter::LifeRegen(float DeltaSeconds)
{
    ElapseLifeRegen += DeltaSeconds;
    if (TimeToAddLife <= ElapseLifeRegen)
    {
        if (Life->GetLife() == InitLife)
        {
            return;
        }
        if (Life->GetLife() > InitLife)
        {
            Life->MutateLife() = InitLife;
            return;
        }
        Life->AddLife(AmountLifeRegen);
        ElapseLifeRegen = 0.0f;
    }
}

void APandaProjectCharacter::AddMana(float AmountManaAdd)
{
    if (bInUltimate)
        return;
    if (CurrentMana < MaxMana)
    {
        CurrentMana += AmountManaAdd;
    }
    else if (CurrentMana > MaxMana)
    {
        CurrentMana = MaxMana;
    }
}

void APandaProjectCharacter::CanAddMana()
{
    bInUltimate = false;
}

void APandaProjectCharacter::ManaRegen(float DeltaSeconds)
{
    ElapseManaRegen += DeltaSeconds;
    if (TimeUltimateToAddMana <= ElapseManaRegen)
    {
        ElapseManaRegen = 0.f;
        CurrentMana += AmountManaRegen;
        if (CurrentMana > 100.f)
        {
            CurrentMana = 100.f;
        }
    }
}

//------------Basic Attacks------------//
void APandaProjectCharacter::OnAttackBegin()
{
    SetupCombo();
    switch (ComboCount)
    {
    case (int)AttackSelector::DEFAULT:
        ReceiveBeginAttack();
        break;
    case (int)AttackSelector::SECOND:
        AttackZoneFirst->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
        if (MeshZoneFirst) 
        {
            MeshZoneFirst->SetVisibility(true);
        }
        break;
    case (int)AttackSelector::THIRD:
        AttackZoneSecond->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
        if (MeshZoneSecond) 
        {
            MeshZoneSecond->SetVisibility(true);
        }
        break;
    default:
        ReceiveBeginAttack();
    }
}

void APandaProjectCharacter::OnAttackEnd()
{
    if (MeshZoneFirst) 
    {
        MeshZoneFirst->SetVisibility(false);
    }
    if (MeshZoneSecond) 
    {
        MeshZoneSecond->SetVisibility(false);
    }
    ReceiveEndAttack();
    AttackZoneSecond->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    AttackZoneFirst->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void APandaProjectCharacter::StartAttack()
{
    UPlayerAnimInstance* PlayerAnim = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());
    if (PlayerAnim != nullptr)
    {
        PlayerAnim->IsAttacking = true;
    }
}

void APandaProjectCharacter::StopAttack()
{
    UPlayerAnimInstance* PlayerAnim = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());
    if (PlayerAnim != nullptr)
    {
        PlayerAnim->IsAttacking = false;
    }
}

void APandaProjectCharacter::ResetCombo()
{
    ComboCount = 0;
}

void APandaProjectCharacter::SetupCombo()
{
    if (ComboCount == (int)AttackSelector::THIRD)
    {
        return;
    }
    ComboCount++;
}

void APandaProjectCharacter::BeginOverlapZoneFirst(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    //Todo add end Garantir one damge
    if (OtherActor != Tower && OtherActor != this)
    {
        if (OtherComp->GetName() == "RangeAttack")
            return;
        UGameplayStatics::ApplyDamage(OtherActor, DamageFirstZone, Controller,
            this, UDamageType::StaticClass());
    }
}

void APandaProjectCharacter::BeginOverlapZoneSecond(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor != Tower && OtherActor != this)
    {
        if (OtherComp->GetName() == "RangeAttack")
            return;
        UGameplayStatics::ApplyDamage(OtherActor, DamageSecondZone, Controller,
            this, UDamageType::StaticClass());
    }
}

//--------SpecialAttack-------//
void APandaProjectCharacter::Ultimate()
{
    if (CurrentMana >= MaxMana || bTestCameraShake)
    {
        if (!bTestCameraShake)
        {
            bInUltimate = true;
            CurrentMana -= MaxMana;
        }
        if (ShakeUltimate != nullptr)
        {
            GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayCameraShake(ShakeUltimate);
        }
        if (ParticleUltimate != nullptr)
        {
            ParticleUltimate->Activate(true);
        }
        if (SoundUltimate)
        {
            SoundUltimate->Activate(true);
            SoundUltimate->Play(0.0f);
        }
        TArray<AActor*> FoundActors;
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyCharacter::StaticClass(), FoundActors);
        for (AActor* Element : FoundActors)
        {
            if (Element != nullptr)
            {
                Cast<AEnemyCharacter>(Element)->ActiveParticle();
                UGameplayStatics::ApplyDamage(Element, DamageUltimate, Controller,
                    this, UDamageType::StaticClass());
            }
        }
        if (bCanAttackAllEnemies)
        {
            TArray<AActor*> FoundTowers;
            UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACanon::StaticClass(), FoundActors);
            for (AActor* Element : FoundActors)
            {
                if (Element != nullptr)
                {
                    UGameplayStatics::ApplyDamage(Element, DamageUltimate, Controller,
                        this, UDamageType::StaticClass());
                }
            }
        }
        GetWorldTimerManager().SetTimer(InUltimateTimer, this,
            &APandaProjectCharacter::CanAddMana, DelayToAddMana);
    }
}

void APandaProjectCharacter::Capacity()
{
    float Check = CurrentMana - AmountMana;
    if (Check >= 0.f)
    {

        CurrentMana -= AmountMana;
        CapacityDelegate.Broadcast();
    }
    if (SoundCapacity)
    {
        SoundCapacity->Activate(true);
        SoundCapacity->Play(0.0f);
    }
    if (ParticleCapacity != nullptr)
    {
        ParticleCapacity->Activate(true);
    }

}

//-------------Jump Section----------------//

void APandaProjectCharacter::UpdateJump(float DeltaSeconds)
{
    JumpUI(DeltaSeconds);
    CheckEnableImpulse();
}

void APandaProjectCharacter::CheckEnableImpulse()
{
    if (JumpImpulse)
    {
        if (JumpCollisionDown != nullptr)
        {
            if (CanJump())
            {
                GetCharacterMovement()->JumpZVelocity = JumpVelocity;
                GetCharacterMovement()->GravityScale = DefaultGravity;
                ImpulseZone();
                JumpImpulse = false;
                JumpCollisionDown->SetCollisionEnabled(ECollisionEnabled::NoCollision);
                GetWorldTimerManager().SetTimer(JumpTimerStopAttack, this, &APandaProjectCharacter::StopAttack, TimeToStopAnimAttack);
            }
        }
    }
}

void APandaProjectCharacter::ImpulseZone()
{
    if (ShakeJump != nullptr)
    {
        GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayCameraShake(ShakeJump);
    }
    if (ParticleJump != nullptr)
    {
        ParticleJump->Activate(true);
    }
    FCollisionShape SphereCol = FCollisionShape::MakeSphere(ImpactRadius);
    bool SweepHit = GetWorld()->SweepMultiByChannel(HitActors, GetActorLocation(),
        GetActorLocation() + FVector(0.1f, 0.1f, 0.f), FQuat::Identity,
        ECC_WorldStatic, SphereCol);

    SphereJump->SetVisibility(true);

    GetWorldTimerManager().SetTimer(JumpTimer, this, &APandaProjectCharacter::HidenJumpSphere, TimingSphereJump);
    //Debug
  // DrawDebugSphere(GetWorld(), GetActorLocation(), ImpactRadius, 50, FColor::Orange, false, 2.0f);

    if (SweepHit)
    {
        TArray<AActor*> Actors;
        Algo::Reverse(HitActors);
        for (auto& Actorhit : HitActors)
        {
            if (Actors.Find(Actorhit.GetActor()) == INDEX_NONE)
            {
                Actors.Add(Actorhit.GetActor());

                UCharacterMovementComponent* MovementComponent = Actorhit.GetActor()->FindComponentByClass<UCharacterMovementComponent>();
                if (MovementComponent)
                {
                    APawn* enemy = MovementComponent->GetPawnOwner();
                    if (!enemy->IsPlayerControlled() && enemy != Tower)
                    {
                        MovementComponent->GetPawnOwner()->GetController()->StopMovement();
                        MovementComponent->AddRadialImpulse(Actorhit.Location, ImpactRadius, RadialImpactForce,
                            ERadialImpulseFalloff::RIF_Constant, true);

                        //MovementComponent->AddForce(Actorhit.Location + FVector(0.f, 0.f, 1.f));

                        UGameplayStatics::ApplyDamage(Actorhit.GetActor(), DamageJumpAttack, Controller,
                            this, UDamageType::StaticClass());
                    }
                }
            }
        }
    }
}

void APandaProjectCharacter::JumpUI(float DeltaSeconds)
{
    if (bJumpPressed)
    {
        ElapseJump += DeltaSeconds;
        if (ElapseJump > TimeChargeJump)
            ElapseJump = TimeChargeJump;
    }
    else if (bJumpPressed == false && bJumpRelease == true)
    {
        bJumpRelease = false;
        ElapseJump = 0;
    }
    if (JumpResultUI != nullptr)
    {
        if (APlayerController* PC = Cast<APlayerController>(GetController()))
        {
            FHitResult TraceHitResult;
            PC->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);
            //FVector CursorFV = TraceHitResult.ImpactNormal;
            //FRotator CursorR = CursorFV.Rotation();

            FHitResult HitResult;
            FCollisionQueryParams Params(NAME_None, FCollisionQueryParams::GetUnknownStatId());
            FVector StartLocation = GetActorLocation();
            FVector EndLocation = { GetActorLocation().X ,GetActorLocation().Y ,GetActorLocation().Z - 200 };
            GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, Params);
            JumpResultUI->SetWorldLocation({ GetActorLocation().X + GetActorForwardVector().X * FMath::Abs(GetCharacterMovement()->Velocity.X) ,
           GetActorLocation().Y + GetActorForwardVector().Y * FMath::Abs(GetCharacterMovement()->Velocity.Y) , HitResult.Location.Z });
        }
    }
    FHitResult HitR;
    Landed(HitR);
}

void APandaProjectCharacter::JumpInterpolation(float Alpha)
{
    //Reset Values
    if (Alpha > 1.0f)
        Alpha = 1.0f;

    GetCharacterMovement()->JumpZVelocity = JumpVelocity;
    GetCharacterMovement()->GravityScale = DefaultGravity;
    //Interpolation
    float JumpChargeCompletVelocity = GetCharacterMovement()->JumpZVelocity + HeightForce;
    GetCharacterMovement()->JumpZVelocity = FMath::Lerp(GetCharacterMovement()->JumpZVelocity, JumpChargeCompletVelocity, Alpha);

    float JumpChargeCompletGravityScale = GetCharacterMovement()->GravityScale + DownForce;
    GetCharacterMovement()->GravityScale = FMath::Lerp(GetCharacterMovement()->GravityScale, JumpChargeCompletGravityScale, Alpha);
}

void APandaProjectCharacter::HidenJumpSphere()
{
    SphereJump->SetVisibility(false);
}

void APandaProjectCharacter::BeginOverlapJumpZone(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    UCharacterMovementComponent* MovementComponent = OtherActor->FindComponentByClass<UCharacterMovementComponent>();
    if (MovementComponent)
    {
        APawn* enemy = MovementComponent->GetPawnOwner();
        if (!enemy->IsPlayerControlled() && enemy != Tower)
        {
            UGameplayStatics::ApplyDamage(OtherActor, 1000, Controller,
                this, UDamageType::StaticClass());
        }
    }
}

void APandaProjectCharacter::EndOverlapJumpZone(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    UCharacterMovementComponent* MovementComponent = OtherActor->FindComponentByClass<UCharacterMovementComponent>();
    if (MovementComponent)
    {
        APawn* enemy = MovementComponent->GetPawnOwner();
        if (!enemy->IsPlayerControlled() && enemy != Tower)
        {
            UGameplayStatics::ApplyDamage(OtherActor, 1000, Controller,
                this, UDamageType::StaticClass());
        }
    }
}

//-------------JumpKey------------//
void APandaProjectCharacter::LoadJumpAttack()
{
    StartLoadJump = GetWorld()->GetTimeSeconds();
    bJumpPressed = true;
}

void APandaProjectCharacter::JumpAttack()
{
    bJumpPressed = false;
    bJumpRelease = true;
    EndLoadJump = GetWorld()->GetTimeSeconds();
    ElapseJump = EndLoadJump - StartLoadJump;
    JumpInterpolation(ElapseJump / TimeChargeJump);
    if (ElapseJump >= TimeChargeJump)
    {
        JumpCollisionDown->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
        JumpImpulse = true;
        if (SoundJumpCharge)
        {
            SoundJumpCharge->Activate(true);
            SoundJumpCharge->Play(0.0f);
        }
        Jump();


        GetWorldTimerManager().SetTimer(JumpTimerAttack, this, &APandaProjectCharacter::StartAttack, TimeToActiveAnimAttack);
    }
    else
    {
        if (SoundJump)
        {
            SoundJump->Activate(true);
            SoundJump->Play(0.0f);
        }
        Jump();
    }
}
