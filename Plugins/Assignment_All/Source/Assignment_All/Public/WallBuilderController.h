// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "InputModifiers.h"
#include "WallSpline.h"
#include "WallBuilderController.generated.h"

DECLARE_DELEGATE_OneParam(FDisplayNotification, FString);

/**
 * 
 */
UCLASS()
class ASSIGNMENT_ALL_API AWallBuilderController : public APlayerController
{
	GENERATED_BODY()
	
public:

	virtual void BeginPlay() override;
	UPROPERTY()
	class UInputMappingContext* WallBuilderMappingContext;

	UPROPERTY()
	class UInputAction* MouseLeftClick;

	UPROPERTY()
	class UInputAction* MouseRightClick;

	UPROPERTY()
	class UInputAction* ZClick;

	UFUNCTION(BlueprintImplementableEvent)
	void TriggerWidgetNotification(const FString& TextToChange);

	FDisplayNotification TriggerNotification;

	UPROPERTY()
	TArray<AWallSpline*> WallSplineArray;

	virtual void SetupInputComponent() override;

	void MouseLeftClickLocation();
	
	void MouseRightClickNewWallCreation();

	void UndoConstruction();

	UFUNCTION(BlueprintCallable)
	void DeleteCurrentWall();

	UFUNCTION(BlueprintCallable)
	void DeleteAllWall();

	UFUNCTION(BlueprintCallable)
	void PreviousWall();

	UFUNCTION(BlueprintCallable)
	void NextWall();

	UPROPERTY()
	int32 WallIndex;

};
