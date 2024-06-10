// Fill out your copyright notice in the Description page of Project Settings.


#include "VerticalRailActor.h"

AVerticalRailActor::AVerticalRailActor()
{
	PrimaryActorTick.bCanEverTick = true;

	VerticalRailingMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("VerticalRailing"));
	RootComponent = VerticalRailingMesh;

	VerticalRailingMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("/Script/Engine.Material'/Game/StarterContent/Materials/M_Wood_Walnut.M_Wood_Walnut'"));
	SphereMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("/Script/Engine.Material'/Game/StarterContent/Materials/M_Wood_Walnut.M_Wood_Walnut'"));
	PyramidMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("/Script/Engine.Material'/Game/StarterContent/Materials/M_Wood_Walnut.M_Wood_Walnut'"));
	TopHorizontalRailingMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("/Script/Engine.Material'/Game/StarterContent/Materials/M_Wood_Walnut.M_Wood_Walnut'"));
	BottomHorizontalRailingMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("/Script/Engine.Material'/Game/StarterContent/Materials/M_Wood_Walnut.M_Wood_Walnut'"));
	StandMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("/Script/Engine.Material'/Game/StarterContent/Materials/M_Wood_Walnut.M_Wood_Walnut'"));
	SphereConeMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("/Script/Engine.Material'/Game/StarterContent/Materials/M_Wood_Walnut.M_Wood_Walnut'"));
	RectangleMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("/Script/Engine.Material'/Game/StarterContent/Materials/M_Wood_Walnut.M_Wood_Walnut'"));
	OvalMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("/Script/Engine.Material'/Game/StarterContent/Materials/M_Wood_Walnut.M_Wood_Walnut'"));
	RoundedOverTopMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("/Script/Engine.Material'/Game/StarterContent/Materials/M_Wood_Walnut.M_Wood_Walnut'"));
}

void AVerticalRailActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	GenerateVerticalRail();

	switch (RailingType)
	{
	case ERailingType::Pyramid:
		GeneratePyramid(Length, Width, 10);
		break;
	case ERailingType::Sphere:
		GenerateSphere(Length / 2, 25, 25);
		GenerateStand(FVector(0, 0, Height / 2), Width/2, 4, Width/4, 1, 10, 10);
		break;
	case ERailingType::Oval:
		GenerateOval(Length / 2, 25, 25);
		GenerateStand(FVector(0, 0, Height / 2), Width/2, 4, Width/4, 1, 10, 10);
		break;
	case ERailingType::SphereWithCone:
		GenerateSphere(Length / 2, 25, 25);
		GenerateSphereWithCone(Length-2, Width, 12);
		GenerateStand(FVector(0, 0, Height / 2), Width / 2, 4, Width / 4, 1, 10, 10);
		break;
	case ERailingType::Rectangle:
		GenerateRectangle();
		break;
	case ERailingType::RoundedOverTop:
		GenerateRectangle();
		GenerateRoundedOverTop((Length / 2)-2, 25, 25);
		break;
	case ERailingType::GlobeWithStick:
		GenerateRoundedOverTop((Length / 2), 25, 25);
		break;
	default:
		break;
	}
	
	GenerateTopHorizontalRailingMesh();
	GenerateBottomHorizontalRailingMesh();
}

void AVerticalRailActor::GenerateVerticalRail() {

	VerticalRailingMesh->ClearAllMeshSections();

	const int32 VertexCount = 6 * 4; // 6 sides on a cube, 4 verts each
	const int32 TriangleCount = 6 * 2 * 3; // 2x Triangles_Arr per cube side, 3 verts each

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

	const float OffsetX = Length / 2.0f;
	const float OffsetY = Width / 2.0f;
	const float OffsetZ = Height / 2.0f;

	// Define the 8 corners of the cube
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

	VerticalRailingMesh->CreateMeshSection_LinearColor(0, Vertices_Arr, Triangles_Arr, Normals_Arr, UVs_Arr, Colors_Arr, Tangents_Arr, true);
	VerticalRailingMesh->SetMaterial(0, VerticalRailingMaterial);
}

