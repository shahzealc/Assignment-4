// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectionArea.h"

// Sets default values
ASelectionArea::ASelectionArea()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Material = LoadObject<UMaterialInterface>(nullptr, TEXT("/Script/Engine.Material'/MeshModelingToolsetExp/Materials/SculptMaterial_Transparent.SculptMaterial_Transparent'"));

	ProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProceduralMesh"));
	RootComponent = ProceduralMesh;

	ProceduralMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ProceduralMesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	ProceduralMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
}

void ASelectionArea::GenerateSphere(float InRadius, int32 Segments, int32 Rings)
{

	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FLinearColor> Colors;
	TArray<FVector2D> UVs;
	TArray<FProcMeshTangent> Tangents;


	Vertices.Empty();
	Triangles.Empty();
	Normals.Empty();
	UVs.Empty();
	Tangents.Empty();
	FVector Offset = FVector(0, 0, 0);

	for (int32 RingNumber = 0; RingNumber <= Rings; RingNumber++)
	{
		for (int32 SegmentNumber = 0; SegmentNumber <= Segments; SegmentNumber++)
		{
			float Theta = 2.0f * PI * (float)SegmentNumber / (float)Segments;
			float Phi = PI * (float)RingNumber / (float)Rings;

			float X = FMath::Sin(Phi) * FMath::Cos(Theta);
			float Y = FMath::Sin(Phi) * FMath::Sin(Theta);
			float Z = FMath::Cos(Phi);

			FVector Vertex = FVector(X, Y, Z) * InRadius + Offset;
			Vertices.Add(Vertex);

			FVector Normal = Vertex.GetSafeNormal();
			Normals.Add(Normal);

			FVector2D UV = FVector2D((float)SegmentNumber / (float)Segments, (float)RingNumber / (float)Rings);
			UVs.Add(UV);

			FProcMeshTangent Tangent = FProcMeshTangent(1.f, 0.f, 0.f);
			Tangents.Add(Tangent);
		}
	}

	for (int32 RingNumber = 0; RingNumber < Rings; RingNumber++)
	{
		for (int32 SegmentNumber = 0; SegmentNumber < Segments; SegmentNumber++)
		{
			int32 FirstVertex = (RingNumber * (Segments + 1)) + SegmentNumber;
			int32 SecondVertex = FirstVertex + Segments + 1;

			Triangles.Add(FirstVertex);
			Triangles.Add(FirstVertex + 1);
			Triangles.Add(SecondVertex);

			Triangles.Add(SecondVertex);
			Triangles.Add(FirstVertex + 1);
			Triangles.Add(SecondVertex + 1);
		}
	}

	ProceduralMesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UVs, Colors, Tangents, true);
	ProceduralMesh->SetMaterial(0, Material);
}

