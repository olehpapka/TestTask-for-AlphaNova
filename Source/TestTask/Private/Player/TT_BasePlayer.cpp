// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/TT_BasePlayer.h"
#include "Components/InputComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "TimerManager.h"
#include "Enemy/TT_Target.h"
#include "TT_GameMode.h"

ATT_BasePlayer::ATT_BasePlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = true;

	//RootComponent = CreateDefaultSubobject<USceneComponent>("RootSceneComponent");

	//StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	//SetRootComponent(StaticMesh);

	//CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	//CameraComponent->SetupAttachment(RootComponent);

	//FloatingMovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>("FloatingMovementComponent");
	//


	////Need it?
	//AutoPossessPlayer = EAutoReceiveInput::Player0;

	
}

void ATT_BasePlayer::BeginPlay()
{
	Super::BeginPlay();

	SetColor(FLinearColor::Red);

	//GetWorldTimerManager().SetTimer(ColorTimerHandle, this, &ATT_BasePlayer::OnTimerFired, 7.0f, true);
}

void ATT_BasePlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//UE_LOG(LogTemp, Display, TEXT("%s"), *GetActorForwardVector().ToString());
	//UE_LOG(LogTemp, Display, TEXT("Velocity: %f"), GetVelocity().Size());
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
	AddMovementInput(GetActorForwardVector(), Amount);
}

void ATT_BasePlayer::MoveRight(float Amount)
{
	AddMovementInput(GetActorRightVector(), Amount);
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

void ATT_BasePlayer::OnTimerFired()
{
	SetColor(FLinearColor::MakeRandomColor());
}