void AVerticalRailActor::GenerateTopHorizontalRailingMesh() {

	const int32 VertexCount = 6 * 4; // 6 sides on a cube, 4 verts each
	const int32 TriangleCount = 6 * 2 * 3; // 2x Triangles_Arr per cube side, 3 verts each

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

	const float OffsetX = Height / 2.0f;
	const float OffsetY = Width / 2.0f;
	const float OffsetZ = 7;

	// Define the 8 corners of the cube
	const FVector P0 = FVector(OffsetX, OffsetY - Width, -OffsetZ + (Height/4));
	const FVector P1 = FVector(OffsetX, -OffsetY - Width, -OffsetZ + (Height/4));
	const FVector P2 = FVector(OffsetX, -OffsetY - Width, OffsetZ + (Height/4));
	const FVector P3 = FVector(OffsetX, OffsetY - Width, OffsetZ + (Height/4));
	const FVector P4 = FVector(-OffsetX, OffsetY - Width, -OffsetZ + (Height/4));
	const FVector P5 = FVector(-OffsetX, -OffsetY - Width, -OffsetZ + (Height/4));
	const FVector P6 = FVector(-OffsetX, -OffsetY - Width, OffsetZ + (Height/4));
	const FVector P7 = FVector(-OffsetX, OffsetY - Width, OffsetZ + (Height/4));

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

	VerticalRailingMesh->CreateMeshSection_LinearColor(2, Vertices_Arr, Triangles_Arr, Normals_Arr, UVs_Arr, Colors_Arr, Tangents_Arr, true);
	VerticalRailingMesh->SetMaterial(2, TopHorizontalRailingMaterial);
}

void AVerticalRailActor::GenerateBottomHorizontalRailingMesh() {

	const int32 VertexCount = 6 * 4; // 6 sides on a cube, 4 verts each
	const int32 TriangleCount = 6 * 2 * 3; // 2x Triangles_Arr per cube side, 3 verts each

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

	const float OffsetX = Height / 2.0f;
	const float OffsetY = Width / 2.0f;
	const float OffsetZ = 7;

	// Define the 8 corners of the cube
	const FVector P0 = FVector(OffsetX, OffsetY - Width, -OffsetZ - (Height / 4));
	const FVector P1 = FVector(OffsetX, -OffsetY - Width, -OffsetZ - (Height / 4));
	const FVector P2 = FVector(OffsetX, -OffsetY - Width, OffsetZ - (Height / 4));
	const FVector P3 = FVector(OffsetX, OffsetY - Width, OffsetZ - (Height / 4));
	const FVector P4 = FVector(-OffsetX, OffsetY - Width, -OffsetZ - (Height / 4));
	const FVector P5 = FVector(-OffsetX, -OffsetY - Width, -OffsetZ - (Height / 4));
	const FVector P6 = FVector(-OffsetX, -OffsetY - Width, OffsetZ - (Height / 4));
	const FVector P7 = FVector(-OffsetX, OffsetY - Width, OffsetZ - (Height/4));

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

	VerticalRailingMesh->CreateMeshSection_LinearColor(3, Vertices_Arr, Triangles_Arr, Normals_Arr, UVs_Arr, Colors_Arr, Tangents_Arr, true);
	VerticalRailingMesh->SetMaterial(3, BottomHorizontalRailingMaterial);
}

void AVerticalRailActor::BuildQuad(TArray<FVector>& InVertices, TArray<int32>& InTriangles, TArray<FVector>& InNormals, TArray<FProcMeshTangent>& InTangents, TArray<FVector2D>& InTexCoords, const FVector BottomLeft, const FVector BottomRight, const FVector TopRight, const FVector TopLeft, int32& VertexOffset, int32& TriangleOffset, const FVector Normal, const FVector Tangent)
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
	// On a cube side, all the vertex normals face the same way
	InNormals[Index0] = InNormals[Index1] = InNormals[Index2] = InNormals[Index3] = Normal;
	InTangents[Index0] = InTangents[Index1] = InTangents[Index2] = InTangents[Index3] = FProcMeshTangent(Tangent, false);
}

