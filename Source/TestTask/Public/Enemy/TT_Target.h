// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TT_Target.generated.h"

UCLASS()
class TESTTASK_API ATT_Target : public AActor
{
	GENERATED_BODY()

public:
	ATT_Target();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float Speed = 300.0f;

	UPROPERTY(EditAnywhere, Category = "Movement")
		float UpdateTime;

	UPROPERTY(EditAnywhere, Category = "Movement")
		float ImpulseStrength;

private:
	UStaticMeshComponent* MeshComponent;
	FTimerHandle MoveTimerHandle;
	void OnTimerFired();
};
