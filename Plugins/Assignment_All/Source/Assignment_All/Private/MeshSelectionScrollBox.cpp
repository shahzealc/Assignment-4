// Fill out your copyright notice in the Description page of Project Settings.


#include "MeshSelectionScrollBox.h"

TSharedRef<SWidget> UMeshSelectionScrollBox::RebuildWidget()
{
	ScrollBoxSlate = SNew(SMeshSelectionScrollBox).MeshAssetManager(MeshAssetManager).AssetType(AssetType);

	ScrollBoxSlate->OnclickMesh.BindUFunction(this, "HandleMeshClick");
	ScrollBoxSlate->OnclickMaterial.BindUFunction(this, "HandleMaterialClick");
	ScrollBoxSlate->OnclickTexture.BindUFunction(this, "HandleTextureClick");

	return ScrollBoxSlate.ToSharedRef();
}

const FText UMeshSelectionScrollBox::GetPaletteCategory() {
	return FText::FromString("Panel");
}

void UMeshSelectionScrollBox::ReleaseSlateResources(bool ReleaseResource)
{
	Super::ReleaseSlateResources(ReleaseResource);
	if (ScrollBoxSlate.IsValid()) {
		ScrollBoxSlate.Reset();
	}
}

void UMeshSelectionScrollBox::HandleMeshClick(const FMeshData& MeshData) {
	OnClickMesh.ExecuteIfBound(MeshData);
}

void UMeshSelectionScrollBox::HandleMaterialClick(const FMaterialData& MaterialData) {
	OnClickMaterial.ExecuteIfBound(MaterialData);
}

void UMeshSelectionScrollBox::HandleTextureClick(const FTextureData& TextureData) {
	OnClickTexture.ExecuteIfBound(TextureData);
}