void AVerticalRailActor::GeneratePyramid(float BaseLength, float BaseWidth, float Height_)
{
	TArray<FVector> Vertices;

	float L = (BaseLength / 2.0f);
	float W = BaseWidth / 2.0f;
	float H = Height_;

	Vertices.Add(FVector(-L, -W, 0+(Height/2))); //0
	Vertices.Add(FVector(L, -W, 0+(Height/2))); //1
	Vertices.Add(FVector(L, W, 0+(Height/2))); //2
	Vertices.Add(FVector(-L, W, 0+(Height/2))); //3

	//front
	Vertices.Add(FVector(-L, W, 0+(Height/2)));//4
	Vertices.Add(FVector(0, 0, H+(Height/2))); //5
	Vertices.Add(FVector(-L, -W, 0+(Height/2))); //6

	//left
	Vertices.Add(FVector(-L, -W, 0+(Height/2))); //7
	Vertices.Add(FVector(0, 0, H+(Height/2))); //8
	Vertices.Add(FVector(L, -W, 0+(Height/2)));//9

	//back
	Vertices.Add(FVector(L, -W, 0+(Height/2))); //10
	Vertices.Add(FVector(0, 0, H+(Height/2))); //11
	Vertices.Add(FVector(L, W, 0+(Height/2)));//12

	//right
	Vertices.Add(FVector(L, W, 0+(Height/2))); //13
	Vertices.Add(FVector(0, 0, H+(Height/2))); //14
	Vertices.Add(FVector(-L, W, 0+(Height/2)));//15
	// Triangles

	TArray<int32> Triangles = { 0,1,3, 1,2,3 ,4,5,6,7,8,9,10,11,12,13,14,15 };

	TArray<FVector2D>  UVs = { FVector2D(0,0), FVector2D(0,1), FVector2D(1,1), FVector2D(1,0),
			FVector2D(1,1),FVector2D(0,0.5),FVector2D(0,0),
			FVector2D(1,1),FVector2D(0,0.5),FVector2D(0,0),
			FVector2D(1,1),FVector2D(0,0.5),FVector2D(0,0),
			FVector2D(1,1),FVector2D(0,0.5),FVector2D(0,0),
	};
	VerticalRailingMesh->CreateMeshSection_LinearColor(1, Vertices, Triangles, TArray<FVector>(), UVs, TArray<FLinearColor>(), TArray<FProcMeshTangent>(), true);

	VerticalRailingMesh->SetMaterial(1, PyramidMaterial);

}

void AVerticalRailActor::GenerateSphere(float Radius, int32 Segments, int32 Rings)
{

	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UVs;
	TArray<FProcMeshTangent> Tangents;
	TArray<FLinearColor> Colors;

	for (int r = 0; r < Rings; ++r)
	{
		float phi = PI * r / (Rings + 1);
		for (int s = 0; s < Segments; ++s)
		{	
			float theta = 2 * PI * s / Segments;
			FVector pos = FVector(Radius * FMath::Sin(phi) * FMath::Cos(theta), Radius * FMath::Sin(phi) * FMath::Sin(theta), Radius * FMath::Cos(phi)+(Height/2)+Radius+4);
			Vertices.Add(pos);
			Normals.Add(pos.GetSafeNormal());
			UVs.Add(FVector2D(s / (float)Segments, r / (float)(Rings + 1)));
			FVector tangent = FVector(-FMath::Sin(theta), FMath::Cos(theta), 0).GetSafeNormal();
			Tangents.Add(FProcMeshTangent(tangent, false));
			Colors.Add(FLinearColor(1, 1, 1, 1));

			// We only add extra vertices at the seam on the first ring iteration
			if (s == Segments) {
				// Duplicate the first vertex of each ring at the seam for seamless UVs
				Vertices.Add(Vertices[1 + (r - 1) * (Segments + 1)]);
				Normals.Add(Normals[1 + (r - 1) * (Segments + 1)]);
				UVs.Add(FVector2D(1.f, (float)r / (Rings + 1)));
				Tangents.Add(Tangents[1 + (r - 1) * (Segments + 1)]);
				Colors.Add(FLinearColor(1, 1, 1, 1));
			}
		}
	}

	// Bottom vertex
	Vertices.Add(FVector(0, 0, -Radius));
	Normals.Add(FVector(0, 0, -1));
	UVs.Add(FVector2D(0.5f, 1));
	Tangents.Add(FProcMeshTangent(FVector(1, 0, 0), false));
	Colors.Add(FLinearColor(1, 1, 1, 1));

	// Top Cap
	for (int s = 0; s < Segments; ++s)
	{
		Triangles.Add(0);
		Triangles.Add(1 + s);
		Triangles.Add(1 + (s + 1) % Segments);
	}

	// Body
	for (int r = 0; r < Rings; ++r)
	{
		for (int s = 0; s < Segments; ++s)
		{
			int current = 1 + r * Segments + s;
			int next = current + 1;
			if (s == Segments - 1) next = 1 + r * Segments;  // Wrap around

			int below = current + Segments;
			int belowNext = next + Segments;

			Triangles.Add(current);
			Triangles.Add(next);
			Triangles.Add(belowNext);

			Triangles.Add(current);
			Triangles.Add(belowNext);
			Triangles.Add(below);
		}
	}

	// Bottom Cap
	int bottomIndex = Vertices.Num() - 1;
	int firstOfLastRing = 1 + (Rings - 1) * Segments;
	for (int s = 0; s < Segments; ++s)
	{
		Triangles.Add(bottomIndex);
		Triangles.Add(firstOfLastRing + (s + 1) % Segments);
		Triangles.Add(firstOfLastRing + s);
	}

	VerticalRailingMesh->CreateMeshSection_LinearColor(1, Vertices, Triangles, Normals, UVs, Colors, Tangents, true);
	VerticalRailingMesh->SetMaterial(1, SphereMaterial);
	//SphereMesh->SetRelativeLocation({ 0,0,(Height +Length)/ 2 });

}

