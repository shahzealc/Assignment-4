// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MeshSelectionScrollBox.h"
#include <Components/CanvasPanel.h>
#include "SelectionWidget.generated.h"

/**
 * 
 */
UCLASS()
class ASSIGNMENT_ALL_API USelectionWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* Canvas;

	UPROPERTY(meta = (BindWidget))
	UMeshSelectionScrollBox* MeshScrollBox;

	UPROPERTY(meta = (BindWidget))
	UMeshSelectionScrollBox* MaterialScrollBox;

	UPROPERTY(meta = (BindWidget))
	UMeshSelectionScrollBox* TextureScrollBox;

	virtual void NativeConstruct() override;
};
