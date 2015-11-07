// Fill out your copyright notice in the Description page of Project Settings.

#include "Prototype.h"
#include "BasePlayerController.h"
#include "Engine/NavigationObjectBase.h"
#include "GameFramework/PlayerStart.h"
#include "MainGameMode.h"

AMainGameMode::AMainGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PlayerController1 = APlayerController::StaticClass();
	PlayerCharacter1 = ABaseCharacter::StaticClass();

	PlayerController2 = APlayerController::StaticClass();
	PlayerCharacter2 = ABaseCharacter::StaticClass();
}

APlayerController* AMainGameMode::SpawnPlayerController(FVector const& SpawnLocation, FRotator const& SpawnRotation)
{
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Instigator = Instigator;
	
	ABasePlayerController* PlayerController;

	switch (++ playerIndicator) {
	case 1:
		PlayerController = GetWorld()->SpawnActor<ABasePlayerController>(PlayerController1, SpawnLocation, SpawnRotation, SpawnInfo);
		break;

	case 2:
		PlayerController = GetWorld()->SpawnActor<ABasePlayerController>(PlayerController2, SpawnLocation, SpawnRotation, SpawnInfo);
		break;

	default:
		PlayerController = GetWorld()->SpawnActor<ABasePlayerController>(PlayerControllerClass, SpawnLocation, SpawnRotation, SpawnInfo);
		break;
	}

	PlayerController->PlayerNumber = playerIndicator;

	return PlayerController;
}


UClass* AMainGameMode::GetDefaultPawnClassForController(AController* InController){
	
	UClass* PawnClass = NULL;

	ABasePlayerController* PlayerController = Cast<ABasePlayerController>(InController);

	if (PlayerController) {

		switch (PlayerController->PlayerNumber) {
		case 1:
			PawnClass = PlayerCharacter1;
			break;

		case 2:
			PawnClass = PlayerCharacter2;
			break;
		}
	}

	if (!PawnClass) {
		PawnClass = DefaultPawnClass;
	}

	return PawnClass;
}

bool AMainGameMode::SaveCheckPoint(ACheckpoint* checkpoint)
{
	if (!checkpoint->HasValidSpawnLocations()){
		return false;
	}

	PlayerStarts.Empty();

	AddPlayerStart(GetWorld()->SpawnActor<APlayerStart>(APlayerStart::StaticClass(), checkpoint->TargetOne->GetActorLocation(), checkpoint->TargetOne->GetActorRotation()));
	AddPlayerStart(GetWorld()->SpawnActor<APlayerStart>(APlayerStart::StaticClass(), checkpoint->TargetTwo->GetActorLocation(), checkpoint->TargetTwo->GetActorRotation()));

	UE_LOG(LogTemp, Warning, TEXT("CHECKPOINT here (*) <----"));

	UE_LOG(LogTemp, Warning, TEXT("start posi [1] : %s"), *checkpoint->TargetOne->GetActorLocation().ToCompactString());
	UE_LOG(LogTemp, Warning, TEXT("start posi [2] : %s"), *checkpoint->TargetTwo->GetActorLocation().ToCompactString());

	LastCheckpoint = checkpoint;

	return true;
}

void AMainGameMode::ACharacterHasDied(AController* InController)
{
	// Don't look at the bad coding ahead
	ABasePlayerController* amigo = Cast<ABasePlayerController>(InController);

	if(amigo) {
		if (amigo->PlayerNumber == 1) {

			for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
			{
				APlayerController* temp = Iterator->Get();

				// If cast fails, continue
				if (!dynamic_cast<ABasePlayerController*>(temp)) continue;

				Respawn((ABasePlayerController*) temp);
			}
		}
		else {
			Respawn(amigo);
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Is not a player controller so we ignore"));
	}
}

void AMainGameMode::Respawn(ABasePlayerController* Player){
	// If null, do nothing
	if (!Player) return;

	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Instigator = Instigator;

	APawn* newPlayerCharacter = SpawnDefaultPawnFor(Player, FindPlayerRestart());

	// If null, do nothing
	if (!newPlayerCharacter) return;

	// If has no pawn, do not destroy
	if (Player->GetPawn())
	{
		Player->GetPawn()->Destroy();
	}
	Player->Possess(newPlayerCharacter);
}

AActor* AMainGameMode::FindPlayerRestart(){

	if (PlayerStarts.Num() == 0) {
		return NULL;
	}

	AActor* start = PlayerStarts[(uint8)FMath::FRandRange(0, PlayerStarts.Num())];

	UE_LOG(LogTemp, Warning, TEXT("player start position %s"), *start->GetActorLocation().ToCompactString());

	return start;
}