void AVerticalRailActor::GenerateOval(float Radius, int32 Segments, int32 Rings)
{

	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UVs;
	TArray<FProcMeshTangent> Tangents;
	TArray<FLinearColor> Colors;
	float OvalHeight = Radius + (Radius / 2);

	for (int r = 0; r < Rings; ++r)
	{
		float phi = PI * r / (Rings + 1);
		for (int s = 0; s < Segments; ++s)
		{
			float theta = 2 * PI * s / Segments;
			FVector pos = FVector(Radius * FMath::Sin(phi) * FMath::Cos(theta), Radius * FMath::Sin(phi) * FMath::Sin(theta), OvalHeight * FMath::Cos(phi) + (Height / 2) + Radius+4);
			Vertices.Add(pos);
			Normals.Add(pos.GetSafeNormal());
			UVs.Add(FVector2D(s / (float)Segments, r / (float)(Rings + 1)));
			FVector tangent = FVector(-FMath::Sin(theta), FMath::Cos(theta), 0).GetSafeNormal();
			Tangents.Add(FProcMeshTangent(tangent, false));
			Colors.Add(FLinearColor(1, 1, 1, 1));

			// We only add extra vertices at the seam on the first ring iteration
			if (s == Segments) {
				// Duplicate the first vertex of each ring at the seam for seamless UVs
				Vertices.Add(Vertices[1 + (r - 1) * (Segments + 1)]);
				Normals.Add(Normals[1 + (r - 1) * (Segments + 1)]);
				UVs.Add(FVector2D(1.f, (float)r / (Rings + 1)));
				Tangents.Add(Tangents[1 + (r - 1) * (Segments + 1)]);
				Colors.Add(FLinearColor(1, 1, 1, 1));
			}
		}
	}

	// Bottom vertex
	Vertices.Add(FVector(0, 0, -Radius));
	Normals.Add(FVector(0, 0, -1));
	UVs.Add(FVector2D(0.5f, 1));
	Tangents.Add(FProcMeshTangent(FVector(1, 0, 0), false));
	Colors.Add(FLinearColor(1, 1, 1, 1));

	// Top Cap
	for (int s = 0; s < Segments; ++s)
	{
		Triangles.Add(0);
		Triangles.Add(1 + s);
		Triangles.Add(1 + (s + 1) % Segments);
	}

	// Body
	for (int r = 0; r < Rings; ++r)
	{
		for (int s = 0; s < Segments; ++s)
		{
			int current = 1 + r * Segments + s;
			int next = current + 1;
			if (s == Segments - 1) next = 1 + r * Segments;  // Wrap around

			int below = current + Segments;
			int belowNext = next + Segments;

			Triangles.Add(current);
			Triangles.Add(next);
			Triangles.Add(belowNext);

			Triangles.Add(current);
			Triangles.Add(belowNext);
			Triangles.Add(below);
		}
	}

	// Bottom Cap
	int bottomIndex = Vertices.Num() - 1;
	int firstOfLastRing = 1 + (Rings - 1) * Segments;
	for (int s = 0; s < Segments; ++s)
	{
		Triangles.Add(bottomIndex);
		Triangles.Add(firstOfLastRing + (s + 1) % Segments);
		Triangles.Add(firstOfLastRing + s);
	}

	VerticalRailingMesh->CreateMeshSection_LinearColor(1, Vertices, Triangles, Normals, UVs, Colors, Tangents, true);
	VerticalRailingMesh->SetMaterial(1, OvalMaterial);
	//SphereMesh->SetRelativeLocation({ 0,0,(Height +Length)/ 2 });

}

