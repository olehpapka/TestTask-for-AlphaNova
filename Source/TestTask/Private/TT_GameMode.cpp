// Copyright Epic Games, Inc. All Rights Reserved.


#include "TT_GameMode.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy/TT_Target.h"
#include "UI/TT_GameHUD.h"

ATT_GameMode::ATT_GameMode()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ATT_GameMode::StartPlay()
{
	Super::StartPlay();
	SpawnTargets();

	OnTargetMarkedDelegate.AddUObject(this, &ATT_GameMode::OnTargetMarked);
}

void ATT_GameMode::SetCleanersNum(int32 Num)
{
	CleanersNum = Num;
}

void ATT_GameMode::Tick(float DeltaTime)
{
	UE_LOG(LogTemp, Warning, TEXT("Cleaners num: %i"), CleanersNum);
}

void ATT_GameMode::SpawnTargets()
{
	if (!GetWorld())
		return;

	TArray<AActor*> PlayerStarts;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), PlayerStarts);

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
			TargersToMark++;
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
			TargersToMark++;
		}

		UE_LOG(LogTemp, Display, TEXT("Iteration: %i, Mult: %i"), i, Multiplier);
		if ((i + 1) % PlayerStarts.Num() == 0)
		{
			Multiplier++;
		}
	}
}

void ATT_GameMode::OnTargetMarked(bool Marked)
{
	if (Marked)
	{
		TargersToMark--;
	}
	else
	{
		TargersToMark++;
	}

	UE_LOG(LogTemp, Display, TEXT("Targets to mark: %i"), TargersToMark);

	if (TargersToMark == 0)
	{
		if (!GetWorld())
			return;

		const auto HUD = Cast<ATT_GameHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
		if (!HUD)
			return;

		HUD->ShowGameOverWidget();

		UE_LOG(LogTemp, Error, TEXT("GAME OVER"));
	}
}
