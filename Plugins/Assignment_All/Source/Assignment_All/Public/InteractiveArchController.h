// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "InputModifiers.h"
#include "SelectionWidget.h"
#include "MeshAssetManager.h"
#include "ArchMeshActor.h"

#include "WallSpline.h"

#include "Engine/DataTable.h"
#include "MyWallWidget.h"

#include "InteractiveArchController.generated.h"

DECLARE_DELEGATE_OneParam(FDisplayNotification, FString);

UENUM()
enum class ECameraType : int8 {
	Perspective,
	Isometric,
	Orthographic
};

USTRUCT(BlueprintType)
struct FCameraTypeStruct : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	ECameraType CharacterType{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Class Reference")
	TSubclassOf<APawn> CharacterReference{};
};

/**
 * 
 */
UCLASS()
class ASSIGNMENT_ALL_API AInteractiveArchController : public APlayerController
{
	GENERATED_BODY()
public:
	AInteractiveArchController();

	//Mesh Generator start

	UPROPERTY()
	class UInputMappingContext* MeshMappingContext;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Widget")
	TSubclassOf<USelectionWidget> SelectionWidgetClass;

	UPROPERTY()
	USelectionWidget* SelectionWidget;

	UPROPERTY()
	AArchMeshActor* MeshActor;

	UPROPERTY()
	FVector ClickLocation;

	UPROPERTY()
	UMaterialInstanceDynamic* DynamicMaterial;

	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;

	void SetupInputComponentWallBuilder();
	void SetupInputComponentMeshGenerator();

	void GetLocation();
	void HideWidget();

	UFUNCTION()
	void SpawnMesh(const FMeshData& MeshData);

	UFUNCTION()
	void ApplyMaterial(const FMaterialData& MaterialData);

	UFUNCTION()
	void ApplyTexture(const FTextureData& TextureData);

	UPROPERTY()
	bool bSameLocation=true;

	UPROPERTY()
	bool ToggleMeshGeneratorInput = true;

	//Mesh Generator End

	//Spline Wall Start

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	TSubclassOf<UMyWallWidget> WallWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMyWallWidget* WallWidget;

	UPROPERTY()
	class UInputMappingContext* WallBuilderMappingContext;

	UPROPERTY()
	class UInputAction* MouseLeftClick;

	UPROPERTY()
	class UInputAction* MouseRightClick;

	UPROPERTY()
	class UInputAction* ZClick;

	UFUNCTION(BlueprintImplementableEvent)
	void TriggerWidgetNotification(const FString& TextToChange);

	FDisplayNotification TriggerNotification;

	UPROPERTY()
	TArray<AWallSpline*> WallSplineArray;

	void MouseLeftClickLocation();

	void MouseRightClickNewWallCreation();

	void UndoConstruction();

	UFUNCTION(BlueprintCallable)
	void DeleteCurrentWall();

	UFUNCTION(BlueprintCallable)
	void DeleteAllWall();

	UFUNCTION(BlueprintCallable)
	void PreviousWall();

	UFUNCTION(BlueprintCallable)
	void NextWall();

	UFUNCTION(BlueprintCallable)
	void ToggleToWall();

	UFUNCTION(BlueprintCallable)
	void ToggleToMeshGenerator();

	UPROPERTY()
	int32 WallIndex;
	
	UPROPERTY()
	bool ToggleWallBuilderInput = false;
	//Spline Wall Builder End


	UDataTable* MyDataTable;

	UPROPERTY()
	AActor* CurrentlySpawnedCamera;

	int CurrentCameraIndex = 0;

	void SpawnCamera(bool ChangeCameraType=true);

};