void AVerticalRailActor::GenerateStand(FVector Location, float BaseRadius, float Height1, float RimRadius, float CurvatureFactor, int NumSlices, int NumStacks)
{
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UVs;
	TArray<FProcMeshTangent> Tangents;
	TArray<FLinearColor> Colors;

	float Theta, Phi;
	float DeltaTheta = 2.0f * PI / NumSlices;
	float DeltaPhi = PI / (NumStacks - 1);

	// Generate vertices
	for (int i = 0; i < NumStacks; i++)
	{
		Phi = i * DeltaPhi;

		// Bell profile function for radius interpolation with curvature
		float t = Phi / (PI / 2.0f);
		float CurrentRadius = FMath::Lerp(BaseRadius, RimRadius, FMath::Sin(t * CurvatureFactor)) + 2.5;

		for (int j = 0; j < NumSlices; j++)
		{
			Theta = j * DeltaTheta;

			float X = CurrentRadius * FMath::Sin(Theta);
			float Y = CurrentRadius * FMath::Cos(Theta);
			float Z = Height1 * (t - 1.0f); // Z decreases with height for bell shape


			// Apply location offset
			X += Location.X;
			Y += Location.Y;
			Z += Location.Z;

			// Invert Z-coordinate


			Vertices.Add(FVector(X, Y, Z + Height1));

			// Calculate normals for lighting (approximate)
			Normals.Add(FVector(X, Y, Z).GetSafeNormal());

			// UV mapping
			UVs.Add(FVector2D((float)j / (NumSlices - 1), (float)i / (NumStacks - 1)));

			// Tangents (optional)
			Tangents.Add(FProcMeshTangent(-FMath::Sin(Theta), FMath::Cos(Theta), 0.0f));
		}
	}

	// Generate triangles (quads converted to triangles)
	for (int i = 0; i < NumStacks - 1; i++)
	{
		for (int j = 0; j < NumSlices - 1; j++)
		{
			int BottomLeft = i * NumSlices + j;
			int BottomRight = BottomLeft + 1;
			int TopLeft = BottomLeft + NumSlices;
			int TopRight = TopLeft + 1;

			// First triangle (bottom left to top left to top right)
			Triangles.Add(BottomLeft);
			Triangles.Add(TopLeft);
			Triangles.Add(BottomRight);

			Triangles.Add(BottomLeft);
			Triangles.Add(BottomRight);
			Triangles.Add(TopLeft);

			// Second triangle (top left to top right to bottom right)
			Triangles.Add(TopLeft);
			Triangles.Add(TopRight);
			Triangles.Add(BottomRight);

			Triangles.Add(TopLeft);
			Triangles.Add(BottomRight);
			Triangles.Add(TopRight);
		}

		// Handle wrap-around triangle for each stack
		int BottomLeftWrap = i * NumSlices + (NumSlices - 1);
		int BottomRightWrap = i * NumSlices;
		int TopLeftWrap = BottomLeftWrap + NumSlices;
		int TopRightWrap = BottomRightWrap + NumSlices;

		// First triangle (bottom left to top left to top right)
		Triangles.Add(BottomLeftWrap);
		Triangles.Add(TopLeftWrap);
		Triangles.Add(BottomRightWrap);

		Triangles.Add(BottomLeftWrap);
		Triangles.Add(BottomRightWrap);
		Triangles.Add(TopLeftWrap);

		// Second triangle (top left to top right to bottom right)
		Triangles.Add(TopLeftWrap);
		Triangles.Add(TopRightWrap);
		Triangles.Add(BottomRightWrap);

		Triangles.Add(TopLeftWrap);
		Triangles.Add(BottomRightWrap);
		Triangles.Add(TopRightWrap);
	}

	// Clear any existing mesh data
	//ProcMeshComponent->ClearAllMeshSections();
	VerticalRailingMesh->CreateMeshSection_LinearColor(4, Vertices, Triangles, Normals, UVs, Colors, Tangents, true);
	VerticalRailingMesh->SetMaterial(4, StandMaterial);
}

