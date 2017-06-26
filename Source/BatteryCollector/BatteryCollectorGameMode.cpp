// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "BatteryCollector.h"
#include "BatteryCollectorGameMode.h"
#include "BatteryCollectorCharacter.h"
#include "Kismet/GameplayStatics.h"

ABatteryCollectorGameMode::ABatteryCollectorGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
		PrimaryActorTick.bCanEverTick = true;
	}

	//Base decay rate
	decayRate = 0.01f;

}

void ABatteryCollectorGameMode::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	
	//Check we're using batter collector character
	ABatteryCollectorCharacter* myCharacter = Cast<ABatteryCollectorCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	if(myCharacter) {

		if(myCharacter->GetCurrentPower() > 0) {
			//Decrease power using decay rate
			myCharacter->UpdatePower(-DeltaTime * decayRate * (myCharacter->GetInitialPower()));
		}

		
	}

}