void ASelectionArea::GenerateBox(float Dimention) {

	ProceduralMesh->ClearAllMeshSections();
	TArray<FVector> Vertices_Arr;
	TArray<int32> Triangles_Arr;
	TArray<FVector> Normals_Arr;
	TArray<FVector2D> UVs_Arr;
	TArray<FProcMeshTangent> Tangents_Arr;
	TArray<FLinearColor> Colors_Arr;

	const int32 VertexCount = 6 * 4; // 6 sides on a Box, 4 verts each
	const int32 TriangleCount = 6 * 2 * 3; // 2x Triangles_Arr per Box side, 3 verts each

	if (VertexCount != Vertices_Arr.Num())
	{
		Vertices_Arr.Empty();
		Vertices_Arr.AddUninitialized(VertexCount);
		Normals_Arr.Empty();
		Normals_Arr.AddUninitialized(VertexCount);
		Tangents_Arr.Empty();
		Tangents_Arr.AddUninitialized(VertexCount);
		UVs_Arr.Empty();
		UVs_Arr.AddUninitialized(VertexCount);
	}

	if (TriangleCount != Triangles_Arr.Num())
	{
		Triangles_Arr.Empty();
		Triangles_Arr.AddUninitialized(TriangleCount);
	}

	const float OffsetX = Dimention / 2.0f;
	const float OffsetY = Dimention / 2.0f;
	const float OffsetZ = Dimention / 2.0f;

	// Define the 8 corners of the Box
	const FVector P0 = FVector(OffsetX, OffsetY, -OffsetZ);
	const FVector P1 = FVector(OffsetX, -OffsetY, -OffsetZ);
	const FVector P2 = FVector(OffsetX, -OffsetY, OffsetZ);
	const FVector P3 = FVector(OffsetX, OffsetY, OffsetZ);
	const FVector P4 = FVector(-OffsetX, OffsetY, -OffsetZ);
	const FVector P5 = FVector(-OffsetX, -OffsetY, -OffsetZ);
	const FVector P6 = FVector(-OffsetX, -OffsetY, OffsetZ);
	const FVector P7 = FVector(-OffsetX, OffsetY, OffsetZ);

	// Now we create 6x faces, 4 vertices each
	int32 VertexOffset = 0;
	int32 TriangleOffset = 0;
	FVector Normal;
	FVector Tangent;

	// Front (+X) face: 0-1-2-3
	Normal = FVector(1, 0, 0);
	Tangent = FVector(0, 1, 0);
	BuildQuad(Vertices_Arr, Triangles_Arr, Normals_Arr, Tangents_Arr, UVs_Arr, P0, P1, P2, P3, VertexOffset, TriangleOffset, Normal, Tangent);

	// Back (-X) face: 5-4-7-6
	Normal = FVector(-1, 0, 0);
	Tangent = FVector(0, -1, 0);
	BuildQuad(Vertices_Arr, Triangles_Arr, Normals_Arr, Tangents_Arr, UVs_Arr, P5, P4, P7, P6, VertexOffset, TriangleOffset, Normal, Tangent);

	// Left (-Y) face: 1-5-6-2
	Normal = FVector(0, -1, 0);
	Tangent = FVector(1, 0, 0);
	BuildQuad(Vertices_Arr, Triangles_Arr, Normals_Arr, Tangents_Arr, UVs_Arr, P1, P5, P6, P2, VertexOffset, TriangleOffset, Normal, Tangent);

	// Right (+Y) face: 4-0-3-7
	Normal = FVector(0, 1, 0);
	Tangent = FVector(-1, 0, 0);
	BuildQuad(Vertices_Arr, Triangles_Arr, Normals_Arr, Tangents_Arr, UVs_Arr, P4, P0, P3, P7, VertexOffset, TriangleOffset, Normal, Tangent);

	// Top (+Z) face: 6-7-3-2
	Normal = FVector(0, 0, 1);
	Tangent = FVector(1, 0, 0);
	BuildQuad(Vertices_Arr, Triangles_Arr, Normals_Arr, Tangents_Arr, UVs_Arr, P6, P7, P3, P2, VertexOffset, TriangleOffset, Normal, Tangent);

	// Bottom (-Z) face: 1-0-4-5
	Normal = FVector(0, 0, -1);
	Tangent = FVector(-1, 0, 0);
	BuildQuad(Vertices_Arr, Triangles_Arr, Normals_Arr, Tangents_Arr, UVs_Arr, P1, P0, P4, P5, VertexOffset, TriangleOffset, Normal, Tangent);

	Colors_Arr.Init(FLinearColor(1, 1, 1, 1), Vertices_Arr.Num());

	ProceduralMesh->CreateMeshSection_LinearColor(0, Vertices_Arr, Triangles_Arr, Normals_Arr, UVs_Arr, Colors_Arr, Tangents_Arr, true);
	if (Material)
		ProceduralMesh->SetMaterial(0, Material);
}

void ASelectionArea::BuildQuad(TArray<FVector>& InVertices, TArray<int32>& InTriangles, TArray<FVector>& InNormals, TArray<FProcMeshTangent>& InTangents, TArray<FVector2D>& InTexCoords, const FVector BottomLeft, const FVector BottomRight, const FVector TopRight, const FVector TopLeft, int32& VertexOffset, int32& TriangleOffset, const FVector Normal, const FVector Tangent)
{
	const int32 Index0 = VertexOffset++;
	const int32 Index1 = VertexOffset++;
	const int32 Index2 = VertexOffset++;
	const int32 Index3 = VertexOffset++;
	InVertices[Index0] = BottomLeft;
	InVertices[Index1] = BottomRight;
	InVertices[Index2] = TopRight;
	InVertices[Index3] = TopLeft;
	InTexCoords[Index0] = FVector2D(0.0f, 1.0f);
	InTexCoords[Index1] = FVector2D(1.0f, 1.0f);
	InTexCoords[Index2] = FVector2D(1.0f, 0.0f);
	InTexCoords[Index3] = FVector2D(0.0f, 0.0f);
	InTriangles[TriangleOffset++] = Index0;
	InTriangles[TriangleOffset++] = Index1;
	InTriangles[TriangleOffset++] = Index2;
	InTriangles[TriangleOffset++] = Index2;
	InTriangles[TriangleOffset++] = Index3;
	InTriangles[TriangleOffset++] = Index0;
	// On a Box side, all the vertex normals face the same way
	InNormals[Index0] = InNormals[Index1] = InNormals[Index2] = InNormals[Index3] = Normal;
	InTangents[Index0] = InTangents[Index1] = InTangents[Index2] = InTangents[Index3] = FProcMeshTangent(Tangent, false);
}

// Called when the game starts or when spawned
void ASelectionArea::BeginPlay()
{
	Super::BeginPlay();
	GenerateBox(200);
}

// Called every frame
void ASelectionArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASelectionArea::ToggleShape(const FString& ToggletoShape) {
	if (ToggletoShape == "Box") {
		GenerateBox(200);
	}
	else if(ToggletoShape == "Sphere"){
		GenerateSphere(100, 50, 50);
	}
}