// Fill out your copyright notice in the Description page of Project Settings.

#include "BatteryCollector.h"
#include "SpawnVolume.h"
#include "Kismet/KismetMathLibrary.h"
#include "Pickup.h"


// Sets default values
ASpawnVolume::ASpawnVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//Create default box component
	m_whereToSpawn = CreateDefaultSubobject<UBoxComponent>(TEXT("WhereToSpawn"));
	RootComponent = m_whereToSpawn;

	//Set spawn delay range
	spawnDelayMin = 1.0f;
	spawnDelayMax = 4.5f;

}

// Called when the game starts or when spawned
void ASpawnVolume::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpawnVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector ASpawnVolume::GetRandomPointInVolume() {

	FVector spawnOrigin = m_whereToSpawn->Bounds.Origin;
	FVector spawnExtents = m_whereToSpawn->Bounds.BoxExtent;

	return UKismetMathLibrary::RandomPointInBoundingBox(spawnOrigin, spawnExtents);

}

void ASpawnVolume::SetSpawningActive(bool bShouldSpawn) {

	if(bShouldSpawn) {
		//Set timer on spawn pickup
		m_spawnDelay = FMath::FRandRange(spawnDelayMin, spawnDelayMax);
		GetWorldTimerManager().SetTimer(spawnTimer, this, &ASpawnVolume::SpawnPickup, m_spawnDelay, false);
	} else {
		//Clear timer
		GetWorldTimerManager().ClearTimer(spawnTimer);
	}

}

void ASpawnVolume::SpawnPickup() {

	//If we have set something to spawn
	if(whatToSpawn != NULL) {
		//Check for valid world
		UWorld* const world = GetWorld();

		if(world) {

			//Set the spawn parameters
			FActorSpawnParameters spawnParams;
			spawnParams.Owner = this;
			spawnParams.Instigator = Instigator;

			//Get a random location to spawn at
			FVector spawnLocation = GetRandomPointInVolume();

			//Get a random rotation
			FRotator spawnRotation;
			spawnRotation.Yaw = FMath::FRand() * 360.0f;
			spawnRotation.Pitch = FMath::FRand() * 360.0f;
			spawnRotation.Roll = FMath::FRand() * 360.0f;

			//Spawn the pickup
			APickup* const spawnedPickup = world->SpawnActor<APickup>(whatToSpawn, spawnLocation, spawnRotation, spawnParams);

			m_spawnDelay = FMath::FRandRange(spawnDelayMin, spawnDelayMax);
			GetWorldTimerManager().SetTimer(spawnTimer, this, &ASpawnVolume::SpawnPickup, m_spawnDelay, false);

		}

	}

}

