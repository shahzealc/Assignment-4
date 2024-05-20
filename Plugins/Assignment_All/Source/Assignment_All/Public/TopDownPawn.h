// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include <Camera/CameraComponent.h>
#include <GameFramework/SpringArmComponent.h>
#include <WallBuilderController.h>
#include "TopDownPawn.generated.h"

UCLASS()
class ASSIGNMENT_ALL_API ATopDownPawn : public APawn
{
	GENERATED_BODY()

public:

	ATopDownPawn();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	UCameraComponent* TopDownCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	USpringArmComponent* CameraSpringArm;

	UPROPERTY()
	class UInputMappingContext* TopDownPawnMappingContext;

	UPROPERTY()
	class UInputAction* MoveAction;

	UPROPERTY()
	class UInputAction* ZoomInOutAction;

	UPROPERTY(EditAnywhere)
	class UFloatingPawnMovement* Movement;

	UPROPERTY(EditAnywhere)
	float MoveScale;

	void Move(const struct FInputActionValue& ActionValue);
	void ZoomInOut(const struct FInputActionValue& ActionValue);


protected:

	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
