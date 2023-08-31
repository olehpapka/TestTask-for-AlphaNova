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

	void SetDefaultColor(const FLinearColor& NewColor);
	void SetNewColor(const FLinearColor& NewColor);
	const FLinearColor& GetDefaultColor() const { return DefaultColor; }

	void SetCleaner(bool Cleaner) { bIsCleaner = Cleaner; }
	bool IsCleaner() const { return bIsCleaner; }

	bool IsMarked() const { return bIsMarked; }

protected:
	UPROPERTY(EditAnywhere, Category = "Movement")
		float UpdateTime;

	UPROPERTY(EditAnywhere, Category = "Movement")
		float ImpulseStrength;


	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
private:
	UStaticMeshComponent* MeshComponent;
	FTimerHandle MoveTimerHandle;

	bool bIsCleaner = false;
	bool bIsMarked = false;

	FLinearColor DefaultColor;
	FLinearColor CurrentColor;

	void OnTimerFired();

	void SetMaterialColor();
};
