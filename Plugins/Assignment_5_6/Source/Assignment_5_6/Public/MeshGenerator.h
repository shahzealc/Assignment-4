// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include <Components/BoxComponent.h>
#include <Components/SphereComponent.h>
#include "MeshDataAsset.h"
#include "AsyncHandler.h"
#include "MeshGenerator.generated.h"

UCLASS()
class ASSIGNMENT_5_6_API AMeshGenerator : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMeshGenerator();
	virtual ~AMeshGenerator();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Asset")
	UMeshDataAsset* DataAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Instances")
	int32 NumberOfInstances;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Location")
	FVector Location;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scale")
	FVector Scale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material")
	TArray<UMaterialInterface*> MeshMaterial;

	UPROPERTY()
	float ProgressValue;
	
	UPROPERTY(BlueprintReadOnly)
	float CurrentProgress=0;

	UPROPERTY()
	int32 count=0;

	UPROPERTY()
	bool isBoxSelectionArea;

	UFUNCTION(BlueprintCallable)
	void GenerateStaticMeshes(int32 NumberOfInstances_, FVector Location_, FVector Scale_,bool isBoxSelectionArea_);

	UFUNCTION(BlueprintImplementableEvent)
	void IncreaseProgress(float CurrentProgressValue);

	UFUNCTION(BlueprintImplementableEvent)
	void RemoveProgressBar();

	void AddInstances(UStaticMesh* StaticMesh, const TArray<FTransform>& Transforms);

	void Cleanup();

private:

	FAsyncTask<FAsyncHandler>* AsyncScatterTask;

	UPROPERTY()
	TMap<UStaticMesh*, UHierarchicalInstancedStaticMeshComponent*> HISMComponents;

};