void AVerticalRailActor::GenerateSphereWithCone(float BaseLength, float BaseWidth, float Height_){
	TArray<FVector> Vertices;

	float L = (BaseLength / 2.0f);
	float W = BaseWidth / 2.0f;
	float H = Height_;

	Vertices.Add(FVector(-L, -W, 0 + (Height / 2) + 4 + (Length/2))); //0
	Vertices.Add(FVector(L, -W, 0 + (Height / 2) + 4 + (Length/2))); //1
	Vertices.Add(FVector(L, W, 0 + (Height / 2) + 4 + (Length/2))); //2
	Vertices.Add(FVector(-L, W, 0 + (Height / 2) + 4 + (Length/2))); //3

	//front
	Vertices.Add(FVector(-L, W, 0 + (Height / 2) + 4 + (Length/2)));//4
	Vertices.Add(FVector(0, 0, H + (Height / 2) + 4 + (Length/2))); //5
	Vertices.Add(FVector(-L, -W, 0 + (Height / 2) + 4 + (Length/2))); //6

	//left
	Vertices.Add(FVector(-L, -W, 0 + (Height / 2) + 4 + (Length/2))); //7
	Vertices.Add(FVector(0, 0, H + (Height / 2) + 4 + (Length/2))); //8
	Vertices.Add(FVector(L, -W, 0 + (Height / 2) + 4 + (Length/2)));//9

	//back
	Vertices.Add(FVector(L, -W, 0 + (Height / 2) + 4 + (Length/2))); //10
	Vertices.Add(FVector(0, 0, H + (Height / 2) + 4 + (Length/2))); //11
	Vertices.Add(FVector(L, W, 0 + (Height / 2) + 4 + (Length/2)));//12

	//right
	Vertices.Add(FVector(L, W, 0 + (Height / 2) + 4 + (Length/2))); //13
	Vertices.Add(FVector(0, 0, H + (Height / 2) + 4 + (Length/2))); //14
	Vertices.Add(FVector(-L, W, 0 + (Height / 2) + 4 + (Length/2)));//15
	// Triangles

	TArray<int32> Triangles = { 0,1,3, 1,2,3 ,4,5,6,7,8,9,10,11,12,13,14,15 };

	TArray<FVector2D>  UVs = { FVector2D(0,0), FVector2D(0,1), FVector2D(1,1), FVector2D(1,0),
			FVector2D(1,1),FVector2D(0,0.5),FVector2D(0,0),
			FVector2D(1,1),FVector2D(0,0.5),FVector2D(0,0),
			FVector2D(1,1),FVector2D(0,0.5),FVector2D(0,0),
			FVector2D(1,1),FVector2D(0,0.5),FVector2D(0,0),
	};
	VerticalRailingMesh->CreateMeshSection_LinearColor(5, Vertices, Triangles, TArray<FVector>(), UVs, TArray<FLinearColor>(), TArray<FProcMeshTangent>(), true);

	VerticalRailingMesh->SetMaterial(5, SphereConeMaterial);
}

void AVerticalRailActor::GenerateRectangle(){

	const int32 VertexCount = 6 * 4; // 6 sides on a cube, 4 verts each
	const int32 TriangleCount = 6 * 2 * 3; // 2x Triangles_Arr per cube side, 3 verts each

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

	const float OffsetX = Length / 2.0f;
	const float OffsetY = Width / 2.0f;
	const float OffsetZ = 5;

	// Define the 8 corners of the cube
	const FVector P0 = FVector(OffsetX, OffsetY, -OffsetZ + (Height/2) + 6);
	const FVector P1 = FVector(OffsetX, -OffsetY, -OffsetZ + (Height/2) + 6);
	const FVector P2 = FVector(OffsetX, -OffsetY, OffsetZ + (Height/2) + 6);
	const FVector P3 = FVector(OffsetX, OffsetY, OffsetZ + (Height/2) + 6);
	const FVector P4 = FVector(-OffsetX, OffsetY, -OffsetZ + (Height/2) + 6);
	const FVector P5 = FVector(-OffsetX, -OffsetY, -OffsetZ + (Height/2) + 6);
	const FVector P6 = FVector(-OffsetX, -OffsetY, OffsetZ + (Height/2) + 6);
	const FVector P7 = FVector(-OffsetX, OffsetY, OffsetZ + (Height/2) + 6);

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

	VerticalRailingMesh->CreateMeshSection_LinearColor(1, Vertices_Arr, Triangles_Arr, Normals_Arr, UVs_Arr, Colors_Arr, Tangents_Arr, true);
	VerticalRailingMesh->SetMaterial(1, RectangleMaterial);
}

