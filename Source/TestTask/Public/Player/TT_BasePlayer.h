// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TT_BasePlayer.generated.h"

class UFloatingPawnMovement;
class UCameraComponent;

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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UFloatingPawnMovement* FloatingMovementComponent;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

private:
	FLinearColor PlayerColor;

	void MoveForward(float Amount);
	void MoveRight(float Amount);

	void SetColor(const FLinearColor& Color);
};
