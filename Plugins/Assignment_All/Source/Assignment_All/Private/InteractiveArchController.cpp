// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractiveArchController.h"

//Mesh Generator Start

AInteractiveArchController::AInteractiveArchController()
{
	SetShowMouseCursor(true);

}

void AInteractiveArchController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (ToggleMeshGeneratorInput) {
		SetupInputComponentMeshGenerator();
	}
	else if (ToggleWallBuilderInput) {
		SetupInputComponentWallBuilder();
	}
}

void AInteractiveArchController::SetupInputComponentMeshGenerator()
{
	SetShowMouseCursor(true);
	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent)) {

		MeshMappingContext = NewObject<UInputMappingContext>(this);

		UInputAction* LeftClick = NewObject<UInputAction>(this);
		LeftClick->ValueType = EInputActionValueType::Boolean;

		UInputAction* TabClick = NewObject<UInputAction>(this);
		TabClick->ValueType = EInputActionValueType::Boolean;

		UInputAction* CameraSpawn = NewObject<UInputAction>(this);
		CameraSpawn->ValueType = EInputActionValueType::Boolean;

		MeshMappingContext->MapKey(LeftClick, EKeys::LeftMouseButton);
		MeshMappingContext->MapKey(TabClick, EKeys::Tab);
		MeshMappingContext->MapKey(CameraSpawn, EKeys::P);

		EIC->BindAction(LeftClick, ETriggerEvent::Completed, this, &AInteractiveArchController::GetLocation);
		EIC->BindAction(TabClick, ETriggerEvent::Completed, this, &AInteractiveArchController::HideWidget);
		EIC->BindAction(CameraSpawn, ETriggerEvent::Completed, this, &AInteractiveArchController::SpawnCamera,true);

		UEnhancedInputLocalPlayerSubsystem* Subsystem = GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();

		Subsystem->RemoveMappingContext(WallBuilderMappingContext);

		Subsystem->AddMappingContext(MeshMappingContext, 0);
	}

}