void AVerticalRailActor::GenerateRoundedOverTop(float Radius, int32 Segments, int32 Rings) {
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UVs;
	TArray<FProcMeshTangent> Tangents;
	TArray<FLinearColor> Colors;

	for (int r = 0; r < Rings; ++r)
	{
		float phi = PI * r / (Rings + 1);
		for (int s = 0; s < Segments; ++s)
		{
			float theta = 2 * PI * s / Segments;
			FVector pos = FVector(Radius * FMath::Sin(phi) * FMath::Cos(theta), Radius * FMath::Sin(phi) * FMath::Sin(theta), Radius * FMath::Cos(phi) + (Height / 2) + Radius + 10);
			Vertices.Add(pos);
			Normals.Add(pos.GetSafeNormal());
			UVs.Add(FVector2D(s / (float)Segments, r / (float)(Rings + 1)));
			FVector tangent = FVector(-FMath::Sin(theta), FMath::Cos(theta), 0).GetSafeNormal();
			Tangents.Add(FProcMeshTangent(tangent, false));
			Colors.Add(FLinearColor(1, 1, 1, 1));

			// We only add extra vertices at the seam on the first ring iteration
			if (s == Segments) {
				// Duplicate the first vertex of each ring at the seam for seamless UVs
				Vertices.Add(Vertices[1 + (r - 1) * (Segments + 1)]);
				Normals.Add(Normals[1 + (r - 1) * (Segments + 1)]);
				UVs.Add(FVector2D(1.f, (float)r / (Rings + 1)));
				Tangents.Add(Tangents[1 + (r - 1) * (Segments + 1)]);
				Colors.Add(FLinearColor(1, 1, 1, 1));
			}
		}
	}

	// Bottom vertex
	Vertices.Add(FVector(0, 0, -Radius));
	Normals.Add(FVector(0, 0, -1));
	UVs.Add(FVector2D(0.5f, 1));
	Tangents.Add(FProcMeshTangent(FVector(1, 0, 0), false));
	Colors.Add(FLinearColor(1, 1, 1, 1));

	// Top Cap
	for (int s = 0; s < Segments; ++s)
	{
		Triangles.Add(0);
		Triangles.Add(1 + s);
		Triangles.Add(1 + (s + 1) % Segments);
	}

	// Body
	for (int r = 0; r < Rings; ++r)
	{
		for (int s = 0; s < Segments; ++s)
		{
			int current = 1 + r * Segments + s;
			int next = current + 1;
			if (s == Segments - 1) next = 1 + r * Segments;  // Wrap around

			int below = current + Segments;
			int belowNext = next + Segments;

			Triangles.Add(current);
			Triangles.Add(next);
			Triangles.Add(belowNext);

			Triangles.Add(current);
			Triangles.Add(belowNext);
			Triangles.Add(below);
		}
	}

	// Bottom Cap
	int bottomIndex = Vertices.Num() - 1;
	int firstOfLastRing = 1 + (Rings - 1) * Segments;
	for (int s = 0; s < Segments; ++s)
	{
		Triangles.Add(bottomIndex);
		Triangles.Add(firstOfLastRing + (s + 1) % Segments);
		Triangles.Add(firstOfLastRing + s);
	}

	VerticalRailingMesh->CreateMeshSection_LinearColor(5, Vertices, Triangles, Normals, UVs, Colors, Tangents, true);
	VerticalRailingMesh->SetMaterial(5, RoundedOverTopMaterial);

}

void AVerticalRailActor::SetVerticalMaterial(int segment,UMaterialInterface* Material) {
	VerticalRailingMesh->SetMaterial(segment, Material);
	VerticalRailingMesh->SetMaterial(3, Material);
	VerticalRailingMesh->SetMaterial(4, Material);
	VerticalRailingMesh->SetMaterial(5, Material);
}

void AVerticalRailActor::BeginPlay()
{
	Super::BeginPlay();

}

void AVerticalRailActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}