// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SelectionArea.h"
#include "PlayerController_Assignment6.generated.h"

/**
 *
 */
UCLASS()
class ASSIGNMENT_5_6_API APlayerController_Assignment6 : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void ToggleSelectionArea(const FString& ToggletoShape);

	UFUNCTION(BlueprintCallable)
	void ChangeRadiusAndDimention(FVector RadiusDimention);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool MoveActor = true;

protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ASelectionArea* SelectionActor;


};
