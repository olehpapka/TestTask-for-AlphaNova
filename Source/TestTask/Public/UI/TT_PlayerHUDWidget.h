// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TT_PlayerHUDWidget.generated.h"

class ATT_GameMode;

UCLASS()
class TESTTASK_API UTT_PlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "UI")
	int32 GetTargetsToMarkNum() const;

	UFUNCTION(BlueprintCallable, Category = "UI")
	int32 GetCleanersNum() const;

protected:
	virtual void NativeOnInitialized() override;

private:
	ATT_GameMode* GameMode;
};
