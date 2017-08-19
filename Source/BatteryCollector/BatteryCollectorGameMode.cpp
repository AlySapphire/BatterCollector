// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "BatteryCollector.h"
#include "BatteryCollectorGameMode.h"
#include "BatteryCollectorCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "SpawnVolume.h"

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

void ABatteryCollectorGameMode::BeginPlay() {

	Super::BeginPlay();

	//Find all spawn volume actors
	TArray<AActor*> foundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), foundActors);

	for(auto actor : foundActors) {
		ASpawnVolume* spawnVolumeActor = Cast<ASpawnVolume>(actor);
		if(spawnVolumeActor) {
			m_spawnVolumeActors.AddUnique(spawnVolumeActor);
		}
	}

	SetCurrentState(eBatteryPlayState::ePlaying);

	//Set score to beat
	ABatteryCollectorCharacter* myCharacter = Cast<ABatteryCollectorCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	if(myCharacter) {
		powerToWin = myCharacter->GetInitialPower() * 1.25f;
	}

	if(HUDWidgetClass != NULL) {
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);
		if(CurrentWidget != nullptr)
			CurrentWidget->AddToViewport();
	}

}

void ABatteryCollectorGameMode::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	
	//Check we're using batter collector character
	ABatteryCollectorCharacter* myCharacter = Cast<ABatteryCollectorCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	if(myCharacter) {
		
		//If our power exceeds level needed to win then switch game state
		if(myCharacter->GetCurrentPower() > powerToWin) {
			SetCurrentState(eBatteryPlayState::eWon);
		}
		//otherwise if our power is positive
		else if(myCharacter->GetCurrentPower() > 0) {
			//Decrease power using decay rate
			myCharacter->UpdatePower(-DeltaTime * decayRate * (myCharacter->GetInitialPower()));
		} else {
			SetCurrentState(eBatteryPlayState::eGameOver);
		}

		
	}

}

float ABatteryCollectorGameMode::GetPowerToWin() const {
	return powerToWin;
}

eBatteryPlayState ABatteryCollectorGameMode::GetCurrentState() const {
	return m_currentState;
}

void ABatteryCollectorGameMode::SetCurrentState(eBatteryPlayState newState) {
	m_currentState = newState;
	HandleNewState(newState);
}

void ABatteryCollectorGameMode::HandleNewState(eBatteryPlayState newState) {

	switch(newState) {
		case eBatteryPlayState::ePlaying:
		{
			//Spawn volumes active
			for(auto volume : m_spawnVolumeActors) {
				volume->SetSpawningActive(true);
			}
		}
			break;
		case eBatteryPlayState::eWon: 
		{
			//Spawn volumes inactive
			for(auto volume : m_spawnVolumeActors) {
				volume->SetSpawningActive(false);
			}
		}			
			break;
		case eBatteryPlayState::eGameOver:
		{
			//Spawn volumes inactive
			for(auto volume : m_spawnVolumeActors) {
				volume->SetSpawningActive(false);
			}
			//block input
			APlayerController* player = UGameplayStatics::GetPlayerController(this, 0);
			if(player) {
				player->SetCinematicMode(true, false, false, true, true);
			}

			//ragdoll
			ACharacter* myCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
			if(myCharacter) {
				myCharacter->GetMesh()->SetSimulatePhysics(true);
				myCharacter->GetMovementComponent()->MovementState.bCanJump = false;
			}

		}			
			break;
		case eBatteryPlayState::eUnknown:
			break;
	}

}
