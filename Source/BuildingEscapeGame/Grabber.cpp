// Fill out your copyright notice in the Description page of Project Settings.
#include "Grabber.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

#define OUT //for us to read as out statements
// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsHandle();
	SetUpInputComponent();
}

//called every Frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if(PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(GetTraceEnd());
	}
}
void UGrabber::FindPhysicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if(PhysicsHandle)
		{/*Nothing*/}
	else 
	{
		UE_LOG(LogTemp, Error, TEXT("No Physics Handle Component on %s"), *GetOwner()->GetName());	
	}
}

void UGrabber::SetUpInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if(InputComponent)
	{
		InputComponent->BindAction("Grab",IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab",IE_Released, this, &UGrabber::Release);
	}

}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{	
	FHitResult Hit;
	
	//dont Count Source as a Collision
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	//Ray-cast out to a certain distance (Reach)
	FVector PlayerViewPointLocation;
	bool PlayerColission = GetWorld()->LineTraceSingleByObjectType
		(
			OUT Hit,
			PlayerViewPointLocation,
			GetTraceEnd(),
			FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
			TraceParams
		);
	
	if (PlayerColission)
	{
		UE_LOG(LogTemp,Warning,TEXT("Line Trace Hit: %s"),*Hit.GetActor()->GetName());
	}
	return Hit;
}

FVector UGrabber::GetTraceEnd() const
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint
		(
			OUT PlayerViewPointLocation,
			OUT PlayerViewPointRotation
		); 

	FVector LineTraceEnd = PlayerViewPointLocation + (PlayerViewPointRotation.Vector() * Reach);
	return LineTraceEnd;
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp,Warning,TEXT("Grabber Press"));
	FHitResult HitResult = GetFirstPhysicsBodyInReach();
	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();
	
	if(HitResult.GetActor())
		PhysicsHandle->GrabComponentAtLocation
			(
				ComponentToGrab,
				NAME_None,
				GetTraceEnd()
			);
}

void UGrabber::Release()
{
	UE_LOG(LogTemp,Warning,TEXT("Grabber Released"));
	if(PhysicsHandle->GrabbedComponent)
		PhysicsHandle->ReleaseComponent();
}
/*
	DrawDebugLine(
		GetWorld(), 
		PlayerViewPointLocation, 
		LineTraceEnd, 
		FColor(0,255,0), 
		false, 
		0.f, 0, 5
		);
	
*/