// Fill out your copyright notice in the Description page of Project Settings.


#include "DjikstraTest.h"

// Sets default values
ADjikstraTest::ADjikstraTest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADjikstraTest::BeginPlay()
{
	Super::BeginPlay();


	Generate2DGrid();

	PrintGrid();

	GenerateAndPrintStartEndPoint();

	GetShortestPath((int)StartPoint.X, (int)StartPoint.Y, 0);


	PrintTraversedGrid();

	
}

// Called every frame
void ADjikstraTest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADjikstraTest::Generate2DGrid()
{
	for (int i = 0; i < MAX_ROW; i++)
	{
		for (int j = 0; j < MAX_COLUMN; j++)
		{
			GridNode tempData;

			tempData.X = i;
			tempData.Y = j;
			tempData.Cost = FMath::RandRange(1, 3);

			GridData.Add(tempData);
		}

	}

}

// core algorithm
void ADjikstraTest::GetShortestPath(int i, int j, int CurrentCost)
{


	if (i < 0 || i > MAX_ROW-1 || j < 0 || j > MAX_COLUMN-1)
		return;

	int currentIndex = GetIndex(i, j);

	if (i == (int)EndPoint.X && j == (int)EndPoint.Y)
		return;

	if (i == (int)StartPoint.X && j == (int)StartPoint.Y && CurrentCost > 0)
		return;


	int rightIndex = GetIndex(i + 1, j);
	int leftIndex = GetIndex(i - 1, j);
	int upIndex = GetIndex(i, j + 1);
	int downIndex = GetIndex(i, j - 1);

	if (rightIndex == -1)
	{

	}
	else
	{
		int Cost = CurrentCost + GridData[rightIndex].Cost;
		if (Cost < GridData[rightIndex].MinCost)
		{
			GridData[rightIndex].MinCost = Cost;
			GridData[rightIndex].Parent = &GridData[currentIndex];
			GetShortestPath(i+1, j, Cost);
		}

	}


	if (leftIndex == -1)
	{

	}
	else
	{
		int Cost = CurrentCost + GridData[leftIndex].Cost;
		if (Cost < GridData[leftIndex].MinCost)
		{
			GridData[leftIndex].MinCost = Cost;
			GridData[leftIndex].Parent = &GridData[currentIndex];
			GetShortestPath(i - 1, j, Cost);
		}

	}


	if (upIndex == -1)
	{

	}
	else
	{
		int Cost = CurrentCost + GridData[upIndex].Cost;
		if (Cost < GridData[upIndex].MinCost)
		{
			GridData[upIndex].MinCost = Cost;
			GridData[upIndex].Parent = &GridData[currentIndex];
			GetShortestPath(i, j+1, Cost);
		}

	}

	if (downIndex == -1)
	{

	}
	else
	{
		int Cost = CurrentCost + GridData[downIndex].Cost;
		if (Cost < GridData[downIndex].MinCost)
		{
			GridData[downIndex].MinCost = Cost;
			GridData[downIndex].Parent = &GridData[currentIndex];
			GetShortestPath(i , j-1, Cost);
		}

	}

}

void ADjikstraTest::PrintGrid()
{
	FString debugString;
	for (int i = 0; i < MAX_ROW; i++)
	{
		debugString = "";
		for (int j = 0; j < MAX_COLUMN; j++)
		{
			debugString.Append("[");
			debugString.Append(FString::FromInt(GridData[GetIndex(i, j)].Cost));
			debugString.Append("]");
		}

		UE_LOG(LogTemp, Warning, TEXT("%s"), *debugString);
	}


}

void ADjikstraTest::PrintTraversedGrid()
{
	FString debugString;
	for (int i = 0; i < MAX_ROW; i++)
	{
		debugString = "";
		for (int j = 0; j < MAX_COLUMN; j++)
		{
			debugString.Append("[");
			debugString.Append(FString::FromInt(GridData[GetIndex(i, j)].MinCost));
			debugString.Append("]");
		}

		UE_LOG(LogTemp, Warning, TEXT("%s"), *debugString);
	}


	bool end = false;

	GridNode currentNode = GridData[GetIndex((int)EndPoint.X, (int)EndPoint.Y)];
	while (!end)
	{
		int X = currentNode.X;
		int Y = currentNode.Y;
		int cost = currentNode.Parent->Cost;

		UE_LOG(LogTemp, Warning, TEXT("StartPoint: (%d, %d) %d"), X, Y, cost);


		currentNode = *currentNode.Parent;

		if (currentNode.X == (int)StartPoint.X && currentNode.Y == (int)StartPoint.Y)
			end = true;

	}
}

void ADjikstraTest::GenerateAndPrintStartEndPoint()
{
	StartPoint.X = FMath::RandRange(0, MAX_ROW - 1);
	StartPoint.Y = FMath::RandRange(0, MAX_COLUMN - 1);
	EndPoint.X = FMath::RandRange(0, MAX_ROW - 1);
	EndPoint.Y = FMath::RandRange(0, MAX_COLUMN - 1);

	UE_LOG(LogTemp, Warning, TEXT("StartPoint: %d %d"), (int)StartPoint.X, (int)StartPoint.Y);
	UE_LOG(LogTemp, Warning, TEXT("EndPoint: %d %d"), (int)EndPoint.X, (int)EndPoint.Y);
}

