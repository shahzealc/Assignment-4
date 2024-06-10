// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"

#include "SelectionArea.generated.h"

UCLASS()
class ASSIGNMENT_5_6_API ASelectionArea : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASelectionArea();

	UPROPERTY(VisibleAnywhere)
	UProceduralMeshComponent* ProceduralMesh;

	UFUNCTION()
	void GenerateSphere(float Radius, int32 Segments, int32 Rings);

	UFUNCTION()
	void GenerateBox(float Dimention);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Translucent Materials")
	UMaterialInterface* Material;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Box = true;

protected:
	void BuildQuad(TArray<FVector>& InVertices, TArray<int32>& InTriangles, TArray<FVector>& InNormals, TArray<FProcMeshTangent>& InTangents, TArray<FVector2D>& InTexCoords, const FVector BottomLeft, const FVector BottomRight, const FVector TopRight, const FVector TopLeft, int32& VertexOffset, int32& TriangleOffset, const FVector Normal, const FVector Tangent);
	// Called when the game starts or when spawned

public:
	// Called every frame
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	void ToggleShape(const FString& ToggletoShape);
};
