// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	CurrentYaw = InitialYaw;
	TargetYaw = InitialYaw + TargetYaw;

	if(!PressurePlate)
	{
		UE_LOG(LogTemp,Error, TEXT("%s has the OpenDoor component on it, but no pressureplate set"), *GetOwner()->GetName());
	}

	ActorThatOpen = GetWorld()->GetFirstPlayerController()->GetPawn();
	
/*
	float MyFloat = 90.0f;
	FRotator CurrentRotation = GetOwner()->GetActorRotation();
	CurrentRotation.Yaw = CurrentRotation.Yaw + 90;

	GetOwner()->SetActorRotation(CurrentRotation);
*/
}

void UOpenDoor::OpenDoor(float DeltaTime){
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	CurrentYaw = FMath::FInterpTo(CurrentYaw, TargetYaw,DeltaTime, DoorOpenSpeed);
	DoorRotation.Yaw=CurrentYaw;
	GetOwner()->SetActorRotation(DoorRotation);
}

void UOpenDoor::CloseDoor(float DeltaTime){
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	CurrentYaw = FMath::FInterpTo(CurrentYaw, InitialYaw,DeltaTime, DoorCloseSpeed);
	DoorRotation.Yaw=CurrentYaw;
	GetOwner()->SetActorRotation(DoorRotation);
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (PressurePlate->IsOverlappingActor(ActorThatOpen))
	{
		OpenDoor(DeltaTime);
		DoorLastOpened = GetWorld()->GetTimeSeconds();
	} 
	else
	{
		DoorClosedDelay= GetWorld()->GetTimeSeconds() - DoorLastOpened;
		if(DoorClosedDelay >=.4f)
		{
			CloseDoor(DeltaTime);
		}
	}
	//UE_LOG(LogTemp,Warning, TEXT("%s"), *GetOwner()->GetActorRotation().ToString());
	//UE_LOG(LogTemp,Warning, TEXT("Yaw is: %f"), GetOwner()->GetActorRotation().Yaw);
	//OpenDoor.Yaw =FMath::FInterpConstantTo(GetOwner()->GetActorRotation().Yaw, TargetYaw, DeltaTime, 45);
	//OpenDoor.Yaw =FMath::Lerp(GetOwner()->GetActorRotation().Yaw, TargetYaw,DeltaTime *1.f);
}

