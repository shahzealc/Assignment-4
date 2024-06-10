// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController_Assignment6.h"

void APlayerController_Assignment6::BeginPlay()
{
    Super::BeginPlay();
    SelectionActor = GetWorld()->SpawnActor<ASelectionArea>(ASelectionArea::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
}

void APlayerController_Assignment6::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

	if (MoveActor)
	{
		FHitResult HitResult;
		FCollisionQueryParams TraceParams(FName(TEXT("LineTrace")), true, SelectionActor);

		FVector CursorWorldLocation;
		FVector CursorWorldDirection;
		DeprojectMousePositionToWorld(CursorWorldLocation, CursorWorldDirection);

		if (GetWorld()->LineTraceSingleByChannel(HitResult, CursorWorldLocation, CursorWorldLocation + CursorWorldDirection * 10000, ECC_Visibility, TraceParams)) {
			FVector NewLocation = HitResult.Location;
			FVector scale = SelectionActor->GetActorRelativeScale3D();
			NewLocation.Z = scale.Z * 100;
			SelectionActor->SetActorRelativeLocation(NewLocation);
		}

	}
	
}

void APlayerController_Assignment6::ToggleSelectionArea(const FString& ToggletoShape) {

	if (SelectionActor)
	{	
		ASelectionArea* SelectionArea = Cast<ASelectionArea>(SelectionActor);
		if (SelectionArea)
		{
			SelectionArea->ToggleShape(ToggletoShape);
		}
	}
}

void APlayerController_Assignment6::ChangeRadiusAndDimention(FVector RadiusDimention)
{
	SelectionActor->SetActorRelativeScale3D(RadiusDimention/100);
	FVector ActorLocation = SelectionActor->GetActorLocation();
	FVector scale = SelectionActor->GetActorRelativeScale3D();
	ActorLocation.Z = RadiusDimention.Z;
	SelectionActor->SetActorRelativeLocation(ActorLocation);
}

