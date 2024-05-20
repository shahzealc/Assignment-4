// Fill out your copyright notice in the Description page of Project Settings.


#include "SMeshSelectionScrollBox.h"
#include "SlateOptMacros.h"
#include "Brushes/SlateColorBrush.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SMeshSelectionScrollBox::Construct(const FArguments& InArgs)
{
	ScrollContainer = SNew(SScrollBox).Orientation(EOrientation::Orient_Horizontal);
	MeshAssetManager = InArgs._MeshAssetManager;
	AssetType = InArgs._AssetType;

	TSharedPtr<SVerticalBox> MainVertical = SNew(SVerticalBox);
	AssetTypeText = SNew(STextBlock)
		.Justification(ETextJustify::Center)
		//.ColorAndOpacity(FLinearColor::Green)
		.ColorAndOpacity(FLinearColor(0.89627f, 0.783538f, 0.40724f, 1.0f))
		.Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Bold.ttf"), 20));

	MainVertical->AddSlot().FillHeight(0.2)[AssetTypeText.ToSharedRef()];
	MainVertical->AddSlot()[ScrollContainer.ToSharedRef()];

	ChildSlot[MainVertical.ToSharedRef()];

	RefereshAssets();
}

void SMeshSelectionScrollBox::RefereshAssets()
{
	ScrollContainer->ClearChildren();

	if (MeshAssetManager.IsValid())
	{
		UpdateAssetTypeText();
		ShowAssetType();
	}
}

void SMeshSelectionScrollBox::UpdateAssetTypeText()
{
	FText AssetText;
	switch (AssetType)
	{
	case EAssetTypes::Mesh:
		AssetText = FText::FromString("Meshes");
		break;
	case EAssetTypes::Material:
		AssetText = FText::FromString("Materials");
		break;
	case EAssetTypes::Texture:
		AssetText = FText::FromString("Textures");
		break;
	}
	AssetTypeText.ToSharedRef().Get().SetText(AssetText);
}

void SMeshSelectionScrollBox::ShowAssetType()
{
	switch (AssetType)
	{
	case EAssetTypes::Mesh:
		ShowMesh();
		break;
	case EAssetTypes::Material:
		ShowMaterial();
		break;
	case EAssetTypes::Texture:
		ShowTexture();
		break;
	}
}

void SMeshSelectionScrollBox::ShowMesh() {

	for (const FMeshData& MeshData : MeshAssetManager->MeshArray) {

		if (UTexture2D* ThumbnailTexture = Cast<UTexture2D>(MeshData.MeshThumbnail)) {

			FSlateBrush* ThumbnailBrush = new FSlateBrush();
			ThumbnailBrush->SetResourceObject(ThumbnailTexture);
			ThumbnailBrush->ImageSize = FVector2D(120, 80);

			TSharedPtr<SVerticalBox> VerticalBox = SNew(SVerticalBox);

			FSlateColorBrush* BorderBrush = new FSlateColorBrush(FLinearColor(0.651406f, 0.651406f, 0.651406f, 1.0f));

			TSharedPtr<SBorder> BorderAroundImage = SNew(SBorder)
				.BorderImage(BorderBrush)
				.Padding(FMargin(10.0f))
				[
					SNew(SImage)
						.Image(ThumbnailBrush)
						.OnMouseButtonDown_Lambda([this, MeshData](const FGeometry& InGeometry, const FPointerEvent& MouseEvent) {
						if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton) {
							OnclickMesh.ExecuteIfBound(MeshData);
							return FReply::Handled();
						}
						return FReply::Unhandled();
							})
						.Cursor(EMouseCursor::Hand)
				];

			TSharedPtr<STextBlock> MeshText = SNew(STextBlock)
				.Text(FText::FromString(MeshData.MeshName))
				.Justification(ETextJustify::Center)
				//.ColorAndOpacity(FLinearColor::Green)
				.ColorAndOpacity(FLinearColor(0.89627f, 0.783538f, 0.40724f, 1.0f))
				.Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Bold.ttf"), 20));

			VerticalBox->AddSlot()
				[
					BorderAroundImage.ToSharedRef()
				];

			VerticalBox->AddSlot().FillHeight(0.2)
				[
					MeshText.ToSharedRef()
				];

			ScrollContainer->AddSlot()
				[
					VerticalBox.ToSharedRef()
				];
		}
	}

}

