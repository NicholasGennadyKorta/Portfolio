#include "FluidArchitectHexaSphereGenerator.h"
#include "Kismet/KismetMathLibrary.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Architect HexaSphere Object Generate

void UFluidArchitectHexaSphereObject::Generate(int Subdivisions)
{
	//Step 0 : Insure Empty Data To Start Off From Scratch
	RawTriangles.Empty();
	RawVertices.Empty();
	
	//Step 1 : Generate Vertices (Icosahedron Has 12 Vertices | Corners)
	{
		float Tao = (1.0f + FMath::Sqrt((5.0f))) / 2.0f;

		//Create Vertices
		RawVertices.Add (FVector (-1, Tao, 0));
		RawVertices.Add (FVector (1, Tao, 0));
		RawVertices.Add (FVector (-1, -Tao, 0));
		RawVertices.Add (FVector (1, -Tao, 0));
		RawVertices.Add (FVector (0, -1, Tao));
		RawVertices.Add (FVector (0, 1, Tao));
		RawVertices.Add (FVector (0, -1, -Tao));
		RawVertices.Add (FVector (0, 1, -Tao));
		RawVertices.Add (FVector (Tao, 0, -1));
		RawVertices.Add (FVector (Tao, 0, 1));
		RawVertices.Add (FVector (-Tao, 0, -1));
		RawVertices.Add (FVector (-Tao, 0, 1));

		//Normalize Vertices To Stop Artifacts | Scale It
		for (int i = 0; i < RawVertices.Num(); i++)
			RawVertices[i].Normalize();
	}

	//Step 2 : Create 20 Sided Die From Vertices Above (Triangles / Faces)
	{
		RawTriangles.Add(FIntVector( 0, 11,  5));
		RawTriangles.Add(FIntVector( 0,  5,  1));
		RawTriangles.Add(FIntVector( 0,  1,  7));
		RawTriangles.Add(FIntVector( 0,  7, 10));
		RawTriangles.Add(FIntVector( 0, 10, 11));
		RawTriangles.Add(FIntVector( 1,  5,  9));
		RawTriangles.Add(FIntVector( 5, 11,  4));
		RawTriangles.Add(FIntVector(11, 10,  2));
		RawTriangles.Add(FIntVector(10,  7,  6));
		RawTriangles.Add(FIntVector( 7,  1,  8));
		RawTriangles.Add(FIntVector( 3,  9,  4));
		RawTriangles.Add(FIntVector( 3,  4,  2));
		RawTriangles.Add(FIntVector( 3,  2,  6));
		RawTriangles.Add(FIntVector( 3,  6,  8));
		RawTriangles.Add(FIntVector( 3,  8,  9));
		RawTriangles.Add(FIntVector( 4,  9,  5));
		RawTriangles.Add(FIntVector( 2,  4, 11));
		RawTriangles.Add(FIntVector( 6,  2, 10));
		RawTriangles.Add(FIntVector( 8,  6,  7));
		RawTriangles.Add(FIntVector( 9,  8,  1));
	}

	//Step 3 : Subdivide
	{
		auto MidPointCache = TMap<int, int>();
	
		for (int i = 0; i < Subdivisions; i++)
		{
			//New Triangles Array
			auto NewTriangles = TArray<FIntVector>();

			for (int j = 0; j < RawTriangles.Num(); j++)
			{
				//Get Current Triangle
				FIntVector Triangle = RawTriangles[j];
	
				//Get Vertices For Triangle
				const int A = Triangle[0];
				const int B = Triangle[1];
				const int C = Triangle[2];

				// Use GetMidPointIndex To
				// A) Create A New Vertex Between 2 Old Vertices
				// B) Find The Vertex Already Created
				const int AB = GetMidPointIndex(MidPointCache, A, B);
				const int BC = GetMidPointIndex(MidPointCache, B, C);
				const int CA = GetMidPointIndex(MidPointCache, C, A);

				// Create The 4 New Triangles Using Original 3 Vertices
				// And The 3 New Midpoints.
				NewTriangles.Add(FIntVector(A, AB, CA));
				NewTriangles.Add(FIntVector(B, BC, AB));
				NewTriangles.Add(FIntVector(C, CA, BC));
				NewTriangles.Add(FIntVector(AB, BC, CA));
			}

			//Set New Triangles
			RawTriangles = NewTriangles;
		}
	}
}

