// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/GameModeBase.h"
#include "BatteryCollectorGameMode.generated.h"

//Enum to store gameplay state
UENUM(BlueprintType)
enum class eBatteryPlayState : uint8 {
	ePlaying,
	eGameOver,
	eWon,
	eUnknown
};

UCLASS(minimalapi)
class ABatteryCollectorGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ABatteryCollectorGameMode();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	//Returns power needed to win - Needed for HUD
	UFUNCTION(BlueprintPure, Category = "Power")
	float GetPowerToWin() const;

	//Returns current play state
	UFUNCTION(BlueprintPure, Category = "Power")
	eBatteryPlayState GetCurrentState() const;

	//Set new play state
	void SetCurrentState(eBatteryPlayState newState);

protected:
	//Rate that player loses power
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Power", meta = (BlueprintProtected = "true"))
	float decayRate;

	//Power needed to win the game
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Power", meta = (BlueprintProtected = "true"))
	float powerToWin;

	//The widget class to use for our HUD
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Power", meta = (BlueprintProtected = "true"))
	TSubclassOf<class UUserWidget> HUDWidgetClass;

	//HUD instance
	UPROPERTY()
	class UUserWidget* CurrentWidget;

private:

	//Keeps track of the current play state
	eBatteryPlayState m_currentState;

	TArray<class ASpawnVolume*> m_spawnVolumeActors;

	//Handle any function calls that rely upon game state changes
	void HandleNewState(eBatteryPlayState newState);

};



