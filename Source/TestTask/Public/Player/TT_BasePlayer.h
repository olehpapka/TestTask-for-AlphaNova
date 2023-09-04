// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TT_BasePlayer.generated.h"

class UFloatingPawnMovement;
class UCameraComponent;
class USoundCue;
class UNiagaraSystem;

UCLASS()
class TESTTASK_API ATT_BasePlayer : public APawn
{
	GENERATED_BODY()

public:
	ATT_BasePlayer();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float MaxSpeed = 750.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
	USoundCue* CollisionSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
	UNiagaraSystem* CollisionEffect;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

private:
	FLinearColor PlayerColor;

	void MoveForward(float Amount);
	void MoveRight(float Amount);

	void SetColor(const FLinearColor& Color);

	void LimitSpeed();
	void DrawArrowBasedOnSpeed();
};