void AInteractiveArchController::BeginPlay()
{
	Super::BeginPlay();

	MyDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Script/Engine.DataTable'/Game/Assignment4/CameraTypeDataTable/CameraPawnDataTable.CameraPawnDataTable'"));
	SpawnCamera();

	//Mesh Generator
	if (SelectionWidgetClass) {
		SelectionWidget = CreateWidget<USelectionWidget>(this, SelectionWidgetClass);
		SelectionWidget->AddToViewport();
		SelectionWidget->MeshScrollBox->SetVisibility(ESlateVisibility::Hidden);
		SelectionWidget->MaterialScrollBox->SetVisibility(ESlateVisibility::Hidden);
		SelectionWidget->TextureScrollBox->SetVisibility(ESlateVisibility::Hidden);
	}
	SelectionWidget->MeshScrollBox->OnClickMesh.BindUFunction(this, "SpawnMesh");
	SelectionWidget->MaterialScrollBox->OnClickMaterial.BindUFunction(this, "ApplyMaterial");
	SelectionWidget->TextureScrollBox->OnClickTexture.BindUFunction(this, "ApplyTexture");

	//Wall Builder
	if (WallWidgetClass) {
		WallWidget = CreateWidget<UMyWallWidget>(this, WallWidgetClass);
		WallWidget->AddToViewport();
		WallWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	WallSplineArray.Add(NewObject<AWallSpline>(this));
	WallIndex = 0;

	TriggerNotification.BindUFunction(this, "TriggerWidgetNotification");

}

void AInteractiveArchController::GetLocation() {
	FHitResult HitResult;
	GetHitResultUnderCursor(ECC_Visibility, true, HitResult);

	if (HitResult.bBlockingHit)
	{
		AArchMeshActor* HitActor = Cast<AArchMeshActor>(HitResult.GetActor());
		if (HitActor) {
			MeshActor = HitActor;
			SelectionWidget->MeshScrollBox->SetVisibility(ESlateVisibility::Visible);
			SelectionWidget->MaterialScrollBox->SetVisibility(ESlateVisibility::Visible);
			SelectionWidget->TextureScrollBox->SetVisibility(ESlateVisibility::Visible);
			ClickLocation = HitActor->GetActorLocation();
			ClickLocation = ClickLocation - 50;
			DynamicMaterial = UMaterialInstanceDynamic::Create(HitActor->GetStaticMeshComponent()->GetMaterial(0), this);
			SpawnCamera(false);
		}
		else {
			ClickLocation = HitResult.Location;
			SelectionWidget->MeshScrollBox->SetVisibility(ESlateVisibility::Visible);
			SelectionWidget->MaterialScrollBox->SetVisibility(ESlateVisibility::Hidden);
			SelectionWidget->TextureScrollBox->SetVisibility(ESlateVisibility::Hidden);
			bSameLocation = true;
		}

	}

}

void AInteractiveArchController::SpawnMesh(const FMeshData& MeshData) {

	FActorSpawnParameters SpawnParams;

	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FVector Max = MeshData.Mesh->GetBoundingBox().Max;

	if (MeshActor && !bSameLocation) {
		MeshActor->GetStaticMeshComponent()->DestroyComponent();
	}

	if (MeshData.MeshName == "Couch" || MeshData.MeshName == "Table" || MeshData.MeshName == "Chair") {
		MeshActor = GetWorld()->SpawnActor<AArchMeshActor>(ClickLocation, FRotator::ZeroRotator, SpawnParams);
	}
	else {
		MeshActor = GetWorld()->SpawnActor<AArchMeshActor>(ClickLocation + (0, 0, Max), FRotator::ZeroRotator, SpawnParams);
	}

	if (MeshActor) {
		MeshActor->GetStaticMeshComponent()->SetMobility(EComponentMobility::Movable);
		MeshActor->GetStaticMeshComponent()->SetStaticMesh(MeshData.Mesh);
		SelectionWidget->MaterialScrollBox->SetVisibility(ESlateVisibility::Visible);
		SelectionWidget->TextureScrollBox->SetVisibility(ESlateVisibility::Visible);
		DynamicMaterial = UMaterialInstanceDynamic::Create(MeshActor->GetStaticMeshComponent()->GetMaterial(0), this);
		SpawnCamera(false);
	}
	bSameLocation = false;
}

void AInteractiveArchController::ApplyMaterial(const FMaterialData& MaterialData) {
	if (MeshActor) {
		MeshActor->GetStaticMeshComponent()->SetMaterial(0, MaterialData.Material);
		DynamicMaterial = UMaterialInstanceDynamic::Create(MeshActor->GetStaticMeshComponent()->GetMaterial(0), this);
	}
}

void AInteractiveArchController::ApplyTexture(const FTextureData& TextureData) {
	if (MeshActor) {

		//if (!DynamicMaterial)
		//	DynamicMaterial = UMaterialInstanceDynamic::Create(MeshActor->GetStaticMeshComponent()->GetMaterial(0), this);

		if (DynamicMaterial) {
			DynamicMaterial->SetTextureParameterValue("ParameterTexture", TextureData.Texture);
			DynamicMaterial->SetScalarParameterValue("ParameterSpecular", TextureData.Specular);
			DynamicMaterial->SetScalarParameterValue("ParameterMetallic", TextureData.Metallic);
			MeshActor->GetStaticMeshComponent()->SetMaterial(0, DynamicMaterial);
		}

	}
}

void AInteractiveArchController::HideWidget() {
	SelectionWidget->MeshScrollBox->SetVisibility(ESlateVisibility::Hidden);
	SelectionWidget->MaterialScrollBox->SetVisibility(ESlateVisibility::Hidden);
	SelectionWidget->TextureScrollBox->SetVisibility(ESlateVisibility::Hidden);
}

// Mesh Generator End

//Spline Wall Builder Start

void AInteractiveArchController::SetupInputComponentWallBuilder()
{

	WallBuilderMappingContext = NewObject<UInputMappingContext>(this);

	MouseLeftClick = NewObject<UInputAction>(this);
	MouseLeftClick->ValueType = EInputActionValueType::Boolean;

	MouseRightClick = NewObject<UInputAction>(this);
	MouseRightClick->ValueType = EInputActionValueType::Boolean;

	ZClick = NewObject<UInputAction>(this);
	ZClick->ValueType = EInputActionValueType::Boolean;

	UInputAction* CameraSpawn = NewObject<UInputAction>(this);
	CameraSpawn->ValueType = EInputActionValueType::Boolean;

	WallBuilderMappingContext->MapKey(MouseLeftClick, EKeys::LeftMouseButton);
	WallBuilderMappingContext->MapKey(MouseRightClick, EKeys::RightMouseButton);
	WallBuilderMappingContext->MapKey(ZClick, EKeys::Z);
	WallBuilderMappingContext->MapKey(CameraSpawn, EKeys::P);


	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent);

	SetShowMouseCursor(true);

	check(EIC);

	EIC->BindAction(MouseLeftClick, ETriggerEvent::Completed, this, &AInteractiveArchController::MouseLeftClickLocation);
	EIC->BindAction(MouseRightClick, ETriggerEvent::Completed, this, &AInteractiveArchController::MouseRightClickNewWallCreation);
	EIC->BindAction(ZClick, ETriggerEvent::Completed, this, &AInteractiveArchController::UndoConstruction);
	EIC->BindAction(CameraSpawn, ETriggerEvent::Completed, this, &AInteractiveArchController::SpawnCamera,true);

	ULocalPlayer* LocalPlayer = GetLocalPlayer();
	check(LocalPlayer);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);

	Subsystem->RemoveMappingContext(MeshMappingContext);

	Subsystem->AddMappingContext(WallBuilderMappingContext, 0);
}

