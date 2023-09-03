// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/TT_BasePlayer.h"
#include "Components/InputComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "TimerManager.h"
#include "Enemy/TT_Target.h"
#include "TT_GameMode.h"
#include "DrawDebugHelpers.h"

ATT_BasePlayer::ATT_BasePlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = true;

	//RootComponent = CreateDefaultSubobject<USceneComponent>("RootSceneComponent");

	//StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	//SetRootComponent(StaticMesh);

	//CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	//CameraComponent->SetupAttachment(RootComponent);

	////Need it?
	//AutoPossessPlayer = EAutoReceiveInput::Player0;	
}

void ATT_BasePlayer::BeginPlay()
{
	Super::BeginPlay();

	SetColor(FLinearColor::Red);

	StaticMesh = FindComponentByClass<UStaticMeshComponent>();
	CameraComponent = FindComponentByClass<UCameraComponent>();
}

void ATT_BasePlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UE_LOG(LogTemp, Display, TEXT("Velocity: %f"), StaticMesh->GetPhysicsLinearVelocity().Size());

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

	const auto GameMode = Cast<ATT_GameMode>(GetWorld()->GetAuthGameMode());
	if (!GameMode)
		return;

	if (!Target->IsMarked())
	{
		Target->SetNewColor(PlayerColor);
		GameMode->OnTargetMarkedDelegate.Broadcast(true);
		if (Target->IsCleaner())
		{
			GameMode->SetCleanersNum(GameMode->GetCleanersNum() - 1);
		}
	}
}

void ATT_BasePlayer::MoveForward(float Amount)
{
	if (Amount == 0.0f)
		return;

	FVector ForceToAdd = CameraComponent->GetForwardVector();
	ForceToAdd.X *= Amount * 500.0f;
	ForceToAdd.Y *= Amount * 500.0f;
	StaticMesh->AddForce(ForceToAdd, FName{}, true);

	LimitSpeed();
}

void ATT_BasePlayer::MoveRight(float Amount)
{
	if (Amount == 0.0f)
		return;

	FVector ForceToAdd = CameraComponent->GetRightVector();
	ForceToAdd.X *= Amount * 500.0f;
	ForceToAdd.Y *= Amount * 500.0f;
	StaticMesh->AddForce(ForceToAdd, FName{}, true);

	LimitSpeed();
}

void ATT_BasePlayer::SetColor(const FLinearColor& Color)
{
	const auto Mesh = FindComponentByClass<UStaticMeshComponent>();
	if (Mesh)
	{
		const auto DynMaterial = Mesh->CreateAndSetMaterialInstanceDynamic(0);
		if (DynMaterial)
		{
			DynMaterial->SetVectorParameterValue("Color", Color);
			PlayerColor = Color;
		}
	}
}

void ATT_BasePlayer::LimitSpeed()
{
	FVector CurrentVelocity = StaticMesh->GetPhysicsLinearVelocity();

	if (CurrentVelocity.Size() > MaxSpeed)
	{
		FVector ClampedSpeed = CurrentVelocity.GetClampedToMaxSize(MaxSpeed);
		UE_LOG(LogTemp, Warning, TEXT("Clamped: %s"), *ClampedSpeed.ToString());
		StaticMesh->SetPhysicsLinearVelocity(ClampedSpeed);
	}
}

void ATT_BasePlayer::DrawArrowBasedOnSpeed()
{
	FVector Start = GetActorLocation();
	FVector End = Start + (StaticMesh->GetPhysicsLinearVelocity().GetSafeNormal() *
		StaticMesh->GetPhysicsLinearVelocity().Size());
	DrawDebugDirectionalArrow(GetWorld(), Start, End, 300.0f, FColor::Blue, false, -1.0f, 0, 4.0f);
}
