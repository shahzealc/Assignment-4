// Fill out your copyright notice in the Description page of Project Settings.


#include "FenceMeshActor.h"

AFenceMeshActor::AFenceMeshActor()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = Scene;

	MySplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("MySplineComponent"));
	MySplineComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	MySplineComponent->SetClosedLoop(false);

	VerticalRailStaticMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Script/Engine.StaticMesh'/Game/Assignment5/StaticMesh/Pyramid_SM.Pyramid_SM'"));

}

void AFenceMeshActor::OnConstruction(const FTransform& Transform)
{
	DestroyConstructionMesh();

	switch (RailingTypeFence)
	{
	case ERailingTypeFence::Pyramid:
		VerticalRailStaticMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Script/Engine.StaticMesh'/Game/Assignment5/StaticMesh/SM_Pyramid.SM_Pyramid'"));
		break;
	case ERailingTypeFence::Sphere:
		VerticalRailStaticMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Script/Engine.StaticMesh'/Game/Assignment5/StaticMesh/SM_Sphere.SM_Sphere'"));
		break;
	case ERailingTypeFence::Oval:
		VerticalRailStaticMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Script/Engine.StaticMesh'/Game/Assignment5/StaticMesh/SM_Oval.SM_Oval'"));
		break;
	case ERailingTypeFence::SphereWithCone:
		VerticalRailStaticMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Script/Engine.StaticMesh'/Game/Assignment5/StaticMesh/SM_SphereWithCone.SM_SphereWithCone'"));
		break;
	case ERailingTypeFence::Rectangle:
		VerticalRailStaticMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Script/Engine.StaticMesh'/Game/Assignment5/StaticMesh/SM_Rectangle.SM_Rectangle'"));
		break;
	case ERailingTypeFence::RoundedOverTop:
		VerticalRailStaticMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Script/Engine.StaticMesh'/Game/Assignment5/StaticMesh/SM_RoundedOverTop.SM_RoundedOverTop'"));
		break;
	case ERailingTypeFence::GlobeWithStick:
		VerticalRailStaticMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Script/Engine.StaticMesh'/Game/Assignment5/StaticMesh/SM_GlobeWithStick.SM_GlobeWithStick'"));
		break;
	case ERailingTypeFence::Simple:
		VerticalRailStaticMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Script/Engine.StaticMesh'/Game/Assignment5/StaticMesh/SM_Simple.SM_Simple'"));
		break;
	default:
		break;
	}

	GenerateStaticFence();
}

void AFenceMeshActor::DestroyConstructionMesh() {

	for (int i = 0; i < AttachedVerticalMesh.Num(); i++)
	{
		if (AttachedVerticalMesh[i])
		{
			AttachedVerticalMesh[i]->DestroyComponent();
			AttachedVerticalMesh[i] = nullptr;
		}
	}
	AttachedVerticalMesh.Empty();

}

void AFenceMeshActor::GenerateStaticFence()
{
	auto SplineLength = MySplineComponent->GetSplineLength();

	int32 NumMeshes = FMath::FloorToInt(SplineLength / (FenceProperties.Length + FenceProperties.Spacing));

	for (int32 i = 0; i < MySplineComponent->GetNumberOfSplinePoints() - 1; ++i)
	{
		MySplineComponent->SetSplinePointType(i, ESplinePointType::Linear);
	}

	for (int32 j = 0; j <= NumMeshes; ++j)
	{
		float Distance = j * (FenceProperties.Length + FenceProperties.Spacing);
		FVector Location = MySplineComponent->GetLocationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);
		FRotator Rotation = MySplineComponent->GetRotationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);

		Location.Z += ((FenceProperties.Height * 10) / 2);

		Location += {(j * 0.001f), (j * 0.001f), (j * 0.001f)};

		FString MeshName = "VerticalMesh" + FString::FromInt(NumMeshes + j);
		UStaticMeshComponent* SplineMeshComponent = NewObject<UStaticMeshComponent>(this, *MeshName);
		if (SplineMeshComponent)
		{
			SplineMeshComponent->SetMobility(EComponentMobility::Movable);
			SplineMeshComponent->SetStaticMesh(VerticalRailStaticMesh);
			SplineMeshComponent->SetWorldLocationAndRotation(Location, Rotation);
			SplineMeshComponent->RegisterComponentWithWorld(GetWorld());
			SplineMeshComponent->SetRelativeScale3D({ FenceProperties.Length / 10,FenceProperties.Width / 10,FenceProperties.Height / 10 });

			SplineMeshComponent->SetMaterial(0, FenceMaterial);
			SplineMeshComponent->SetMaterial(1, HorizontalFenceMaterial);
			SplineMeshComponent->SetMaterial(2, HorizontalFenceMaterial);
	
			AttachedVerticalMesh.Add(SplineMeshComponent);
		}
	}

}

void AFenceMeshActor::GenerateFence() {

	auto SplineLength = MySplineComponent->GetSplineLength();

	int32 NumMeshes = FMath::FloorToInt(SplineLength / (FenceProperties.Length + FenceProperties.Spacing));

	for (int32 j = 0; j <= NumMeshes; ++j)
	{
		float Distance = j * (FenceProperties.Length + FenceProperties.Spacing);
		FVector Location = MySplineComponent->GetLocationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);
		FRotator Rotation = MySplineComponent->GetRotationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);

		Location.Z += ((FenceProperties.Height * 10) / 2);

		Location += {(j * 0.001f), (j * 0.001f), (j * 0.001f)};

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		AVerticalRailActor* SpawnedActor = GetWorld()->SpawnActor<AVerticalRailActor>(VerticalRailClass, Location, Rotation, SpawnParams);

		if (SpawnedActor)
		{
			SpawnedActor->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
			SpawnedActor->SetActorRelativeScale3D({ FenceProperties.Length / 10,FenceProperties.Width / 10,FenceProperties.Height / 10 });

			UMaterialInstanceDynamic* VerticalDynamicMaterial = UMaterialInstanceDynamic::Create(FenceMaterial, this);
			if (VerticalDynamicMaterial) {
				TileX = FenceProperties.Length / 20;
				TileY = FenceProperties.Height / 8;
				VerticalDynamicMaterial->SetScalarParameterValue("TileX", TileX);
				VerticalDynamicMaterial->SetScalarParameterValue("TileY", TileY);
				SpawnedActor->SetVerticalMaterial(0, VerticalDynamicMaterial);
			}
			UMaterialInstanceDynamic* HorizontalDynamicMaterial = UMaterialInstanceDynamic::Create(FenceMaterial, this);
			if (HorizontalDynamicMaterial) {
				TileX = FenceProperties.Length / 4;
				TileY = FenceProperties.Height / 40;
				HorizontalDynamicMaterial->SetScalarParameterValue("TileX", TileX);
				HorizontalDynamicMaterial->SetScalarParameterValue("TileY", TileY);
				SpawnedActor->SetVerticalMaterial(1, HorizontalDynamicMaterial);
				SpawnedActor->SetVerticalMaterial(2, HorizontalDynamicMaterial);
			}
		}
	}

}

void AFenceMeshActor::BeginPlay()
{
	Super::BeginPlay();
	DestroyConstructionMesh();
	GenerateFence();
}

void AFenceMeshActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}