int UFluidArchitectHexaSphereObject::GetMidPointIndex(TMap<int, int>& Cache, int IndexA, int IndexB)
{
	//Create Map Key (Sorted To Ensure Same Result No Matter If A Or B With Higher Index Is Passed In First)
	const int SmallerIndex = FMath::Min(IndexA, IndexB);
	const int GreaterIndex = FMath::Max(IndexA, IndexB);
	const int Key = (SmallerIndex << 16) + GreaterIndex;

	//Return If Midpoint Is Already Defined | Else New Midpoint And Return
	if (Cache.Contains(Key))
		return Cache[Key];
	int R = 0;

	//Create Midpoint From Two Vertices
	FVector P1 = RawVertices[IndexA];
	FVector P2 = RawVertices[IndexB];
	FVector Middle = FMath::Lerp(P1, P2, 0.5f);
	Middle.Normalize();

	//Set R To Vertices Count | Add Middle To HexaSphere Vertices
	R = RawVertices.Num();
	RawVertices.Add(Middle);

	//Cache Midpoint So We Only Need To Calculate It Once
	Cache.Add(Key, R);
	return R;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Architect HexaSphere Object Mesh

void UFluidArchitectHexaSphereObject::MeshUtilitiesConstructMesh(FRandomStream Seed, float ScaleMin, float ScaleMax, TArray<FVector>& Vertices_Out, TArray<int>& Triangles_Out, TArray<FVector>& Normals_Out,TArray<FLinearColor>& VertexColors_Out)
{
	//Get Scale
	const int Scale = Seed.FRandRange(ScaleMin,ScaleMax);
	
	//Mesh Data
	int MeshVertexCount = RawTriangles.Num() * 3;
	Vertices_Out.Init(FVector(0,0,0),MeshVertexCount);
	Triangles_Out.Init(0,MeshVertexCount);
	Normals_Out.Init(FVector(0,0,0),MeshVertexCount);
	VertexColors_Out.Init(FLinearColor::Black,MeshVertexCount);

	//Create Mesh Out Of HexaSphere Raw Triangles And Vertices
	for (int i = 0; i < RawTriangles.Num(); i++)
	{
		//Get Hexa Sphere Triangle
		auto Triangle = RawTriangles[i];

		//Create Mesh Triangles
		Triangles_Out[i * 3 + 0] = i * 3 + 0;
		Triangles_Out[i * 3 + 1] = i * 3 + 1;
		Triangles_Out[i * 3 + 2] = i * 3 + 2;

		//Create Mesh Vertices From HexaSphere Vertices
		Vertices_Out[i * 3 + 0] = RawVertices[Triangle[0]] * Scale;
		Vertices_Out[i * 3 + 1] = RawVertices[Triangle[1]] * Scale;
		Vertices_Out[i * 3 + 2] = RawVertices[Triangle[2]] * Scale;

		//Create Mesh Normals From HexaSphere Vertices
		Normals_Out[i * 3 + 0] = RawVertices[Triangle[0]];
		Normals_Out[i * 3 + 1] = RawVertices[Triangle[1]];
		Normals_Out[i * 3 + 2] = RawVertices[Triangle[2]];
		
		//Create Mesh Vertex Colors
		VertexColors_Out[i * 3 + 0] = FColor::White;
		VertexColors_Out[i * 3 + 1] = FColor::White;
		VertexColors_Out[i * 3 + 2] = FColor::White;
	}
}

void UFluidArchitectHexaSphereObject::MeshUtilitiesConstructMeshWithRandVertexColors(FRandomStream Seed, float ScaleMin, float ScaleMax, TArray<FVector>& Vertices_Out, TArray<int>& Triangles_Out, TArray<FVector>& Normals_Out,TArray<FLinearColor>& VertexColors_Out)
{
	MeshUtilitiesConstructMesh(Seed,ScaleMin,ScaleMax,Vertices_Out,Triangles_Out,Normals_Out,VertexColors_Out);
	

	//Create Mesh Out Of HexaSphere Raw Triangles And Vertices
	for (int i = 0; i < VertexColors_Out.Num(); i++)
	{
		//Create Mesh Vertex Colors Random
		const FLinearColor VertexColor = FLinearColor(FColor(Seed.FRandRange(0,255),Seed.FRandRange(0,255),Seed.FRandRange(0,255),255));
		VertexColors_Out[i] = VertexColor;
		VertexColors_Out[i] = VertexColor;
		VertexColors_Out[i] = VertexColor;
	}
}
