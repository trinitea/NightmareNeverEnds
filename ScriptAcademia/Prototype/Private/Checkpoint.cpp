// Fill out your copyright notice in the Description page of Project Settings.

#include "Prototype.h"
#include "BaseCharacter.h"
#include "BasePlayerController.h"
#include "MainGameMode.h"
#include "Checkpoint.h"
 

//Creates a sphere volume trigger instance, and manages the trigger with OnOverlapBegin()
ACheckpoint::ACheckpoint(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	RootComponent = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, "Base");

	CheckpointMesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("Mesh"));
	CheckpointMesh->SetSimulatePhysics(false);
	CheckpointMesh->AttachTo(RootComponent);
	
	CollisionVolume = ObjectInitializer.CreateDefaultSubobject<UBoxComponent>(this, "CollisionBox");
	CollisionVolume->AttachTo(RootComponent);
	CollisionVolume->OnComponentBeginOverlap.AddDynamic(this, &ACheckpoint::OnOverlapBegin);
}

void ACheckpoint::BeginPlay()
{
	Super::BeginPlay();

	if (Role < ROLE_Authority) {
		return;
	}

	IsValidCheckpoint = (TargetOne && TargetTwo);
	IsActivated = false;
}

bool ACheckpoint::HasValidSpawnLocations()
{
	return IsValidCheckpoint;
}

void ACheckpoint::OnOverlapBegin_Implementation(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Role < ROLE_Authority || IsActivated) {
		return;
	}

	ABaseCharacter* character = Cast<ABaseCharacter>(OtherActor);
	
	if (character)
	{
		if (Cast<ABasePlayerController>(character->GetController())){

			AMainGameMode* mainGM = dynamic_cast<AMainGameMode*>(GetWorld()->GetAuthGameMode());

			if (mainGM)
			{
				if (mainGM->SaveCheckPoint(this)){
					IsActivated = true;
				}
				
			}
		}
	}
}