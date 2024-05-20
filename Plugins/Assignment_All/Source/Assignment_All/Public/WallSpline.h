// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/SplineComponent.h>
#include <Components/SplineMeshComponent.h>
#include "WallSpline.generated.h"



UCLASS()
class ASSIGNMENT_ALL_API AWallSpline : public AActor
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

public:

	AWallSpline();

	UPROPERTY(EditAnywhere)
	USplineComponent* MySplineComponent;

	UPROPERTY(EditAnywhere, Category = "Spline Mesh")
	UStaticMesh* SplineMesh;

	UPROPERTY()
	USplineMeshComponent* SplineMeshComponent;

	UPROPERTY(EditAnywhere, Category = "Spline Material")
	UMaterial* SplineMaterial;

	UPROPERTY(EditAnywhere, Category = "Spline Points")
	int SplinePoints = 0;

	TArray<USplineMeshComponent*> SplineArray;

	void GenerateWall();

	void SplineLocationPoint(FVector& SplinePointLocation);

	void UndoOneSplinePoint();

	virtual void Tick(float DeltaTime) override;

};
