// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/TT_Target.h"
#include "TimerManager.h"

ATT_Target::ATT_Target()
{
	PrimaryActorTick.bCanEverTick = true;
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

void ATT_Target::OnTimerFired()
{
	FVector RandomImpulse = FVector(FMath::FRandRange(-1.0f, 1.0f), FMath::FRandRange(-1.0f, 1.0f), 0.0f).GetSafeNormal() * ImpulseStrength;
	RandomImpulse.Z = 0.0f;
	MeshComponent->AddImpulse(RandomImpulse);
}
