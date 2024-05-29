// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "VerticalRailActor.h"
#include "FenceMeshActor.generated.h"

UENUM()
enum class ERailingTypeFence {
    Simple,
    Pyramid,
    Sphere,
    SphereWithCone,
    Oval,
    Rectangle,
    RoundedOverTop,
    GlobeWithStick
};

USTRUCT(BlueprintType)
struct FFenceProperties
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fence Properties")
    float Length;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fence Properties")
    float Width;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fence Properties")
    float Height;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fence Properties")
    float Spacing;

    FFenceProperties()
        : Length(20.f), Width(20.f), Height(20.f), Spacing(50.0f)
    {}
};


UCLASS()
class ASSIGNMENT_5_6_API AFenceMeshActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFenceMeshActor();

    UPROPERTY(EditAnyWhere,BlueprintReadWrite, Category = "Fence Properties")
    FFenceProperties FenceProperties;

    UPROPERTY()
    USplineComponent* MySplineComponent;

    UPROPERTY()
    UStaticMesh* VerticalRailStaticMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fence")
    TSubclassOf<AVerticalRailActor> VerticalRailClass;

    UPROPERTY()
    TArray<UStaticMeshComponent*> AttachedVerticalMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RailingType)
    TEnumAsByte<ERailingTypeFence> RailingTypeFence;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fence Material")
    UMaterialInterface* FenceMaterial;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fence Material")
    UMaterialInterface* HorizontalFenceMaterial;

    UPROPERTY(VisibleAnywhere)
    UProceduralMeshComponent* TopHorizontalRailingMesh;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Materials | TopHorizontal Railing Material")
    UMaterialInterface* TopHorizontalRailingMaterial;

    UPROPERTY()
    float TileX;

    UPROPERTY()
    float TileY;

protected:

    virtual void BeginPlay() override;


public:	

    virtual void Tick(float DeltaTime) override;

    virtual void OnConstruction(const FTransform& Transform) override;

    void DestroyConstructionMesh();

    void GenerateStaticFence();

    void GenerateFence();

};
