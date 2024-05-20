// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include <Camera/CameraComponent.h>
#include <GameFramework/SpringArmComponent.h>
#include <Components/CapsuleComponent.h>
#include "PawnAttributeAsset.h"
#include "FirstPersonPawn.generated.h"


UCLASS()
class ASSIGNMENT_1_2_API AFirstPersonPawn : public APawn
{
	GENERATED_BODY()

public:
	AFirstPersonPawn();

	UPROPERTY()
	UPawnAttributeAsset* PawnAttributeAsset;

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	class UInputMappingContext* PawnMappingContext;

	UPROPERTY()
	class UInputAction* MoveAction;

	UPROPERTY()
	class UInputAction* RotateAction;

	UPROPERTY(EditAnywhere)
	UCapsuleComponent* Capsule;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Body;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(EditAnywhere)
	class UFloatingPawnMovement* Movement;
		
	UPROPERTY(EditAnywhere)
	float MoveScale;

	UPROPERTY(EditAnywhere)
	float RotateScale;

	void Move(const struct FInputActionValue& ActionValue);

	void Rotate(const struct FInputActionValue& ActionValue);

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
