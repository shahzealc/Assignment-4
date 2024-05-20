// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/CanvasPanel.h>
#include <Components/TextBlock.h>
#include "MyWallWidget.generated.h"

/**
 * 
 */
UCLASS()
class ASSIGNMENT_ALL_API UMyWallWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	UTextBlock* NotificationTextWidget;

	virtual void NativeConstruct() override;
	
};
