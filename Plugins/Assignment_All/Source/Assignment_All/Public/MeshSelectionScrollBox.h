// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "SMeshSelectionScrollBox.h"
#include "MeshSelectionScrollBox.generated.h"

/**
 * 
 */


DECLARE_DELEGATE_OneParam(FOnClickMeshEvent,const FMeshData&)
DECLARE_DELEGATE_OneParam(FOnClickMaterialEvent,const FMaterialData&)
DECLARE_DELEGATE_OneParam(FOnClickTextureEvent,const FTextureData&)


UCLASS()
class ASSIGNMENT_ALL_API UMeshSelectionScrollBox : public UWidget
{
	GENERATED_BODY()


public:
	TSharedPtr<SMeshSelectionScrollBox> ScrollBoxSlate;

	FOnClickMeshEvent OnClickMesh;
	FOnClickMaterialEvent OnClickMaterial;
	FOnClickTextureEvent OnClickTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MeshAssetData")
	UMeshAssetManager* MeshAssetManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SlateType")
	TEnumAsByte<EAssetTypes> AssetType;

protected:

	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual const FText GetPaletteCategory() override;
	void ReleaseSlateResources(bool ReleaseResource) override;

	UFUNCTION()
	void HandleMeshClick(const FMeshData& MeshData);

	UFUNCTION()
	void HandleMaterialClick(const FMaterialData& MeshData);
	
	UFUNCTION()
	void HandleTextureClick(const FTextureData& MeshData);
	
};
