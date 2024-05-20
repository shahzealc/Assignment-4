// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MeshAssetManager.generated.h"

USTRUCT(BlueprintType)
struct FMeshData {
	GENERATED_BODY()

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Mesh")
	FString MeshName;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMesh* Mesh;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Mesh")
	UTexture2D* MeshThumbnail;

};

USTRUCT(BlueprintType)
struct FMaterialData {
	GENERATED_BODY()

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Material")
	FString MaterialName;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Material")
	UMaterial* Material;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Material")
	UTexture2D* MaterialThumbnail;

};

USTRUCT(BlueprintType)
struct FTextureData {
	GENERATED_BODY()

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Texture")
	FString TextureName;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Texture")
	UTexture* Texture;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Texture")
	UTexture2D* TextureThumbnail;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Texture Specular",Meta=(UMIN=0,UMAX=1))
	float Specular;
	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Texture Metallic",Meta=(UMIN=0,UMAX=1))
	float Metallic;
};

/**
 * 
 */
UCLASS(BlueprintType)
class ASSIGNMENT_ALL_API UMeshAssetManager : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Mesh Structure Array")
	TArray<FMeshData> MeshArray;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Material Structure Array")
	TArray<FMaterialData> MaterialArray;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Texture Structure Array")
	TArray<FTextureData> TextureArray;
	
};
