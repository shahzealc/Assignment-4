// Fill out your copyright notice in the Description page of Project Settings.


#include "MeshGenerator.h"

AMeshGenerator::AMeshGenerator()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AsyncScatterTask = nullptr;
}

AMeshGenerator::~AMeshGenerator()
{
	if (AsyncScatterTask)
	{
		if (!AsyncScatterTask->IsDone())
		{
			AsyncScatterTask->EnsureCompletion();
		}
		delete AsyncScatterTask;
		AsyncScatterTask = nullptr;
	}

	Cleanup();
}

// Called when the game starts or when spawned
void AMeshGenerator::BeginPlay()
{
	Super::BeginPlay();

}

void AMeshGenerator::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if (AsyncScatterTask)
	{
		if (!AsyncScatterTask->IsDone()) {
			AsyncScatterTask->EnsureCompletion();

		}
		delete AsyncScatterTask;
		AsyncScatterTask = nullptr;
	}
}

// Called every frame
void AMeshGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMeshGenerator::GenerateStaticMeshes(int32 NumberOfInstances_, FVector Location_, FVector Scale_,bool isBoxSelectionArea_)
{
	Cleanup();
	NumberOfInstances = NumberOfInstances_;
	Location = Location_;
	Scale = Scale_;
	ProgressValue = 1.0f / static_cast<float>(NumberOfInstances);
	CurrentProgress = 0.0f;
	count = 0;
	isBoxSelectionArea = isBoxSelectionArea_;

	if (DataAsset)
	{
		if (AsyncScatterTask && !AsyncScatterTask->IsDone())
		{
			AsyncScatterTask->EnsureCompletion();
			delete AsyncScatterTask;
			AsyncScatterTask = nullptr;
		}

		AsyncScatterTask = new FAsyncTask<FAsyncHandler>(this);
		AsyncScatterTask->StartBackgroundTask();

	}
}

void AMeshGenerator::AddInstances(UStaticMesh* StaticMesh, const TArray<FTransform>& Transforms)
{
	AsyncTask(ENamedThreads::GameThread, [this, StaticMesh, Transforms]()
		{
			UHierarchicalInstancedStaticMeshComponent** HISMCPtr = HISMComponents.Find(StaticMesh);
			if (HISMCPtr && *HISMCPtr && (*HISMCPtr)->IsValidLowLevel())
			{
				AsyncTask(ENamedThreads::GameThread, [this, HISMCPtr, StaticMesh, Transforms]()
					{
						(*HISMCPtr)->AddInstances(Transforms, false);

					});
			}
			else
			{

				UHierarchicalInstancedStaticMeshComponent* NewHISMC = NewObject<UHierarchicalInstancedStaticMeshComponent>(this);
				NewHISMC->SetStaticMesh(StaticMesh);

				HISMComponents.Add(StaticMesh, NewHISMC);
				NewHISMC->RegisterComponentWithWorld(GetWorld());
				int32 RandomIndex = FMath::RandRange(0,MeshMaterial.Num()-1);
				if (MeshMaterial[RandomIndex])
				{
					NewHISMC->SetMaterial(0, MeshMaterial[RandomIndex]);
				}
				NewHISMC->AddInstances(Transforms, false);

			}
			CurrentProgress += ProgressValue;
			IncreaseProgress(CurrentProgress);

			count++;
			if (count == NumberOfInstances)
				RemoveProgressBar();

		});
		
}

void AMeshGenerator::Cleanup()
{
	for (auto& Pair : HISMComponents)
	{
		if (UHierarchicalInstancedStaticMeshComponent* HISM = Pair.Value)
		{
			if (HISM->IsValidLowLevel())
			{
				HISM->ClearInstances();
			}
		}
	}
	//HISMComponents.Empty();
}