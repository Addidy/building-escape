// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingEscape.h"
#include "OpenDoor.h"
#include "Grabber.h"
#define OUT


// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	Owner = GetOwner();

	if (!PressurePlate) {
		UE_LOG(LogTemp, Error, TEXT("Pressure Plate has not been assigned on %s"), *GetOwner()->GetName());
	}
}

void UOpenDoor::OpenDoor() {
	//Owner->SetActorRotation(FRotator(0.f, OpenAngle, 0.f));
	OnOpenRequest.Broadcast();
}

void UOpenDoor::CloseDoor() {
	Owner->SetActorRotation(FRotator(0.f, 0.f, 0.f));
}

// Called every frame
void UOpenDoor::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	if (!PressurePlate) { return; }

	//Poll the Trigger Volume
	//If the ActorThatOpens is in the volume
	if (GetTotalMassOfActorsOnPlate() > 20.f) { //TODO change to editable parameter
		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}

	//Check if it's time to close the door
	if (DoorCloseDelay <= GetWorld()->GetTimeSeconds() - LastDoorOpenTime) {
		CloseDoor();
	}

}

float UOpenDoor::GetTotalMassOfActorsOnPlate() {
	float TotalMass = 0.f;

	//Find all the overlapping actors
	TArray<AActor*> OverlappingActors;
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);
	
	//Iterate through them adding their masses
	for (const auto& Actor : OverlappingActors) {
		//UE_LOG(LogTemp, Warning, TEXT("%s on Pressure Plate"), *Actor->GetName());
		TotalMass += Actor->GetRootPrimitiveComponent()->GetMass();

	}
	//UE_LOG(LogTemp, Warning, TEXT("Total mass on plate: %f"), TotalMass);
	return TotalMass;
}

