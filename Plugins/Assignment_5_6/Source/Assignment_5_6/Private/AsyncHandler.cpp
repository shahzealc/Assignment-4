// Fill out your copyright notice in the Description page of Project Settings.


#include "AsyncHandler.h"
#include "MeshGenerator.h"
#include "Async/ParallelFor.h"

FAsyncHandler::FAsyncHandler(AMeshGenerator* InMeshGenerator)
{
	MeshGenerator = InMeshGenerator;
}

void FAsyncHandler::DoWork()
{
	if (MeshGenerator)
	{
		if (UMeshDataAsset* DataAsset = MeshGenerator->DataAsset)
		{
			TArray<FMeshDataStruct> MeshDataStruct = DataAsset->MeshDataStruct;

			for (int iIndex = 0; iIndex < MeshGenerator->NumberOfInstances; iIndex++)
			{
				int RandomIndex = FMath::RandRange(0, MeshDataStruct.Num() - 1);

				UStaticMesh* CurrentMesh = MeshDataStruct[RandomIndex].MeshType;
				float MinScale = MeshDataStruct[RandomIndex].MinScale;
				float MaxScale = MeshDataStruct[RandomIndex].MaxScale;
				float MinRotation = MeshDataStruct[RandomIndex].MinRotation;
				float MaxRotation = MeshDataStruct[RandomIndex].MaxRotation;

				TArray<FTransform> InstanceTransforms;
				FTransform Transform;

				if (MeshGenerator->isBoxSelectionArea == true) {
					FVector BoundingExtent = MeshGenerator->Scale;
					FVector Origin = MeshGenerator->Location;
					FBox BoundingBox(Origin - BoundingExtent, Origin + BoundingExtent);

					Transform.SetLocation(FMath::RandPointInBox(BoundingBox));
				}
				else {
					float Radius = MeshGenerator->Scale.X;
					FVector Origin = MeshGenerator->Location;
					Transform.SetLocation(FMath::VRand() * FMath::FRandRange(0.0f, Radius) + Origin);
				}

				Transform.SetScale3D(FVector(FMath::RandRange(MinScale, MaxScale)));
				Transform.SetRotation(FQuat(FRotator(FMath::RandRange(MinRotation, MaxRotation), FMath::RandRange(MinRotation, MaxRotation), FMath::RandRange(MinRotation, MaxRotation))));
				InstanceTransforms.Add(Transform);

				MeshGenerator->AddInstances(CurrentMesh, InstanceTransforms);
				FPlatformProcess::Sleep(0.01f);
			}


		}

	}
}

