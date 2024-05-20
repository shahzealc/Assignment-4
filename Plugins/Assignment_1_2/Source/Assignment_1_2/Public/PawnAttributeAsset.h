// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PawnAttributeAsset.generated.h"

/**
 *
 */
UCLASS()
class ASSIGNMENT_1_2_API UPawnAttributeAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite ,Meta = (UIMin = "0", ClampMin = "0"))
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite ,Meta = (UIMin = "0", UIMax = "100", ClampMin = "0", ClampMax = "100"))
	int Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite ,Meta = (UIMin = "0", ClampMin = "0"))
	float JumpHeight;

};
