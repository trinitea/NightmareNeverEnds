// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Spawner.generated.h"

/**
 * 
 */
UCLASS()
class PROTOTYPE_API ASpawner : public AActor
{
	GENERATED_BODY()
	
public:
	ASpawner(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<APawn> PawnToSpawn;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<AController> PawnController;
	
	UFUNCTION(BlueprintCallable, Category = "Spawning")
	APawn* CreatePawn();

	UFUNCTION(BlueprintCallable, Category = "Spawning")
	AController* CreateController();

	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void SpawnControlledCharacter();
};
