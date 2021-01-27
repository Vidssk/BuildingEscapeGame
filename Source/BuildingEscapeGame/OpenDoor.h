// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPEGAME_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime);
private:
	float InitialYaw;
	float CurrentYaw;

	UPROPERTY(EditAnywhere)
	float TargetYaw = 90.0f;

	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate;

	UPROPERTY(EditAnywhere)
	AActor* ActorThatOpen;

	float DoorLastOpened = 0;

	UPROPERTY(EditAnywhere)
	float DoorClosedDelay = .5f;
	UPROPERTY(EditAnywhere)
	float DoorOpenSpeed = .8f;
	UPROPERTY(EditAnywhere)
	float DoorCloseSpeed = 2.f;
};
