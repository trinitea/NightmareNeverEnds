// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Checkpoint.generated.h"

/**
 * 
 */
UCLASS()
class PROTOTYPE_API ACheckpoint : public AActor
{
	GENERATED_BODY()
	
public:
	ACheckpoint(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintReadOnly, meta = (UIMin = "0.0"))
	float radius = 1000.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* CollisionVolume;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Checkpoint")
	UStaticMeshComponent* CheckpointMesh;

	UPROPERTY(BlueprintReadOnly)
	bool IsActivated = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Checkpoint")
	AActor* TargetOne;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Checkpoint")
	AActor* TargetTwo;

	void BeginPlay() override;

	UFUNCTION(BlueprintNativeEvent, Category="Checkpoint")
	void OnOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable, Category = "Validate")
	bool HasValidSpawnLocations();

private:
	UPROPERTY()
	bool IsValidCheckpoint = false;
};
