// Fill out your copyright notice in the Description page of Project Settings.


#include "WallSpline.h"

AWallSpline::AWallSpline()
{
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = Scene;

	MySplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("MySplineComponent"));
	MySplineComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	MySplineComponent->SetClosedLoop(false);
	MySplineComponent->ClearSplinePoints();

	SplineMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Architecture/Wall_400x200.Wall_400x200'"));

	SplineMaterial = LoadObject<UMaterial>(nullptr, TEXT("/Script/Engine.Material'/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial'"));	

}

void AWallSpline::BeginPlay()
{
	Super::BeginPlay();
}

void AWallSpline::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWallSpline::GenerateWall() {

	SplinePoints = MySplineComponent->GetNumberOfSplinePoints() - 1;

	if (SplineMesh && MySplineComponent && SplinePoints >= 1)
	{

		FString name = "SplineMesh" + FString::FromInt(SplinePoints);

		MySplineComponent->SetSplinePointType(SplinePoints - 1, ESplinePointType::Linear);

		SplineMeshComponent = NewObject<USplineMeshComponent>(this, *name);

		if (SplineMeshComponent)
		{
			SplineMeshComponent->SetMobility(EComponentMobility::Movable);
			SplineMeshComponent->SetStaticMesh(SplineMesh);

			FVector StartPos, StartTangent, EndPos, EndTangent;
			MySplineComponent->GetLocationAndTangentAtSplinePoint(SplinePoints - 1, StartPos, StartTangent, ESplineCoordinateSpace::World);
			MySplineComponent->GetLocationAndTangentAtSplinePoint(SplinePoints, EndPos, EndTangent, ESplineCoordinateSpace::World);

			SplineMeshComponent->SetStartAndEnd(StartPos, StartTangent, EndPos, EndTangent);

			SplineMeshComponent->RegisterComponentWithWorld(GetWorld());

			SplineMeshComponent->SetMaterial(0, SplineMaterial);

			SplineArray.Add(SplineMeshComponent);

		}

	}
}

void AWallSpline::SplineLocationPoint(FVector& SplinePointLocation) {

	MySplineComponent->AddSplinePoint(SplinePointLocation, ESplineCoordinateSpace::World);
	GenerateWall();
}

void AWallSpline::UndoOneSplinePoint()
{
	if (SplinePoints > 0) {
		MySplineComponent->RemoveSplinePoint(SplinePoints);

		if ((SplineArray.Num() - 1) >= 0) {
			SplineArray[SplineArray.Num() - 1]->DestroyComponent();
			SplineArray.RemoveAt(SplineArray.Num() - 1);
		}
		if (SplineArray.Num() == 0) {
			MySplineComponent->ClearSplinePoints();
		}

	}
	SplinePoints = MySplineComponent->GetNumberOfSplinePoints() - 1;

}