void SMeshSelectionScrollBox::ShowMaterial() {

	for (const FMaterialData& MaterialData : MeshAssetManager->MaterialArray) {

		if (UTexture2D* ThumbnailTexture = Cast<UTexture2D>(MaterialData.MaterialThumbnail)) {

			FSlateBrush* ThumbnailBrush = new FSlateBrush();
			ThumbnailBrush->SetResourceObject(ThumbnailTexture);
			ThumbnailBrush->ImageSize = FVector2D(120, 80);

			TSharedPtr<SVerticalBox> VerticalBox = SNew(SVerticalBox);

			FSlateColorBrush* BorderBrush = new FSlateColorBrush(FLinearColor(0.651406f, 0.651406f, 0.651406f, 1.0f));

			TSharedPtr<SBorder> BorderAroundImage = SNew(SBorder)
				.BorderImage(BorderBrush)
				.Padding(FMargin(10.0f))
				[
					SNew(SImage)
						.Image(ThumbnailBrush)
						.OnMouseButtonDown_Lambda([this, MaterialData](const FGeometry& InGeometry, const FPointerEvent& MouseEvent) {
						if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton) {
							OnclickMaterial.ExecuteIfBound(MaterialData);
							return FReply::Handled();
						}
						return FReply::Unhandled();
							})
						.Cursor(EMouseCursor::Hand)
				];

			TSharedPtr<STextBlock> MeshText = SNew(STextBlock)
				.Text(FText::FromString(MaterialData.MaterialName))
				.Justification(ETextJustify::Center)
				//.ColorAndOpacity(FLinearColor::Green)
				.ColorAndOpacity(FLinearColor(0.89627f, 0.783538f, 0.40724f, 1.0f))
				.Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Bold.ttf"), 20));

			VerticalBox->AddSlot()
				[
					BorderAroundImage.ToSharedRef()
				];

			VerticalBox->AddSlot().FillHeight(0.2)
				[
					MeshText.ToSharedRef()
				];

			ScrollContainer->AddSlot()
				[
					VerticalBox.ToSharedRef()
				];
		}

	}

}

void SMeshSelectionScrollBox::ShowTexture() {

	for (const FTextureData& TextureData : MeshAssetManager->TextureArray) {

		if (UTexture2D* ThumbnailTexture = Cast<UTexture2D>(TextureData.TextureThumbnail)) {

			FSlateBrush* ThumbnailBrush = new FSlateBrush();
			ThumbnailBrush->SetResourceObject(ThumbnailTexture);
			ThumbnailBrush->ImageSize = FVector2D(120, 80);

			TSharedPtr<SVerticalBox> VerticalBox = SNew(SVerticalBox);

			FSlateColorBrush* BorderBrush = new FSlateColorBrush(FLinearColor(0.651406f, 0.651406f, 0.651406f, 1.0f));

			TSharedPtr<SBorder> BorderAroundImage = SNew(SBorder)
				.BorderImage(BorderBrush)
				.Padding(FMargin(10.0f))
				[
					SNew(SImage)
						.Image(ThumbnailBrush)
						.OnMouseButtonDown_Lambda([this, TextureData](const FGeometry& InGeometry, const FPointerEvent& MouseEvent) {
						if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton) {
							OnclickTexture.ExecuteIfBound(TextureData);
							return FReply::Handled();
						}
						return FReply::Unhandled();
							})
						.Cursor(EMouseCursor::Hand)
				];
			TSharedPtr<STextBlock> MeshText = SNew(STextBlock)
				.Text(FText::FromString(TextureData.TextureName))
				.Justification(ETextJustify::Center)
				.ColorAndOpacity(FLinearColor(0.89627f, 0.783538f, 0.40724f, 1.0f))
				.Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Bold.ttf"), 20));

			VerticalBox->AddSlot()
				[
					BorderAroundImage.ToSharedRef()
				];

			VerticalBox->AddSlot().FillHeight(0.2)
				[
					MeshText.ToSharedRef()
				];

			ScrollContainer->AddSlot()
				[
					VerticalBox.ToSharedRef()
				];
		}

	}
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

