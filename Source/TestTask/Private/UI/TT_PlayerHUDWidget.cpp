// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TT_PlayerHUDWidget.h"
#include "TT_GameMode.h"

int32 UTT_PlayerHUDWidget::GetTargetsToMarkNum() const
{
	if (!GameMode)
		return int32();

	return GameMode->GetTargetsToMarkNum();
}

int32 UTT_PlayerHUDWidget::GetCleanersNum() const
{	
	if (!GameMode)
		return int32();

	return GameMode->GetCleanersNum();
}

void UTT_PlayerHUDWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (!GetWorld())
		return;

	GameMode = Cast<ATT_GameMode>(GetWorld()->GetAuthGameMode());
}
