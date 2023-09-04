// Copyright Epic Games, Inc. All Rights Reserved.


#include "TT_GameMode.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy/TT_Target.h"
#include "UI/TT_GameHUD.h"
#include "Player/TT_BasePlayer.h"
#include "Player/TT_PlayerController.h"

ATT_GameMode::ATT_GameMode()
{
	DefaultPawnClass = ATT_BasePlayer::StaticClass();
	PlayerControllerClass = ATT_PlayerController::StaticClass();
	HUDClass = ATT_GameHUD::StaticClass();
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

	if (TargersToMark == 0)
	{
		if (!GetWorld())
			return;

		const auto HUD = Cast<ATT_GameHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
		if (!HUD)
			return;

		HUD->ShowGameOverWidget();

		const auto PlayerController = GetWorld()->GetFirstPlayerController();
		if (!PlayerController)
			return;

		PlayerController->GetPawn()->DisableInput(PlayerController);

		UE_LOG(LogTemp, Error, TEXT("GAME OVER"));
	}
}
