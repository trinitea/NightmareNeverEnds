// Fill out your copyright notice in the Description page of Project Settings.

#include "Prototype.h"
#include "Spawner.h"


ASpawner::ASpawner(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

APawn* ASpawner::CreatePawn(){

	if (PawnToSpawn != NULL){

		UWorld* const world = GetWorld();
		
		if (world){
			return world->SpawnActor<APawn>(PawnToSpawn, GetActorLocation(), GetActorRotation());
		}
		
	}

	return NULL;
}

AController* ASpawner::CreateController(){

	if (PawnController != NULL){

		UWorld* const world = GetWorld();
		
		if (world){
			return world->SpawnActor<AController>(PawnController, GetActorLocation(), GetActorRotation());
		}
		
	}

	return NULL;
}
void ASpawner::SpawnControlledCharacter(){

	if (Role == ROLE_Authority){

		APawn* pawn = CreatePawn();
		AController* controller = CreateController();

		if (pawn && controller) {
			controller->Possess(pawn);
		}
		else
		{
			// something went wrong, need to rollback actor creation
			if (pawn) pawn->Destroy();
			if (controller) controller->Destroy();
		}
	}
}