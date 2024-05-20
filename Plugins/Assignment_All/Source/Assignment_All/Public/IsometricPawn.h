// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "InputModifiers.h"
#include <GameFramework/FloatingPawnMovement.h>
#include <GameFramework/SpringArmComponent.h>
#include "IsometricPawn.generated.h"

UCLASS()
class ASSIGNMENT_ALL_API AIsometricPawn : public APawn
{
	GENERATED_BODY()

public:
	AIsometricPawn();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	void KeyMap(UInputMappingContext* InputMappingContext, UInputAction* InputAction, FKey Key, bool bNegate, bool bSwizzle, EInputAxisSwizzle SwizzleOrder);

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void RotateCameraCounterClockwise();

	void RotateCameraClockwise();
	void MoveCameraToLocation(FVector InLocation);
	void Zoom(const FInputActionValue& ActionValue);


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY()
	class UInputMappingContext* PawnMappingContext;

	UPROPERTY()
	class UInputAction* RotateClockwise;

	UPROPERTY()
	class UInputAction* RotateCounterClockwise;

	UPROPERTY()
	class UInputAction* ZoomAction;

	float RotationAngleIncrement = 45.0f;
	float TargetYaw = 0.0f;
	float RotationSpeed = 3.0f;
	float TotalRotation = 0.0f;
};