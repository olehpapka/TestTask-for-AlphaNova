// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/TT_BasePlayer.h"
#include "Components/InputComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "Enemy/TT_Target.h"
#include "TT_GameMode.h"
#include "DrawDebugHelpers.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SphereComponent.h"

ATT_BasePlayer::ATT_BasePlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	SetRootComponent(MeshComponent);

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->TargetArmLength = 300.0f;
	SpringArmComponent->SocketOffset = FVector{ 0.0f, 0.0f, 50.0f };
	SpringArmComponent->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);

	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SphereComponent->SetupAttachment(GetRootComponent());
}

void ATT_BasePlayer::BeginPlay()
{
	Super::BeginPlay();

	SetColor(FLinearColor::Red);
}

void ATT_BasePlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UE_LOG(LogTemp, Display, TEXT("Velocity: %f"), MeshComponent->GetPhysicsLinearVelocity().Size());

	DrawArrowBasedOnSpeed();
}

void ATT_BasePlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ATT_BasePlayer::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ATT_BasePlayer::MoveRight);
	PlayerInputComponent->BindAxis("TurnAround", this, &ATT_BasePlayer::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &ATT_BasePlayer::AddControllerPitchInput);
}

void ATT_BasePlayer::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	const auto Target = Cast<ATT_Target>(OtherActor);
	if (!Target)
		return;

	if (!Target->IsMarked())
	{
		const auto GameMode = Cast<ATT_GameMode>(GetWorld()->GetAuthGameMode());
		if (!GameMode)
			return;

		Target->SetNewColor(PlayerColor);
		GameMode->OnTargetMarkedDelegate.Broadcast(true);
		if (Target->IsCleaner())
		{
			GameMode->SetCleanersNum(GameMode->GetCleanersNum() - 1);
		}

		UGameplayStatics::PlaySound2D(GetWorld(), CollisionSound, 2.0f);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), CollisionEffect, Target->GetActorLocation(),
			FRotator::ZeroRotator, FVector{0.2f, 0.2f, 0.2f});
	}
}

void ATT_BasePlayer::MoveForward(float Amount)
{
	if (Amount == 0.0f)
		return;

	FVector ForceToAdd = CameraComponent->GetForwardVector();
	ForceToAdd.X *= Amount * 500.0f;
	ForceToAdd.Y *= Amount * 500.0f;
	MeshComponent->AddForce(ForceToAdd, FName{}, true);

	LimitSpeed();
}

void ATT_BasePlayer::MoveRight(float Amount)
{
	if (Amount == 0.0f)
		return;

	FVector ForceToAdd = CameraComponent->GetRightVector();
	ForceToAdd.X *= Amount * 500.0f;
	ForceToAdd.Y *= Amount * 500.0f;
	MeshComponent->AddForce(ForceToAdd, FName{}, true);

	LimitSpeed();
}

void ATT_BasePlayer::SetColor(const FLinearColor& Color)
{
	if (MeshComponent)
	{
		const auto DynMaterial = MeshComponent->CreateAndSetMaterialInstanceDynamic(0);
		if (DynMaterial)
		{
			DynMaterial->SetVectorParameterValue("Color", Color);
			PlayerColor = Color;
		}
	}
}

void ATT_BasePlayer::LimitSpeed()
{
	FVector CurrentVelocity = MeshComponent->GetPhysicsLinearVelocity();

	if (CurrentVelocity.Size() > MaxSpeed)
	{
		FVector ClampedSpeed = CurrentVelocity.GetClampedToMaxSize(MaxSpeed);
		UE_LOG(LogTemp, Warning, TEXT("Clamped: %s"), *ClampedSpeed.ToString());
		MeshComponent->SetPhysicsLinearVelocity(ClampedSpeed);
	}
}

void ATT_BasePlayer::DrawArrowBasedOnSpeed()
{
	FVector Start = GetActorLocation();
	FVector End = Start + (MeshComponent->GetPhysicsLinearVelocity().GetSafeNormal() *
		MeshComponent->GetPhysicsLinearVelocity().Size());
	DrawDebugDirectionalArrow(GetWorld(), Start, End, 300.0f, FColor::Blue, false, -1.0f, 0, 4.0f);
}
