// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/TT_Target.h"
#include "TimerManager.h"

ATT_Target::ATT_Target()
{
	PrimaryActorTick.bCanEverTick = true;
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
	
	MeshComponent = FindComponentByClass<UStaticMeshComponent>();
	if (!MeshComponent)
		return;

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
	}
	else if (bIsCleaner && !bIsMarked && Target->bIsMarked)
	{
		Target->SetDefaultColor(Target->DefaultColor);
	}
}

void ATT_Target::OnTimerFired()
{
	FVector RandomImpulse = FVector(FMath::FRandRange(-1.0f, 1.0f), FMath::FRandRange(-1.0f, 1.0f), 0.0f).GetSafeNormal() * ImpulseStrength;
	RandomImpulse.Z = 0.0f;
	MeshComponent->AddImpulse(RandomImpulse);
}

void ATT_Target::SetMaterialColor()
{
	const auto Mesh = FindComponentByClass<UStaticMeshComponent>();
	if (Mesh)
	{
		const auto DynMaterial = Mesh->CreateAndSetMaterialInstanceDynamic(0);
		if (DynMaterial)
		{
			DynMaterial->SetVectorParameterValue("Color", CurrentColor);
		}
	}
}