void AInteractiveArchController::MouseLeftClickLocation() {

	FHitResult HitResult;
	GetHitResultUnderCursor(ECC_Visibility, true, HitResult);

	if (HitResult.bBlockingHit)
	{
		FVector ClickLocationForWall = HitResult.Location;
		if (WallSplineArray[WallSplineArray.Num() - 1]) {

			WallSplineArray[WallSplineArray.Num() - 1]->SplineLocationPoint(ClickLocationForWall);

			if (WallSplineArray[WallSplineArray.Num() - 1]->SplinePoints == 1) {
				TriggerNotification.ExecuteIfBound("Started Building Wall");
			}

		}
	}
}

void AInteractiveArchController::MouseRightClickNewWallCreation()
{

	if (WallSplineArray[WallSplineArray.Num() - 1]->SplinePoints > 0) {

		WallSplineArray.Add(NewObject<AWallSpline>(this));
		WallIndex = WallSplineArray.Num() - 1;
		TriggerNotification.ExecuteIfBound("Wall Construction Ended");

	}
}

void AInteractiveArchController::UndoConstruction() {

	if (WallSplineArray[WallIndex]->SplinePoints > 0) {

		WallSplineArray[WallIndex]->UndoOneSplinePoint();
		TriggerNotification.ExecuteIfBound("Undo Done");

	}
	else {
		if (WallSplineArray.Num() > 0) {
			DeleteCurrentWall();
		}
	}

	if (WallIndex >= WallSplineArray.Num() - 1) {
		WallIndex = WallSplineArray.Num() - 1;
	}

}

void AInteractiveArchController::DeleteCurrentWall() {

	if (WallSplineArray.Num() > 0) {

		WallSplineArray[WallIndex]->Destroy();
		TriggerNotification.ExecuteIfBound("Deleted Current Wall");
		WallSplineArray.RemoveAt(WallIndex);

		if (WallSplineArray.Num() == 0) {
			WallSplineArray.Add(NewObject<AWallSpline>(this));
			WallIndex = 0;
		}

	}
	if (WallIndex >= WallSplineArray.Num() - 1) {
		WallIndex = WallSplineArray.Num() - 1;
	}
}

