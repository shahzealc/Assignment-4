// Fill out your copyright notice in the Description page of Project Settings.


#include "DynamicPawnController.h"

void ADynamicPawnController::BeginPlay()
{
	Super::BeginPlay();
	MyDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Script/Engine.DataTable'/Game/Assignment2/DataTable/PawnDataTable_.PawnDataTable_'"));
	SpawnCharacter();
}

void ADynamicPawnController::SpawnCharacter()
{

	if (CurrentlySpawnedActor)
	{
		CurrentlySpawnedActor->Destroy();
		CurrentlySpawnedActor = nullptr;
		CurrentActorIndex++;
		SetShowMouseCursor(false);
	}

	if (MyDataTable)
	{
		TArray<FName> RowNames = MyDataTable->GetRowNames();

		if (RowNames.Num() == CurrentActorIndex) {
			CurrentActorIndex = 0;
		}

		FName RowNameByIndex = RowNames[CurrentActorIndex];

		FPawnAttributesStruct* RowData = MyDataTable->FindRow<FPawnAttributesStruct>(RowNameByIndex, TEXT(""));

		if (RowData)
		{

			TSubclassOf<APawn> CharacterClassToSpawn = RowData->CharacterReference;
			if (CharacterClassToSpawn)
			{
				FActorSpawnParameters SpawnParams;

				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

				FVector SpawnLocation_ = FVector(50, 50, 50);
				FRotator SpawnRotation = FRotator(0, 0, 0);

				APawn* SpawnedPawn = GetWorld()->SpawnActor<APawn>(CharacterClassToSpawn, SpawnLocation_, SpawnRotation, SpawnParams);

				if (SpawnedPawn)
				{
					Possess(SpawnedPawn);

					CurrentlySpawnedActor = SpawnedPawn;

					if (RowData->CharacterType == EPawnType::TopDown) {
						SetShowMouseCursor(true);
						DisplayAttributes((Cast<ABaseCharacter>(SpawnedPawn))->PawnAttributeAsset);
					}
					else if (RowData->CharacterType == EPawnType::ThirdPerson) {
						DisplayAttributes((Cast<ABaseCharacter>(SpawnedPawn))->PawnAttributeAsset);
					}
					else {
						DisplayAttributes((Cast<AFirstPersonPawn>(SpawnedPawn))->PawnAttributeAsset);
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
}


void ADynamicPawnController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("Spawn", IE_Pressed, this, &ADynamicPawnController::SpawnCharacter);
}
