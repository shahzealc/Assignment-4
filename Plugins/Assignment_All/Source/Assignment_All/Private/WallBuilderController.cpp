// Fill out your copyright notice in the Description page of Project Settings.


#include "WallBuilderController.h"
#include <Components/WidgetComponent.h>

void AWallBuilderController::BeginPlay()
{
	Super::BeginPlay();
	WallSplineArray.Add(NewObject<AWallSpline>(this));
	WallIndex = 0;

	TriggerNotification.BindUFunction(this, "TriggerWidgetNotification");

}

void AWallBuilderController::SetupInputComponent()
{
	Super::SetupInputComponent();

	WallBuilderMappingContext = NewObject<UInputMappingContext>(this);

	MouseLeftClick = NewObject<UInputAction>(this);
	MouseLeftClick->ValueType = EInputActionValueType::Boolean;

	MouseRightClick = NewObject<UInputAction>(this);
	MouseRightClick->ValueType = EInputActionValueType::Boolean;

	ZClick = NewObject<UInputAction>(this);
	ZClick->ValueType = EInputActionValueType::Boolean;

	WallBuilderMappingContext->MapKey(MouseLeftClick, EKeys::LeftMouseButton);
	WallBuilderMappingContext->MapKey(MouseRightClick, EKeys::RightMouseButton);
	WallBuilderMappingContext->MapKey(ZClick, EKeys::Z);

	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent);

	SetShowMouseCursor(true);

	check(EIC);

	EIC->BindAction(MouseLeftClick, ETriggerEvent::Completed, this, &AWallBuilderController::MouseLeftClickLocation);
	EIC->BindAction(MouseRightClick, ETriggerEvent::Completed, this, &AWallBuilderController::MouseRightClickNewWallCreation);
	EIC->BindAction(ZClick, ETriggerEvent::Completed, this, &AWallBuilderController::UndoConstruction);

	ULocalPlayer* LocalPlayer = GetLocalPlayer();
	check(LocalPlayer);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);

	Subsystem->AddMappingContext(WallBuilderMappingContext, 0);
}

void AWallBuilderController::MouseLeftClickLocation() {

	FHitResult HitResult;
	GetHitResultUnderCursor(ECC_Visibility, true, HitResult);

	if (HitResult.bBlockingHit)
	{
		FVector ClickLocation = HitResult.Location;
		if (WallSplineArray[WallSplineArray.Num() - 1]) {

			WallSplineArray[WallSplineArray.Num() - 1]->SplineLocationPoint(ClickLocation);

			if (WallSplineArray[WallSplineArray.Num() - 1]->SplinePoints == 1) {
				TriggerNotification.ExecuteIfBound("Started Building Wall");
			}

		}
	}
}

void AWallBuilderController::MouseRightClickNewWallCreation()
{

	if (WallSplineArray[WallSplineArray.Num() - 1]->SplinePoints > 0) {

		WallSplineArray.Add(NewObject<AWallSpline>(this));
		WallIndex = WallSplineArray.Num() - 1;
		TriggerNotification.ExecuteIfBound("Wall Construction Ended");

	}
}

void AWallBuilderController::UndoConstruction() {

	if (WallSplineArray[WallIndex]->SplinePoints > 0) {

		WallSplineArray[WallIndex]->UndoOneSplinePoint();
		TriggerNotification.ExecuteIfBound("Undo Done");

	}
	else {
		if (WallSplineArray.Num() > 0) {
			DeleteCurrentWall();
		}
	}

	if (WallIndex >= WallSplineArray.Num() - 1) {
		WallIndex = WallSplineArray.Num() - 1;
	}

}

void AWallBuilderController::DeleteCurrentWall() {

	if (WallSplineArray.Num() > 0) {

		WallSplineArray[WallIndex]->Destroy();
		TriggerNotification.ExecuteIfBound("Deleted Current Wall");
		WallSplineArray.RemoveAt(WallIndex);

		if (WallSplineArray.Num() == 0) {
			WallSplineArray.Add(NewObject<AWallSpline>(this));
			WallIndex = 0;
		}

	}
	if (WallIndex >= WallSplineArray.Num() - 1) {
		WallIndex = WallSplineArray.Num() - 1;
	}
}

void AWallBuilderController::DeleteAllWall() {

	if (WallSplineArray.Num() > 0) {

		for (int32 i = 0; i < WallSplineArray.Num(); i++)
		{
			WallSplineArray[i]->Destroy();
		}

		WallSplineArray.Empty();

		TriggerNotification.ExecuteIfBound("Deleted All Walls");

		WallSplineArray.Add(NewObject<AWallSpline>(this));
		WallIndex = WallSplineArray.Num() - 1;

	}

}

void AWallBuilderController::PreviousWall()
{

	if (WallIndex == 0) {
		TriggerNotification.ExecuteIfBound("No Previous Wall.");
	}
	else {
		WallIndex--;
		FString Message = "Moved to Previous Wall : " + FString::FromInt(WallIndex + 1);
		TriggerNotification.ExecuteIfBound(Message);
	}

}

void AWallBuilderController::NextWall()
{
	if (WallIndex == WallSplineArray.Num() - 1) {
		TriggerNotification.ExecuteIfBound("No Next Wall.");
	}
	else {
		WallIndex++;
		FString Message = "Moved to Next Wall : " + FString::FromInt(WallIndex + 1);
		TriggerNotification.ExecuteIfBound(Message);
	}
}
