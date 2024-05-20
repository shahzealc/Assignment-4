// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/SplineComponent.h>
#include <Components/SplineMeshComponent.h>
#include "StaircaseActor.generated.h"

UENUM(BlueprintType)
enum TypeOfStair {
	Open,
	Close,
	Box,
};

USTRUCT(BlueprintType)
struct FStairStaticMeshStruct
{
	GENERATED_BODY()

public:

	UPROPERTY()
	UStaticMeshComponent* Stairs;

	UPROPERTY()
	UStaticMeshComponent* RightRailing;

	UPROPERTY()
	UStaticMeshComponent* LeftRailing;

};

UCLASS()
class ASSIGNMENT_1_2_API AStaircaseActor : public AActor
{
	GENERATED_BODY()

public:
	AStaircaseActor();

	virtual void OnConstruction(const FTransform& Ftransform);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Stairs|Stairs", Meta = (ClampMin = "0"))
	int NumberOfStairs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Stairs|Stairs")
	bool AddRailings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Stairs|Stair Dimention", Meta = (ClampMin = "0"))
	float Width;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Stairs|Stair Dimention", Meta = (ClampMin = "0"))
	float Depth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Stairs|Stair Dimention", Meta = (ClampMin = "0"))
	float Height;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Stairs|Step Placement", Meta = (EditCondition = "StairType != TypeOfStair::Close && StairType != TypeOfStair::Box"), Meta = (ClampMin = "0"))
	float StepHeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Stairs|Step Placement", Meta = (EditCondition = "StairType != TypeOfStair::Close && StairType != TypeOfStair::Box"), Meta = (ClampMin = "0"))
	float StepRelativeWidth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Stairs|Step Type")
	TEnumAsByte<TypeOfStair> StairType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Stairs|Mesh")
	UStaticMesh* StairStaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Stairs|Mesh")
	UMaterial* StairsMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Stairs|Mesh")
	UMaterial* RailingMaterial;

	void ClearStairArray(TArray<FStairStaticMeshStruct> StairArray);


	UPROPERTY()
	TArray<FStairStaticMeshStruct> StairArray;

	USceneComponent* scene;
	FVector location;
	UStaticMeshComponent* Stair;
	UStaticMeshComponent* SubStair;
	UStaticMeshComponent* RightRailing;
	UStaticMeshComponent* LeftRailing;



public:
	virtual void Tick(float DeltaTime) override;

};
