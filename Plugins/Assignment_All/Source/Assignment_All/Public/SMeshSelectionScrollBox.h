// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <Widgets/Layout/SScrollBox.h>
#include "MeshAssetManager.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */

DECLARE_DELEGATE_OneParam(FCallOnClickMesh, const FMeshData&);
DECLARE_DELEGATE_OneParam(FCallOnClickMaterial, const FMaterialData&);
DECLARE_DELEGATE_OneParam(FCallOnClickTexture, const FTextureData&);

UENUM(BlueprintType)
enum class EAssetTypes : uint8
{
	Mesh,
	Material,
	Texture
};

class ASSIGNMENT_ALL_API SMeshSelectionScrollBox : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SMeshSelectionScrollBox)
	{}
	SLATE_ARGUMENT(TWeakObjectPtr<UMeshAssetManager>,MeshAssetManager)
	SLATE_ARGUMENT(EAssetTypes, AssetType)
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	FCallOnClickMesh OnclickMesh;
	FCallOnClickMaterial OnclickMaterial;
	FCallOnClickTexture OnclickTexture;

private:

	TWeakObjectPtr<UMeshAssetManager> MeshAssetManager;

	TSharedPtr<SScrollBox> ScrollContainer;
	TSharedPtr <STextBlock> AssetTypeText;
	TEnumAsByte<EAssetTypes> AssetType;

	void RefereshAssets();

	void UpdateAssetTypeText();

	void ShowAssetType();

	void ShowMesh();
	void ShowMaterial();
	void ShowTexture();

	void AddAssetThumbnail(UTexture2D* ThumbnailTexture, const FString& AssetName, const FPointerEventHandler& OnClickHandler);



};
