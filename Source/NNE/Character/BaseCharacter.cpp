
#include "NNE.h"
#include "Net/UnrealNetwork.h"
#include "BaseCharacter.h"

ABaseCharacter::ABaseCharacter() : Super()
{
	bReplicates = true;
}

void ABaseCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABaseCharacter, HealthCurrent);
	DOREPLIFETIME(ABaseCharacter, HealthMax);
}

void ABaseCharacter::Die(){

	if (Controller && Role == ROLE_Authority) {
		/*
		AMainGameMode* mainGM = dynamic_cast<AMainGameMode*>(GetWorld()->GetAuthGameMode());

		if (mainGM)
		{
			mainGM->ACharacterHasDied(Controller);
		}
		*/
	}

	//Destroy();
}

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


bool ABaseCharacter::UseMain_Validate(){
	return true; // check if you have main weapon
}

void ABaseCharacter::UseMain_Implementation(){
	return;
}

bool ABaseCharacter::UseSecondary_Validate(){
	return true; // check if you have 2 weapons or 1 weapon with secondary fire
}

void ABaseCharacter::UseSecondary_Implementation(){
	return;
}

bool ABaseCharacter::UseAction_Validate(){
	return true;
}

void ABaseCharacter::UseAction_Implementation(){
	return;
}