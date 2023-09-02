// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TT_GameHUD.generated.h"

UCLASS()
class TESTTASK_API ATT_GameHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	void ShowGameOverWidget();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> PlayerHUDWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> GameOverWidgetClass;

	virtual void BeginPlay() override;

private:
	UUserWidget* PlayerHUDWidget;
	UUserWidget* GameOverWidget;
};
