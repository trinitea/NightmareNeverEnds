#include "Prototype.h"
#include "MainGameMode.h"
#include "BaseCharacter.h"
#include "PatrollingEnemyCharacter.h"

/*
 *	Default constructor, code is from the unreal template for now.
 */
ABaseCharacter::ABaseCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	//START TEMPLATE CODE

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;
	//END TEMPLATE CODE
}

void ABaseCharacter::Die()
{	
	if (Controller && Role == ROLE_Authority) {

		AMainGameMode* mainGM = dynamic_cast<AMainGameMode*>(GetWorld()->GetAuthGameMode());

		if (mainGM)
		{
			mainGM->ACharacterHasDied(Controller);
		}
	}

	//Destroy();
}


//Character has been hit by enemy 
void ABaseCharacter::OnHit_Implementation(AActor *Other)
{
	//server event 
	if (Controller && Role == ROLE_Authority) {
		
		//apply damage received from the other Character.  Ignores hitting other basecharacter (Kid/RubberMan)
		if (dynamic_cast<APatrollingEnemyCharacter*>(Other)){
			ApplyDamage(Cast<APatrollingEnemyCharacter>(Other)->GetAttackDamage());

		}
	}
}



//Apply reveived damage on the character 
void ABaseCharacter::ApplyDamage_Implementation(int32 dmg){

	Health = Health - dmg;

	//Launch blueprint event to notify the HUD about the change
	OnHealthChanged(Health);

	if (Health <= 0){
		Die();
	}
}

int32 ABaseCharacter::GetCurrentHealth(){
	return Health;
}



int32 ABaseCharacter::GetAttackDamage(){
	return Damage;
}


//NOTE: Should be server side only

void ABaseCharacter::CheckAttackOverlap(){

	//Overlapping actors for each box spawned will be stored here
	TArray<struct FOverlapResult> OutActorOverlaps;

	//Hit other actors only once
	TArray<AActor*> ProcessedActors;


	//The initial rotation of our box is the same as our character rotation
	FQuat Rotation = GetTransform().GetRotation();
	FVector Start = GetTransform().GetLocation() + Rotation.Rotator().Vector() * 100.0f;

	FCollisionShape CollisionHitShape;
	FCollisionQueryParams CollisionParams;

	//We do not want the character to hit itself, don't store this character in the array, to ignore it's collision
	CollisionParams.AddIgnoredActor(this);

	//Set the channels that will respond to the collision
	FCollisionObjectQueryParams CollisionObjectTypes;
	CollisionObjectTypes.AddObjectTypesToQuery(ECollisionChannel::ECC_PhysicsBody);
	CollisionObjectTypes.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);
	//CollisionObjectTypes.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic); // uncomment to enable bashing objects

	//Create the box and get the overlapping actors
	CollisionHitShape = FCollisionShape::MakeBox(AttackBox);
	GetWorld()->OverlapMulti(OutActorOverlaps, Start, Rotation, CollisionHitShape, CollisionParams, CollisionObjectTypes);


	AActor* ActorToProcess;
	//Process all hit actors
	for (int i = 0; i < OutActorOverlaps.Num(); ++i)
	{
		ActorToProcess = OutActorOverlaps[i].GetActor();
		//We process each actor only once per Attack execution
		if (ActorToProcess && !ProcessedActors.Contains(ActorToProcess))
		{

			//Add this actor to the array because we are spawning one box per tick and we don't want to hit the same actor twice during the same attack animation
			ProcessedActors.AddUnique(ActorToProcess);

			if ( dynamic_cast<APatrollingEnemyCharacter*>(ActorToProcess) ){
				APatrollingEnemyCharacter* ennemy = (APatrollingEnemyCharacter*)ActorToProcess;
				ennemy->OnHit(this);
			}
		}
	}
}




//allows health to be added from BP events
void  ABaseCharacter::AddHealth(int32 HealthToAdd){

		ApplyHealth(HealthToAdd);
}


//Server validates health can be added, up to max defined by Health
//Then broadcasts the current health to all clients with ClientUpdateCurrentHealth()

bool ABaseCharacter::ApplyHealth_Validate(int32 HealthToAdd){

	return ( Controller 
		  && Role == ROLE_Authority 
		  && 0 <= HealthToAdd && HealthToAdd <= MaxHealth );
}

void ABaseCharacter::ApplyHealth_Implementation(int32 HealthToAdd){
	Health += HealthToAdd;
	if (Health > MaxHealth) {
		Health = MaxHealth;
	}
	ClientUpdateCurrentHealth(Health);
}


void  ABaseCharacter::ClientUpdateCurrentHealth_Implementation(int32 NewHealthValue){
	Health = NewHealthValue;

	//Launch blueprint event to notify the HUD about the change
	OnHealthChanged(Health);
}