// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RuntimeMeshComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TsQuadTreePlaneGen.generated.h"

USTRUCT()
struct FQuadTreeNode
{
	GENERATED_BODY()

	FQuadTreeNode* Parent = nullptr;

	FQuadTreeNode* LeafTL = nullptr;
	FQuadTreeNode* LeafTR = nullptr;
	FQuadTreeNode* LeafBL = nullptr;
	FQuadTreeNode* LeafBR = nullptr;

	FVector2D CenterPoint;

	FVector2D TopLeftPoint;
	FVector2D TopRightPoint;
	FVector2D BottomLeftPoint;
	FVector2D BottomRightPoint;

	float Height;
	float Width;

	int Depth;

	FQuadTreeNode()
	{
		CenterPoint = FVector2D(0, 0);
		Depth = 0;
	}

	

};

UCLASS()
class TSQUADTREEPLANE_API ATsQuadTreePlaneGen : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATsQuadTreePlaneGen();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual void PostActorCreated() override;

	virtual void PostLoad() override;

	virtual void CreatePlane();

	virtual void UpdatePlane();

	virtual void BuildQuadTree();

	virtual void InsertQuadTree(float Width, float Height, FVector2D CenterPoint, int Depth, FVector2D BottomLeftPoint, FVector2D BottomRightPoint, FVector2D TopLeftPoint, FVector2D TopRightPoint, FQuadTreeNode* ParentQT);

	FQuadTreeNode BuildAQuad(float Width, float Height, FVector2D CenterPoint, int Depth, FQuadTreeNode* parentQT);


	TArray<TArray<FQuadTreeNode>> QuadTreeNodeList;

	UPROPERTY()
	TArray<FQuadTreeNode> RenderQuads;
	
	UPROPERTY()
	FVector CameraLocation;

	UPROPERTY()
	FVector ActorLocation;

	// Traverse Tree of Render buffer and generate our current render buffer
	void TraverseTreeCreateRenderBuffer()
	{
		RenderQuads.Empty();


		for (int i = 0; i < QuadTreeNodeList.Num(); i++)
		{
			for (int j = 0; j < QuadTreeNodeList[i].Num(); j++)
			{
				if (QuadTreeNodeList[i][j].LeafBL == nullptr &&
					QuadTreeNodeList[i][j].LeafBR == nullptr &&
					QuadTreeNodeList[i][j].LeafTL == nullptr &&
					QuadTreeNodeList[i][j].LeafTR == nullptr)
				{

					//UE_LOG(LogTemp, Warning, TEXT("Current QuadLeaf not empty: %d,%d"), i, j);

					RenderQuads.Add(QuadTreeNodeList[i][j]);
				}
			}
		}

	}


	float GetDistanceSqrFromPoints(FVector2D TopLeftPoint, FVector2D TopRightPoint, FVector2D BotRightPoint, FVector2D BotLeftPoint, FVector2D CamLocation)
	{
		FVector2D TopLeftDir = TopLeftPoint - CamLocation;
		FVector2D TopRightDir = TopRightPoint - CamLocation;
		FVector2D BotLeftDir = BotLeftPoint - CamLocation;
		FVector2D BotRightDir = BotRightPoint - CamLocation;

		float MaxDistance = 99999999999999;

		float TopLeftDistance = (TopLeftDir.X * TopLeftDir.X) + (TopLeftDir.Y * TopLeftDir.Y);
		float TopRightDistance = (TopRightDir.X * TopRightDir.X) + (TopRightDir.Y * TopRightDir.Y);
		float BotLeftDistance = (BotLeftDir.X * BotLeftDir.X) + (BotLeftDir.Y * BotLeftDir.Y);
		float BotRightDistance = (BotRightDir.X * BotRightDir.X) + (BotRightDir.Y * BotRightDir.Y);


		if (TopLeftDistance < MaxDistance)
			MaxDistance = TopLeftDistance;

		if (TopRightDistance < MaxDistance)
			MaxDistance = TopRightDistance;

		if (BotRightDistance < MaxDistance)
			MaxDistance = BotRightDistance;

		if (BotLeftDistance < MaxDistance)
			MaxDistance = BotLeftDistance;	

		return FMath::Sqrt(MaxDistance);
	}


	void ResetQuadTreeData()
	{
		QuadTreeNodeList.Empty();
		RenderQuads.Empty();

		TArray<FQuadTreeNode> values;
		QuadTreeNodeList.Init(values, DistanceDepthLOD.Num());
	}


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UPROPERTY()
	float MAX_HEIGHT = 20000;

	UPROPERTY()
	float MAX_DISTANCE = 256000;

	//UPROPERTY(EditAnywhere)
	TArray<float> DistanceDepthLOD;

	UPROPERTY(VisibleAnywhere)
	URuntimeMeshComponent* mesh;

	// This should be set at blueprint actor's constructor to a certain material that should be assigned to mesh
	UPROPERTY(BlueprintReadWrite)
	UMaterialInterface* AssignMaterial;

};
