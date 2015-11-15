
#include "NNE.h"
#include "NNEProjectile.h"
#include "Net/UnrealNetwork.h"
#include "BaseCharacter.h"

ABaseCharacter::ABaseCharacter() : Super()
{
	bReplicates = true;
	GetCharacterMovement()->bOrientRotationToMovement = false; // orientation is binded to the thumbstick rotation.

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ShipMesh(TEXT("/Game/TwinStick/Meshes/TwinStickUFO.TwinStickUFO"));
	// Create the mesh component
	ShipMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));
	RootComponent = ShipMeshComponent;

	ShipMeshComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	ShipMeshComponent->SetStaticMesh(ShipMesh.Object);

	static ConstructorHelpers::FObjectFinder<UBlueprint> BulletBlueprint(TEXT("Blueprint'/Game/Blueprints/NNEProjectileBP.NNEProjectileBP'"));
	if (BulletBlueprint.Object){
		ProjectileBP = (UClass*)BulletBlueprint.Object->GeneratedClass;
	} else{
		UE_LOG(LogTemp, Error, TEXT("could not find garbage BP"));
	}

	GunOffset = FVector(90.f, 0.f, 0.f);
	FireRate = 0.1f;
	bCanFire = true;
}

/*
void ABaseCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABaseCharacter, HealthCurrent);
	DOREPLIFETIME(ABaseCharacter, HealthMax);
}
*/
/*
float ABaseCharacter::GetCurrentHealth(){
	return HealthCurrent;
}
 
//Server validates health can be added, up to max defined by Health
//Then broadcasts the current health to all clients with ClientUpdateCurrentHealth()

bool ABaseCharacter::Heal_Validate(float HealAmount){

	return (Controller && Role == ROLE_Authority
		&& 0 <= HealAmount && HealthCurrent < HealthMax);
}

void ABaseCharacter::Heal_Implementation(float HealAmount){

	HealthCurrent += HealAmount;

	if (HealthCurrent > HealthMax) {
		HealthCurrent = HealthMax;
	}
}
*/

bool ABaseCharacter::UseMain_Validate(){
	return bCanFire; // check if you have main weapon
}

void ABaseCharacter::UseMain_Implementation(){

	// If we it's ok to fire again
	if (bCanFire == true)
	{
		const FRotator FireRotation = FRotator(0.f, 0.f, 0.f);// RootComponent->Rotation();
		// Spawn projectile at an offset from this pawn
		const FVector SpawnLocation = GetActorLocation() + FireRotation.RotateVector(GunOffset);

		UWorld* const World = GetWorld();
		if (World != NULL)
		{
			// spawn the projectile
			World->SpawnActor<ANNEProjectile>(ProjectileBP, SpawnLocation, FireRotation);
		}

		bCanFire = false;
		
		World->GetTimerManager().SetTimer(TimerHandle_ShotTimerExpired, this, &ABaseCharacter::ShotTimerExpired, FireRate); // Cool shit !!

		/*
		// try and play the sound if specified
		if (FireSound != nullptr)
		{
			UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
		}

		bCanFire = false;
		*/
	}



	return;
}

void ABaseCharacter::ShotTimerExpired()
{
	bCanFire = true;
}

bool ABaseCharacter::UseSecondary_Validate(){
	return false; // check if you have 2 weapons or 1 weapon with secondary fire
}

void ABaseCharacter::UseSecondary_Implementation(){
	return;
}
/*
bool ABaseCharacter::UseAction_Validate(){
	return true;
}

void ABaseCharacter::UseAction_Implementation(){
	return;
}
*/
//==============================================================================
// SHOULD BE CORRECTED 
//==============================================================================

bool ABaseCharacter::Move_Validate(float ForwardValue, float  RightValue, float DeltaSeconds, FRotator DirectionPerspective){
	return Role == ROLE_Authority;
}

void ABaseCharacter::Move_Implementation(float ForwardValue, float  RightValue, float DeltaSeconds, FRotator DirectionPerspective){

	// Clamp max size so that (X=1, Y=1) doesn't cause faster movement in diagonal directions
	const FVector MoveDirection = FVector(ForwardValue, RightValue, 0.f).GetClampedToMaxSize(1.0f);

	// Calculate  movement
	const FVector Movement = MoveDirection * MoveSpeed * DeltaSeconds;

	// If non-zero size, move this actor
	if (Movement.SizeSquared() > 0.0f)
	{
		//const FRotator NewRotation = Movement.Rotation();
		FHitResult Hit(1.f);
		RootComponent->MoveComponent(Movement, DirectionPerspective, true, &Hit);
	}
	
}
bool ABaseCharacter::Aim_Validate(float ActionForwardValue, float ActionRightValue, const FRotator DirectionPerspective){
	return Role == ROLE_Authority;
}

void ABaseCharacter::Aim_Implementation(float ActionForwardValue, float ActionRightValue, const FRotator DirectionPerspective){
	
	float treshold = 0.1f;

	// early return
	if (ActionForwardValue < treshold && ActionForwardValue > -treshold && 
		ActionRightValue < treshold && ActionRightValue > -treshold) return;

	FRotator Rot = FRotationMatrix::MakeFromX(FVector(ActionForwardValue, ActionRightValue, 0)).Rotator();
	RootComponent->SetRelativeRotation(Rot);
	
}
