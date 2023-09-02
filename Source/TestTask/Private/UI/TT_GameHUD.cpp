// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TT_GameHUD.h"
#include "Blueprint/UserWidget.h"

void ATT_GameHUD::ShowGameOverWidget()
{
	PlayerHUDWidget->SetVisibility(ESlateVisibility::Hidden);
	GameOverWidget->SetVisibility(ESlateVisibility::Visible);
}

void ATT_GameHUD::BeginPlay()
{
	PlayerHUDWidget = CreateWidget<UUserWidget>(GetWorld(), PlayerHUDWidgetClass);
	PlayerHUDWidget->AddToViewport();

	GameOverWidget = CreateWidget<UUserWidget>(GetWorld(), GameOverWidgetClass);
	GameOverWidget->AddToViewport();
	GameOverWidget->SetVisibility(ESlateVisibility::Hidden);
}
