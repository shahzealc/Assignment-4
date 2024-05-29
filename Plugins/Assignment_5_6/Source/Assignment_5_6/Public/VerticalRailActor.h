// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"

#include "VerticalRailActor.generated.h"

UENUM()

enum class ERailingType {
	Simple,
	Pyramid,
	Sphere,	
	SphereWithCone,
	Oval,
	Rectangle,
	RoundedOverTop,
	GlobeWithStick
};

UCLASS()
class ASSIGNMENT_5_6_API AVerticalRailActor : public AActor
{
	GENERATED_BODY()
	
public:	

	AVerticalRailActor();

	virtual void OnConstruction(const FTransform& Transform) override;

	//Vertical Railing
	UPROPERTY(VisibleAnywhere)
	UProceduralMeshComponent* VerticalRailingMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Materials | Vertical Railing Material")
	UMaterialInterface* VerticalRailingMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Railing)
	double Length=15;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Railing)
	double Width=5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Railing)
	double Height=100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RailingType)
	TEnumAsByte<ERailingType> RailingType;

	UFUNCTION()
	void GenerateVerticalRail();

	void BuildQuad(TArray<FVector>& InVertices, TArray<int32>& InTriangles, TArray<FVector>& InNormals, TArray<FProcMeshTangent>& InTangents, TArray<FVector2D>& InTexCoords, const FVector BottomLeft, const FVector BottomRight, const FVector TopRight, const FVector TopLeft, int32& VertexOffset, int32& TriangleOffset, const FVector Normal, const FVector Tangent);
	
	//Top HorizontalRail
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Materials | Top Horizonatal Railing Material")
	UMaterialInterface* TopHorizontalRailingMaterial;

	void GenerateTopHorizontalRailingMesh();

	//Bottom HorizontalRail
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Materials | Bottom Horizonatal Railing Material")
	UMaterialInterface* BottomHorizontalRailingMaterial;

	void GenerateBottomHorizontalRailingMesh();

	//Pyramid
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Materials | Pyramid Material")
	UMaterialInterface* PyramidMaterial;

	void GeneratePyramid(float BaseLength, float BaseWidth, float Height_);

	//Sphere
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Materials | Sphere Material")
	UMaterialInterface* SphereMaterial;

	void GenerateSphere(float Radius, int32 Segments, int32 Rings);

	//Oval
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Materials | Oval Material")
	UMaterialInterface* OvalMaterial;

	void GenerateOval(float Radius, int32 Segments, int32 Rings);

	//Sphere With Cone
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Materials | SphereCone Material")
	UMaterialInterface* SphereConeMaterial;

	void GenerateSphereWithCone(float BaseLength, float BaseWidth, float Height_);

	//Rectangle
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Materials | Rectangle Material")
	UMaterialInterface* RectangleMaterial;

	void GenerateRectangle();

	//Rounded Over Top
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Materials | RoundedOverTop Material")
	UMaterialInterface* RoundedOverTopMaterial;

	void GenerateRoundedOverTop(float Radius, int32 Segments, int32 Rings);

	void GenerateSemiCapsule();

	void SetVerticalMaterial(int segment,UMaterialInterface* Material);

	//Stand
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Materials | Stand Material")
	UMaterialInterface* StandMaterial;

	void GenerateStand(FVector Location, float BaseRadius, float Height1, float RimRadius, float CurvatureFactor, int NumSlices, int NumStacks);

protected:

	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
	TArray<FVector> Vertices_Arr;
	UPROPERTY()
	TArray<int32> Triangles_Arr;
	UPROPERTY()
	TArray<FVector> Normals_Arr;
	UPROPERTY()
	TArray<FVector2D> UVs_Arr;
	UPROPERTY()
	TArray<FProcMeshTangent> Tangents_Arr;
	UPROPERTY()
	TArray<FLinearColor> Colors_Arr;

};

