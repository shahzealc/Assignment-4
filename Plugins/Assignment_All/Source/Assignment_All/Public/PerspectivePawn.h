// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Pawn.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "InputModifiers.h"
#include <GameFramework/FloatingPawnMovement.h>
#include <GameFramework/SpringArmComponent.h>
#include "PerspectivePawn.generated.h"

UCLASS()
class ASSIGNMENT_ALL_API APerspectivePawn : public APawn
{
	GENERATED_BODY()

public:
	APerspectivePawn();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	void KeyMap(UInputMappingContext* InputMappingContext, UInputAction* InputAction, FKey Key, bool bNegate, bool bSwizzle, EInputAxisSwizzle SwizzleOrder);

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Move(const struct FInputActionValue& ActionValue);

	void Rotate(const struct FInputActionValue& ActionValue);

	UPROPERTY(EditAnywhere)
	UCameraComponent* CameraComponent;

	UPROPERTY(EditAnywhere)
	UFloatingPawnMovement* Movement;

	UPROPERTY()
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(EditAnywhere)
	USceneComponent* Scene;

	UPROPERTY()
	class UInputMappingContext* PawnMappingContext;

	UPROPERTY()
	class UInputAction* MoveAction;

	UPROPERTY()
	class UInputAction* RotateAction;

	UPROPERTY(EditAnywhere)
	float MoveScale;

	UPROPERTY(EditAnywhere)
	float RotateScale;

};