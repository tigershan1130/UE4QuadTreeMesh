// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DjikstraTest.generated.h"

#define MAX_ROW 50
#define MAX_COLUMN 50

struct GridNode
{
	int X;
	int Y;
	int Cost;
	int MinCost;

	GridNode* Parent;

	GridNode()
	{
		X = 0;
		Y = 0;
		Cost = 0;
		MinCost = 999999;

		Parent = nullptr;
	}

};

UCLASS()
class ALGORITHMLIBARY_API ADjikstraTest : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADjikstraTest();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FVector2D StartPoint;
	FVector2D EndPoint;

	TArray<GridNode> GridData;

	virtual void Generate2DGrid();

	void GetShortestPath(int i, int j, int CurrentCost);

	void PrintGrid();

	void PrintTraversedGrid();

	void GenerateAndPrintStartEndPoint();

	int GetIndex(int i, int j)
	{
		if (i < 0 || i > MAX_ROW-1 || j < 0 || j > MAX_COLUMN-1)
			return -1;

		return i * MAX_ROW + j;
	}

};
