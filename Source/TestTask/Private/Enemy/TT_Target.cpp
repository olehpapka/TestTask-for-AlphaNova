// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/TT_Target.h"
#include "TimerManager.h"
#include "TT_GameMode.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/BoxComponent.h"

ATT_Target::ATT_Target()
{
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	MeshComponent->SetCenterOfMass(FVector{ 20.0f, 0.0f, 125.0f });
	SetRootComponent(MeshComponent);

	BoxComponent = CreateDefaultSubobject<UBoxComponent>("BoxComponent");
	BoxComponent->SetupAttachment(MeshComponent);
}

void ATT_Target::SetDefaultColor(const FLinearColor& NewColor)
{
	DefaultColor = CurrentColor = NewColor;
	bIsMarked = false;
	SetMaterialColor();
}

void ATT_Target::SetNewColor(const FLinearColor& NewColor)
{
	CurrentColor = NewColor;
	bIsMarked = true;
	SetMaterialColor();
}

void ATT_Target::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorldTimerManager().SetTimer(MoveTimerHandle, this, &ATT_Target::OnTimerFired, UpdateTime, true);
}

void ATT_Target::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATT_Target::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	const auto Target = Cast<ATT_Target>(OtherActor);
	if (!Target)
		return;

	if (!GetWorld())
		return;

	const auto GameMode = Cast<ATT_GameMode>(GetWorld()->GetAuthGameMode());
	if (!GameMode)
		return;

	//Marked cleaner can "clean" other targers and cleaners and doesn't mark other targets
	/*if (bIsMarked && !bIsCleaner && !Target->bIsMarked && !Target->bIsCleaner)
	{
		Target->SetNewColor(CurrentColor);
	}
	else if (bIsCleaner && Target->bIsMarked)
	{
		Target->SetDefaultColor(Target->DefaultColor);
	}*/

	//Marked cleaner can't clean anymore, it markes other targets instead
	if (bIsMarked && !Target->bIsMarked && !Target->bIsCleaner)
	{
		Target->SetNewColor(CurrentColor);
		GameMode->OnTargetMarkedDelegate.Broadcast(true);
		if (Target->bIsCleaner)
		{
			GameMode->SetCleanersNum(GameMode->GetCleanersNum() - 1);
		}

		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), CollisionWithTargetEffect, Target->GetActorLocation(),
			FRotator::ZeroRotator, FVector{ 0.2f, 0.2f, 0.2f });
	}
	else if (bIsCleaner && !bIsMarked && Target->bIsMarked)
	{
		Target->SetDefaultColor(Target->DefaultColor);
		GameMode->OnTargetMarkedDelegate.Broadcast(false);
		if (Target->bIsCleaner)
		{
			GameMode->SetCleanersNum(GameMode->GetCleanersNum() + 1);
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), CleanerCollisionWithCleanerEffect, Target->GetActorLocation(),
				FRotator::ZeroRotator, FVector{ 0.2f, 0.2f, 0.2f });
		}
		else
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), TargetCollisionWithCleanerEffect, Target->GetActorLocation(),
				FRotator::ZeroRotator, FVector{ 0.2f, 0.2f, 0.2f });
		}
	}
}

void ATT_Target::OnTimerFired()
{
	FVector RandomImpulse = FVector(FMath::FRandRange(-1.0f, 1.0f), FMath::FRandRange(-1.0f, 1.0f), 0.0f).GetSafeNormal() * ImpulseStrength;
	MeshComponent->AddImpulse(RandomImpulse);
}

void ATT_Target::SetMaterialColor()
{
	if (MeshComponent)
	{
		const auto DynMaterial = MeshComponent->CreateAndSetMaterialInstanceDynamic(0);
		if (DynMaterial)
		{
			DynMaterial->SetVectorParameterValue("Color", CurrentColor);
		}
	}
}
