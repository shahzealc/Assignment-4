// Fill out your copyright notice in the Description page of Project Settings.

#include "StaircaseActor.h"

AStaircaseActor::AStaircaseActor()
{
	PrimaryActorTick.bCanEverTick = true;

	NumberOfStairs = 5;
	Width = 1.0f;
	Depth = 3.5f;
	Height = 0.35f;
	StepHeight = 30.0f;
	StepRelativeWidth = 40.0f;
	StairType = TypeOfStair::Close;
	AddRailings = false;

	scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Component"));
	this->SetRootComponent(scene);
	StairStaticMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));

}

void AStaircaseActor::OnConstruction(const FTransform& Ftransform)
{

	ClearStairArray(StairArray);


	for (int i = 0; i < NumberOfStairs; i++) {

		float StairX = StairStaticMesh->GetBounds().GetBox().GetSize().X;
		float StairY = StairStaticMesh->GetBounds().GetBox().GetSize().Y;
		float StairZ = StairStaticMesh->GetBounds().GetBox().GetSize().Z;

		FString StairName = "Stair" + FString::FromInt(i);
		Stair = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), *StairName);
		Stair->AttachToComponent(scene, FAttachmentTransformRules::KeepRelativeTransform);
		Stair->RegisterComponentWithWorld(GetWorld());
		Stair->SetStaticMesh(StairStaticMesh);
		Stair->SetMaterial(0, StairsMaterial);

		if (AddRailings) {

			FString RightRailingName = "RightRailing" + FString::FromInt(i);
			RightRailing = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), *RightRailingName);
			RightRailing->AttachToComponent(Stair, FAttachmentTransformRules::KeepRelativeTransform);
			RightRailing->RegisterComponentWithWorld(GetWorld());
			RightRailing->SetRelativeLocation({ 0,(StairY / 2),100 });
			RightRailing->SetStaticMesh(StairStaticMesh);
			RightRailing->SetMaterial(0, RailingMaterial);

			FString LeftRailingName = "LeftRailing" + FString::FromInt(i);
			LeftRailing = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), *LeftRailingName);
			LeftRailing->AttachToComponent(Stair, FAttachmentTransformRules::KeepRelativeTransform);
			LeftRailing->RegisterComponentWithWorld(GetWorld());
			LeftRailing->SetRelativeLocation({ 0,-(StairY / 2),100 });
			LeftRailing->SetStaticMesh(StairStaticMesh);
			LeftRailing->SetMaterial(0, RailingMaterial);
			
		}


		if (StairType == TypeOfStair::Box) {

			FVector scaleStair(Width, Depth, 0.25 + Height * (i + 1));
			Stair->SetRelativeScale3D(scaleStair);
			Stair->SetRelativeLocation({ i * StairX,0.0f,0.0f });

		}
		else {

			FVector scaleStair(Width, Depth, Height);
			Stair->SetRelativeScale3D(scaleStair);

			if (StairType == TypeOfStair::Close) {
				Stair->SetRelativeLocation({ i * StairX,0.0f,(i * StepHeight) });
			}
			else {
				Stair->SetRelativeLocation({ i * StairX,0.0f,(i * (StepHeight + 20)) });
			}
			
		}

		if (AddRailings) {
			RightRailing->SetWorldScale3D({ (Stair->GetRelativeScale3D().X) / 4, 0.1f / 2, 2 });
			LeftRailing->SetWorldScale3D({ (Stair->GetRelativeScale3D().X) / 4, 0.1f / 2, 2 });
		}

		FStairStaticMeshStruct StairMesh;
		StairMesh.Stairs = Stair;
		if (AddRailings) {
			StairMesh.LeftRailing = LeftRailing;
			StairMesh.RightRailing = RightRailing;
		}
		else {
			StairMesh.LeftRailing = nullptr;
			StairMesh.RightRailing = nullptr;
		}

		StairArray.Add(StairMesh);

	}

}

void AStaircaseActor::BeginPlay()
{
	Super::BeginPlay();

}

void AStaircaseActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AStaircaseActor::ClearStairArray(TArray<FStairStaticMeshStruct> StairArray_) {

	for (int i = 0; i < StairArray_.Num(); i++) {
		if (StairArray_[i].Stairs) {
			StairArray_[i].Stairs->DestroyComponent();
			StairArray_[i].Stairs = nullptr;
		}
		if (StairArray_[i].RightRailing) {
			StairArray_[i].RightRailing->DestroyComponent();
			StairArray_[i].RightRailing = nullptr;
		}
		if (StairArray_[i].LeftRailing) {
			StairArray_[i].LeftRailing->DestroyComponent();
			StairArray_[i].LeftRailing = nullptr;
		}
	}

	StairArray_.Empty();

}