void AInteractiveArchController::DeleteAllWall() {

	if (WallSplineArray.Num() > 0) {

		for (int32 i = 0; i < WallSplineArray.Num(); i++)
		{
			WallSplineArray[i]->Destroy();
		}

		WallSplineArray.Empty();

		TriggerNotification.ExecuteIfBound("Deleted All Walls");

		WallSplineArray.Add(NewObject<AWallSpline>(this));
		WallIndex = WallSplineArray.Num() - 1;

	}

}

void AInteractiveArchController::PreviousWall()
{

	if (WallIndex == 0) {
		TriggerNotification.ExecuteIfBound("No Previous Wall.");
	}
	else {
		WallIndex--;
		FString Message = "Moved to Previous Wall : " + FString::FromInt(WallIndex + 1);
		TriggerNotification.ExecuteIfBound(Message);
	}

}

void AInteractiveArchController::NextWall()
{
	if (WallIndex == WallSplineArray.Num() - 1) {
		TriggerNotification.ExecuteIfBound("No Next Wall.");
	}
	else {
		WallIndex++;
		FString Message = "Moved to Next Wall : " + FString::FromInt(WallIndex + 1);
		TriggerNotification.ExecuteIfBound(Message);
	}
}

//Spline Wall Builder End

void AInteractiveArchController::ToggleToWall() {
	SelectionWidget->SetVisibility(ESlateVisibility::Hidden);
	WallWidget->SetVisibility(ESlateVisibility::Visible);
	ToggleMeshGeneratorInput = false;
	ToggleWallBuilderInput = true;
	SetupInputComponent();
}

void AInteractiveArchController::ToggleToMeshGenerator() {
	SelectionWidget->SetVisibility(ESlateVisibility::Visible);
	WallWidget->SetVisibility(ESlateVisibility::Hidden);
	ToggleMeshGeneratorInput = true;
	ToggleWallBuilderInput = false;
	SetupInputComponent();
}

void AInteractiveArchController::SpawnCamera(bool ChangeCameraType)
{

	if (CurrentlySpawnedCamera)
	{
		CurrentlySpawnedCamera->Destroy();
		CurrentlySpawnedCamera = nullptr;
		if(ChangeCameraType)
			CurrentCameraIndex++;
	}

	if (MyDataTable)
	{
		TArray<FName> RowNames = MyDataTable->GetRowNames();

		if (RowNames.Num() == CurrentCameraIndex) {
			CurrentCameraIndex = 0;
		}

		FName RowNameByIndex = RowNames[CurrentCameraIndex];

		FCameraTypeStruct* RowData = MyDataTable->FindRow<FCameraTypeStruct>(RowNameByIndex, TEXT(""));

		if (RowData)
		{

			TSubclassOf<APawn> CameraClassToSpawn = RowData->CharacterReference;
			if (CameraClassToSpawn)
			{
				FActorSpawnParameters SpawnParams;

				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

				FVector SpawnLocation_ = ClickLocation + 50;
				FRotator SpawnRotation = FRotator(0, 0, 0);

				APawn* SpawnedPawn = GetWorld()->SpawnActor<APawn>(CameraClassToSpawn, SpawnLocation_, SpawnRotation, SpawnParams);

				if (SpawnedPawn)
				{
					Possess(SpawnedPawn);

					CurrentlySpawnedCamera= SpawnedPawn;

					if (RowData->CharacterType == ECameraType::Orthographic) {
						GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, FString("Orthographic Camera Spawned"));
					}
					else if (RowData->CharacterType == ECameraType::Isometric) {
						GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, FString("Isometric Camera Spawned"));
					}
					else {
						GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, FString("Perspective Camera Spawned"));
					}
				}
				else
				{
					GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, FString("Failed to spawn actor"));
				}
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, FString("Character class to spawn is invalid"));
			}
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, FString("Failed to find row data in DataTable"));
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, FString("Failed to load DataTable"));
	}
	SetupInputComponent();
}