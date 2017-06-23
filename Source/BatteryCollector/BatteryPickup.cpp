// Fill out your copyright notice in the Description page of Project Settings.

#include "BatteryCollector.h"
#include "BatteryPickup.h"


ABatteryPickup::ABatteryPickup() {

	GetMesh()->SetSimulatePhysics(true);

	//Base power level of the battery
	batteryPower = 150.0f;

}

void ABatteryPickup::WasCollected_Implementation() {
	//Use the base pickup behaviour
	Super::WasCollected_Implementation();
	//Destroy the battery
	Destroy();
}

//report the power level of the battery
float ABatteryPickup::GetPower() {
	return batteryPower;
}
