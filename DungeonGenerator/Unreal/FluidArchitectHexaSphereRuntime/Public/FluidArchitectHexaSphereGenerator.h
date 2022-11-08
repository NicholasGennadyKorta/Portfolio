// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "FluidArchitectHexaSphereGenerator.generated.h"

//Fluid Architect HexaSphere Object
UCLASS(BlueprintType)
class FLUIDARCHITECTHEXASPHERERUNTIME_API UFluidArchitectHexaSphereObject : public UObject
{

	GENERATED_BODY()

//Generations	
public:

	UFUNCTION(BlueprintCallable)
		void Generate(int Subdivisions);
	UFUNCTION(BlueprintInternalUseOnly)
		int GetMidPointIndex(TMap<int, int>& Cache, int IndexA, int IndexB);
	
	UPROPERTY()
		TArray<FVector> RawVertices;
	UPROPERTY()
		TArray<FIntVector> RawTriangles;

//Mesh Utilities
public:

	UFUNCTION(BlueprintCallable)
		void MeshUtilitiesConstructMesh(FRandomStream Seed, float ScaleMin, float ScaleMax, TArray<FVector>& Vertices_Out, TArray<int>& Triangles_Out, TArray<FVector>& Normals_Out,TArray<FLinearColor>& VertexColors_Out);
	UFUNCTION(BlueprintCallable)
		void MeshUtilitiesConstructMeshWithRandVertexColors(FRandomStream Seed, float ScaleMin, float ScaleMax, TArray<FVector>& Vertices_Out, TArray<int>& Triangles_Out, TArray<FVector>& Normals_Out,TArray<FLinearColor>& VertexColors_Out);
	
};