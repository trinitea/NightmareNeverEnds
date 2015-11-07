// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "GameFramework/Info.h"
#include "Prototype.h"
#include "BasePlayerController.h"
#include "BaseCharacter.h"
#include "Checkpoint.h"
#include "KidPlayerController.h"
#include "RubberManPlayerController.h"
#include "MainGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PROTOTYPE_API AMainGameMode : public AGameMode
{
	GENERATED_BODY()
	
	
public:

	AMainGameMode(const FObjectInitializer& ObjectInitializer);
	
	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<class ABasePlayerController> PlayerController1;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<class ABaseCharacter> PlayerCharacter1;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<class ABasePlayerController> PlayerController2;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<class ABaseCharacter> PlayerCharacter2;
	
	virtual APlayerController* SpawnPlayerController(FVector const& SpawnLocation, FRotator const& SpawnRotation) override;
	
	virtual UClass* GetDefaultPawnClassForController(AController* InController) override;
	
	bool SaveCheckPoint(ACheckpoint* checkpoint);

	virtual void ACharacterHasDied(AController* InController);

	virtual AActor* FindPlayerRestart();

	virtual void Respawn(ABasePlayerController* Player);

private:
	uint8 playerIndicator = 0;

	ACheckpoint* LastCheckpoint;
};
