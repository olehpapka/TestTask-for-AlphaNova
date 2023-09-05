// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TT_GameMode.generated.h"

class ATT_Target;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnTargetMarkedSignature, bool);

UCLASS()
class TESTTASK_API ATT_GameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ATT_GameMode();

	FOnTargetMarkedSignature OnTargetMarkedDelegate;

	virtual void StartPlay() override;

	int32 GetTargetsToMarkNum() const { return TargersToMark; }

	int32 GetCleanersNum() const { return CleanersNum; }
	void SetCleanersNum(int32 Num);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Game")
	TSubclassOf<ATT_Target> EnemyClass;

	UPROPERTY(EditDefaultsOnly, Category = "Game", meta = (ClampMin = "0", ClampMax = "50"))
	int32 TargetsNum = 10;

	UPROPERTY(EditDefaultsOnly, Category = "Game", meta = (ClampMin = "0", ClampMax = "15"))
	int32 CleanersNum = 2;

private:
	void SpawnTargets();

	int32 TargersToMark = 0;

	void OnTargetMarked(bool Marked);
};
