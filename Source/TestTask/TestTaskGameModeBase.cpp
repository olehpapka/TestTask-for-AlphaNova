// Copyright Epic Games, Inc. All Rights Reserved.


#include "TestTaskGameModeBase.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy/TT_Target.h"

void ATestTaskGameModeBase::StartPlay()
{
	Super::StartPlay();
	SpawnTargets();
}

void ATestTaskGameModeBase::SpawnTargets()
{
	if (!GetWorld())
		return;

	TArray<AActor*> PlayerStarts;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), PlayerStarts);

	for (const auto Start : PlayerStarts)
	{
		UE_LOG(LogTemp, Error, TEXT("%s"), *Start->GetName());
	}

	FLinearColor TargetColor = FLinearColor::Yellow;
	FActorSpawnParameters SpawnInfo;

	int32 Multiplier = 0;
	for (int32 i = 0; i < TargetsNum; i++)
	{
		
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		FVector SpawnLocation = PlayerStarts[i % PlayerStarts.Num()]->GetActorLocation() 
			 + FVector(0.0f, 0.0f, 100.0f * Multiplier);
		FRotator SpawnRotation = PlayerStarts[i % PlayerStarts.Num()]->GetActorRotation();

		const auto Target = GetWorld()->SpawnActor<ATT_Target>(EnemyClass, SpawnLocation, SpawnRotation, SpawnInfo);
		if (Target)
		{
			Target->SetDefaultColor(TargetColor);
			Target->SetCleaner(false);
		}


		UE_LOG(LogTemp, Display, TEXT("Iteration: %i, Mult: %i"), i, Multiplier);
		if ((i + 1) % PlayerStarts.Num() == 0)
		{
			Multiplier++;
		}
	}

	FLinearColor CleanerColor = FLinearColor::Blue;

	for (int32 i = TargetsNum; i < TargetsNum + CleanersNum; i++)
	{
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		FVector SpawnLocation = PlayerStarts[i % PlayerStarts.Num()]->GetActorLocation()
			+ FVector(0.0f, 0.0f, 100.0f * Multiplier);
		FRotator SpawnRotation = PlayerStarts[i % PlayerStarts.Num()]->GetActorRotation();

		const auto Cleaner = GetWorld()->SpawnActor<ATT_Target>(EnemyClass, SpawnLocation, SpawnRotation, SpawnInfo);
		
		if (Cleaner)
		{
			Cleaner->SetDefaultColor(CleanerColor);
			Cleaner->SetCleaner(true);
		}

		UE_LOG(LogTemp, Display, TEXT("Iteration: %i, Mult: %i"), i, Multiplier);
		if ((i + 1) % PlayerStarts.Num() == 0)
		{
			Multiplier++;
		}
	}
}
