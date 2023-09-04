// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TT_Target.generated.h"

class UNiagaraSystem;
class UBoxComponent;

UCLASS()
class TESTTASK_API ATT_Target : public AActor
{
	GENERATED_BODY()

public:
	ATT_Target();

	void SetDefaultColor(const FLinearColor& NewColor);
	void SetNewColor(const FLinearColor& NewColor);

	void SetCleaner(bool Cleaner) { bIsCleaner = Cleaner; }
	bool IsCleaner() const { return bIsCleaner; }
	bool IsMarked() const { return bIsMarked; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UBoxComponent* BoxComponent;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float UpdateTime = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float ImpulseStrength = 60000.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
	UNiagaraSystem* CollisionWithTargetEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
	UNiagaraSystem* TargetCollisionWithCleanerEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
	UNiagaraSystem* CleanerCollisionWithCleanerEffect;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

private:
	FTimerHandle MoveTimerHandle;

	bool bIsCleaner = false;
	bool bIsMarked = false;

	FLinearColor DefaultColor;
	FLinearColor CurrentColor;

	void OnTimerFired();

	void SetMaterialColor();
};
