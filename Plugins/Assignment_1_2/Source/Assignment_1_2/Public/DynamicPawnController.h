// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Engine/DataTable.h"
#include "PawnAttributeAsset.h"
#include "BaseCharacter.h"
#include <FirstPersonPawn.h>
#include "DynamicPawnController.generated.h"

/**
 * 
 */

UENUM()
enum class EPawnType : int8 {
	ThirdPerson,
	TopDown,
	MyFPS
};

USTRUCT(BlueprintType)
struct FPawnAttributesStruct : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	EPawnType CharacterType{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Class Reference")
	TSubclassOf<APawn> CharacterReference{};
};


UCLASS()
class ASSIGNMENT_1_2_API ADynamicPawnController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

	UFUNCTION(BlueprintImplementableEvent)
	void DisplayAttributes(UPawnAttributeAsset* PawnAttributeAsset);

	void SpawnCharacter();

	UDataTable* MyDataTable;

	UPROPERTY()
	AActor* CurrentlySpawnedActor;

	int CurrentActorIndex=0;
};
