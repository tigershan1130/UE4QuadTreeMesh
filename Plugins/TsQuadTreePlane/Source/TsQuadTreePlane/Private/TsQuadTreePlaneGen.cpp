// Fill out your copyright notice in the Description page of Project Settings.


#include "TsQuadTreePlaneGen.h"
#include "MeshUtilities.h"

// Sets default values
ATsQuadTreePlaneGen::ATsQuadTreePlaneGen()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	mesh = CreateDefaultSubobject<URuntimeMeshComponent>(TEXT("GenerateMesh"));

	RootComponent = mesh;
	//mesh->bUseAsyncCooking = true;

	// This Here is very important, it decides how QuadTree Lod Distance is been controlled.
	DistanceDepthLOD.Add(4000000);
	DistanceDepthLOD.Add(400000); 
	DistanceDepthLOD.Add(200000);
	DistanceDepthLOD.Add(100000);// when at 10000 metres, we need more details
	DistanceDepthLOD.Add(80000);
	DistanceDepthLOD.Add(60000);
	DistanceDepthLOD.Add(40000); 
	DistanceDepthLOD.Add(20000);
	DistanceDepthLOD.Add(15000);
	DistanceDepthLOD.Add(10000);
	DistanceDepthLOD.Add(8000);
	DistanceDepthLOD.Add(6500); 
	DistanceDepthLOD.Add(6000);
	DistanceDepthLOD.Add(4500);
	DistanceDepthLOD.Add(3000);
	DistanceDepthLOD.Add(1500);
	DistanceDepthLOD.Add(700);
	DistanceDepthLOD.Add(350);
	

	MAX_DISTANCE = DistanceDepthLOD[0];

	ActorLocation = GetActorLocation();


	UWorld* world = GetWorld();

	if (world == nullptr)
		return;

	auto viewLocations = world->ViewLocationsRenderedLastFrame;

	if (viewLocations.Num() == 0)
		return;

	CameraLocation = viewLocations[0];


	
}

// Called when the game starts or when spawned
void ATsQuadTreePlaneGen::BeginPlay()
{
	Super::BeginPlay();
	
	CreatePlane();
}

void ATsQuadTreePlaneGen::PostActorCreated()
{
	Super::PostActorCreated();

	//ResetQuadTreeData();
	//
	//BuildQuadTree();

	//CreatePlane();

}

void ATsQuadTreePlaneGen::PostLoad()
{
	Super::PostLoad();

	//ResetQuadTreeData();

	//BuildQuadTree();

	//CreatePlane();
}

void ATsQuadTreePlaneGen::CreatePlane()
{

	FOccluderVertexArray VertexList;
	TArray<int32> TriangleList;
	TArray<FVector2D> UVList;
	FOccluderVertexArray Normals;
	TArray<FColor> VertexColors;
	TArray<FRuntimeMeshTangent> Tangents;

	UE_LOG(LogTemp, Warning, TEXT("number  of triangles: %d"), RenderQuads.Num() * 4);

	float normalizedX = 0;
	float normalizedY = 0;

	float tiledX = 0;
	float tiledY = 0;


	for (int i = 0; i < RenderQuads.Num(); i++)
	{
		VertexList.Add(FVector(RenderQuads[i].CenterPoint, 0));
		VertexList.Add(FVector(RenderQuads[i].TopLeftPoint, 0));
		VertexList.Add(FVector(RenderQuads[i].TopRightPoint, 0));
		VertexList.Add(FVector(RenderQuads[i].BottomRightPoint, 0));
		VertexList.Add(FVector(RenderQuads[i].BottomLeftPoint, 0));

		
		normalizedX = RenderQuads[i].CenterPoint.X / MAX_DISTANCE + .5f;
		normalizedY = RenderQuads[i].CenterPoint.Y / MAX_DISTANCE + .5f;

		UVList.Add(FVector2D(tiledX, tiledY));

		normalizedX = RenderQuads[i].TopLeftPoint.X / MAX_DISTANCE + .5f;
		normalizedY = RenderQuads[i].TopLeftPoint.Y / MAX_DISTANCE + .5f;

		UVList.Add(FVector2D(tiledX, tiledY));
		
		normalizedX = RenderQuads[i].TopRightPoint.X / MAX_DISTANCE + .5f;
		normalizedY = RenderQuads[i].TopRightPoint.Y / MAX_DISTANCE + .5f;

		UVList.Add(FVector2D(tiledX, tiledY));
		
		normalizedX = RenderQuads[i].BottomRightPoint.X / MAX_DISTANCE + .5f;
		normalizedY = RenderQuads[i].BottomRightPoint.Y / MAX_DISTANCE + .5f;

		UVList.Add(FVector2D(tiledX, tiledY));
		
		normalizedX = RenderQuads[i].BottomLeftPoint.X / MAX_DISTANCE + .5f;
		normalizedY = RenderQuads[i].BottomLeftPoint.Y / MAX_DISTANCE + .5f;

		UVList.Add(FVector2D(tiledX, tiledY));
		

		Normals.Add(FVector(0, 0, 1));
		Normals.Add(FVector(0, 0, 1));
		Normals.Add(FVector(0, 0, 1));
		Normals.Add(FVector(0, 0, 1));
		Normals.Add(FVector(0, 0, 1));

		VertexColors.Add(FColor(0, 0, 0, 0));
		VertexColors.Add(FColor(0, 0, 0, 0));
		VertexColors.Add(FColor(0, 0, 0, 0));
		VertexColors.Add(FColor(0, 0, 0, 0));
		VertexColors.Add(FColor(0, 0, 0, 0));

		Tangents.Add(FRuntimeMeshTangent(1, 0, 0));
		Tangents.Add(FRuntimeMeshTangent(1, 0, 0));
		Tangents.Add(FRuntimeMeshTangent(1, 0, 0));
		Tangents.Add(FRuntimeMeshTangent(1, 0, 0));
		Tangents.Add(FRuntimeMeshTangent(1, 0, 0));

		// First Triangle
		TriangleList.Add(i * 5 + 0);
		TriangleList.Add(i * 5 + 1);
		TriangleList.Add(i * 5 + 2);

		TriangleList.Add(i * 5 + 0);
		TriangleList.Add(i * 5 + 2);
		TriangleList.Add(i * 5 + 3);

		TriangleList.Add(i * 5 + 0);
		TriangleList.Add(i * 5 + 3);
		TriangleList.Add(i * 5 + 4);

		TriangleList.Add(i * 5 + 0);
		TriangleList.Add(i * 5 + 4);
		TriangleList.Add(i * 5 + 1);

	}

	mesh->CreateMeshSection(0, VertexList, TriangleList, Normals, UVList,VertexColors, Tangents, false, EUpdateFrequency::Infrequent);
	
	//BuildOptimizationThirdParty::NvTriStripHelper::Build
}


void ATsQuadTreePlaneGen::UpdatePlane()
{

	FOccluderVertexArray VertexList;
	TArray<int32> TriangleList;
	TArray<FVector2D> UVList;
	FOccluderVertexArray Normals;
	TArray<FColor> VertexColors;
	TArray<FRuntimeMeshTangent> Tangents;

	UE_LOG(LogTemp, Warning, TEXT("number  of triangles: %d"), RenderQuads.Num() * 4);

	float normalizedX = 0;
	float normalizedY = 0;

	float tiledX = 0;
	float tiledY = 0;


	for (int i = 0; i < RenderQuads.Num(); i++)
	{
		VertexList.Add(FVector(RenderQuads[i].CenterPoint, 0));
		VertexList.Add(FVector(RenderQuads[i].TopLeftPoint, 0));
		VertexList.Add(FVector(RenderQuads[i].TopRightPoint, 0));
		VertexList.Add(FVector(RenderQuads[i].BottomRightPoint, 0));
		VertexList.Add(FVector(RenderQuads[i].BottomLeftPoint, 0));


		normalizedX = RenderQuads[i].CenterPoint.X / MAX_DISTANCE + .5f;
		normalizedY = RenderQuads[i].CenterPoint.Y / MAX_DISTANCE + .5f;

		UVList.Add(FVector2D(normalizedX, normalizedY));

		normalizedX = RenderQuads[i].TopLeftPoint.X / MAX_DISTANCE + .5f;
		normalizedY = RenderQuads[i].TopLeftPoint.Y / MAX_DISTANCE + .5f;

		UVList.Add(FVector2D(normalizedX, normalizedY));

		normalizedX = RenderQuads[i].TopRightPoint.X / MAX_DISTANCE + .5f;
		normalizedY = RenderQuads[i].TopRightPoint.Y / MAX_DISTANCE + .5f;

		UVList.Add(FVector2D(normalizedX, normalizedY));

		normalizedX = RenderQuads[i].BottomRightPoint.X / MAX_DISTANCE + .5f;
		normalizedY = RenderQuads[i].BottomRightPoint.Y / MAX_DISTANCE + .5f;

		UVList.Add(FVector2D(normalizedX, normalizedY));

		normalizedX = RenderQuads[i].BottomLeftPoint.X / MAX_DISTANCE + .5f;
		normalizedY = RenderQuads[i].BottomLeftPoint.Y / MAX_DISTANCE + .5f;

		UVList.Add(FVector2D(normalizedX, normalizedY));


		Normals.Add(FVector(0, 0, 1));
		Normals.Add(FVector(0, 0, 1));
		Normals.Add(FVector(0, 0, 1));
		Normals.Add(FVector(0, 0, 1));
		Normals.Add(FVector(0, 0, 1));

		VertexColors.Add(FColor(0, 0, 0, 0));
		VertexColors.Add(FColor(0, 0, 0, 0));
		VertexColors.Add(FColor(0, 0, 0, 0));
		VertexColors.Add(FColor(0, 0, 0, 0));
		VertexColors.Add(FColor(0, 0, 0, 0));

		Tangents.Add(FRuntimeMeshTangent(1, 0, 0));
		Tangents.Add(FRuntimeMeshTangent(1, 0, 0));
		Tangents.Add(FRuntimeMeshTangent(1, 0, 0));
		Tangents.Add(FRuntimeMeshTangent(1, 0, 0));
		Tangents.Add(FRuntimeMeshTangent(1, 0, 0));

		// First Triangle
		TriangleList.Add(i * 5 + 0);
		TriangleList.Add(i * 5 + 1);
		TriangleList.Add(i * 5 + 2);

		TriangleList.Add(i * 5 + 0);
		TriangleList.Add(i * 5 + 2);
		TriangleList.Add(i * 5 + 3);

		TriangleList.Add(i * 5 + 0);
		TriangleList.Add(i * 5 + 3);
		TriangleList.Add(i * 5 + 4);

		TriangleList.Add(i * 5 + 0);
		TriangleList.Add(i * 5 + 4);
		TriangleList.Add(i * 5 + 1);

	}




	mesh->UpdateMeshSection(0, VertexList, TriangleList, Normals, UVList, VertexColors, Tangents);


	mesh->SetMaterial(0, AssignMaterial);

}

void ATsQuadTreePlaneGen::BuildQuadTree()
{
	float HeightDifference = ActorLocation.Z - CameraLocation.Z;
	float heightInterpolation = 1 - HeightDifference * HeightDifference / (MAX_HEIGHT * MAX_HEIGHT);
	heightInterpolation = heightInterpolation * 0.5f + 0.5f;


	// construct the reset of the quad trees
	for (int i = 0; i < DistanceDepthLOD.Num(); i++)
	{
		if (i == 0)
		{
			float halfWidth = MAX_DISTANCE / 2;
			float halfHeight = MAX_DISTANCE / 2;

			FVector2D BottomLeftPoint = FVector2D(ActorLocation.X - halfWidth, ActorLocation.Y - halfHeight);
			FVector2D BottomRightPoint = FVector2D(ActorLocation.X + halfWidth, ActorLocation.Y - halfHeight);
			FVector2D TopLeftPoint = FVector2D(ActorLocation.X - halfWidth, ActorLocation.Y + halfHeight);
			FVector2D TopRightPoint = FVector2D(ActorLocation.X + halfWidth, ActorLocation.Y + halfHeight);

			InsertQuadTree(MAX_DISTANCE, MAX_DISTANCE, FVector2D(ActorLocation.X, ActorLocation.Y), 0, BottomLeftPoint, BottomRightPoint, TopLeftPoint, TopRightPoint, nullptr);
						
		}
		else if(i < DistanceDepthLOD.Num() && i > 0)
		{
			//UE_LOG(LogTemp, Warning, TEXT("%d , current quadtree parents: %d"), i, QuadTreeNodeList[i - 1].Num());

			for (int j = 0; j < QuadTreeNodeList[i - 1].Num(); j++)
			{
				//UE_LOG(LogTemp, Warning, TEXT("%d "), j);
				FVector2D BottomLeftPoint = QuadTreeNodeList[i - 1][j].BottomLeftPoint;
				FVector2D BottomRightPoint = QuadTreeNodeList[i - 1][j].BottomRightPoint;
				FVector2D TopLeftPoint = QuadTreeNodeList[i - 1][j].TopLeftPoint;
				FVector2D TopRightPoint = QuadTreeNodeList[i - 1][j].TopRightPoint;

				// =============================== Distance LOD =====================================================
				float CurrentDistance = GetDistanceSqrFromPoints(TopLeftPoint, TopRightPoint, BottomRightPoint, BottomLeftPoint, FVector2D(CameraLocation.X, CameraLocation.Y));

				//UE_LOG(LogTemp, Warning, TEXT("Distance: %f, Depth Baked Distance: %f, Depth: %d"), CurrentDistance, DistanceDepthLOD[i], i);
				bool canDraw = (CurrentDistance < DistanceDepthLOD[i]) ? true : false;

				
				if (!canDraw)
					continue;

				//UE_LOG(LogTemp, Warning, TEXT("UE Insert quad tree %d "), j);
				// ===================================================================================================

				InsertQuadTree(QuadTreeNodeList[i - 1][j].Width*2, QuadTreeNodeList[i - 1][j].Height*2, QuadTreeNodeList[i - 1][j].CenterPoint, i, BottomLeftPoint, BottomRightPoint, TopLeftPoint, TopRightPoint, &QuadTreeNodeList[i - 1][j]);
			}
		}
	}

	//UE_LOG(LogTemp, Warning, TEXT("Traverse Quad Tree Render buffer"));
	TraverseTreeCreateRenderBuffer();


}

void ATsQuadTreePlaneGen::InsertQuadTree(float Width, float Height, FVector2D CenterPoint, int Depth, FVector2D BottomLeftPoint, FVector2D BottomRightPoint, FVector2D TopLeftPoint, FVector2D TopRightPoint, FQuadTreeNode* ParentQT)
{
	float halfWidth = Width/2;
	float halfHeight = Height/2;

	// Current Depth
	int CurrentDepth = Depth;

	// Calculate CenterPoint
	FVector2D BottomLeftCenterPoint = (BottomLeftPoint + CenterPoint) / 2;
	FVector2D BottomRightCenterPoint = (BottomRightPoint + CenterPoint) / 2;
	FVector2D TopLeftCenterPoint = (TopLeftPoint + CenterPoint) / 2;
	FVector2D TopRightCenterPoint = (TopRightPoint + CenterPoint) / 2;

	FQuadTreeNode BotLeftQuadTreeNode = BuildAQuad(halfWidth, halfHeight, BottomLeftCenterPoint, CurrentDepth, ParentQT);
	FQuadTreeNode BotRightQuadTreeNode = BuildAQuad(halfWidth, halfHeight, BottomRightCenterPoint, CurrentDepth, ParentQT);
	FQuadTreeNode TopLeftQuadTreeNode = BuildAQuad(halfWidth, halfHeight, TopLeftCenterPoint, CurrentDepth, ParentQT);
	FQuadTreeNode TopRightQuadTreeNode = BuildAQuad(halfWidth, halfHeight, TopRightCenterPoint, CurrentDepth, ParentQT);

	BotLeftQuadTreeNode.Parent = ParentQT;
	BotRightQuadTreeNode.Parent = ParentQT;
	TopRightQuadTreeNode.Parent = ParentQT;
	TopLeftQuadTreeNode.Parent = ParentQT;

	// int current index
	


	if (ParentQT != nullptr)
	{
		int currentIndex = QuadTreeNodeList[Depth].Num();

		QuadTreeNodeList[Depth].Add(BotLeftQuadTreeNode);
		QuadTreeNodeList[Depth].Add(BotRightQuadTreeNode);
		QuadTreeNodeList[Depth].Add(TopLeftQuadTreeNode);
		QuadTreeNodeList[Depth].Add(TopRightQuadTreeNode);

		ParentQT->LeafBL = &(QuadTreeNodeList[Depth][currentIndex + 0]);
		ParentQT->LeafBR = &(QuadTreeNodeList[Depth][currentIndex + 1]);
		ParentQT->LeafTL = &(QuadTreeNodeList[Depth][currentIndex + 2]);
		ParentQT->LeafTR = &(QuadTreeNodeList[Depth][currentIndex + 3]);
		
	}
	else
	{
		QuadTreeNodeList[Depth].Add(BotLeftQuadTreeNode);
		QuadTreeNodeList[Depth].Add(BotRightQuadTreeNode);
		QuadTreeNodeList[Depth].Add(TopLeftQuadTreeNode);
		QuadTreeNodeList[Depth].Add(TopRightQuadTreeNode);

	}

}

FQuadTreeNode ATsQuadTreePlaneGen::BuildAQuad(float Width, float Height, FVector2D CenterPoint, int Depth, FQuadTreeNode* parentQT)
{
	FQuadTreeNode currentNode;

	float halfWidth = Width / 2;
	float halfHeight = Height / 2;

	currentNode.CenterPoint = CenterPoint;

	currentNode.BottomLeftPoint = FVector2D(CenterPoint.X - halfWidth, CenterPoint.Y - halfHeight);
	currentNode.BottomRightPoint = FVector2D(CenterPoint.X + halfWidth, CenterPoint.Y - halfHeight);
	currentNode.TopLeftPoint = FVector2D(CenterPoint.X - halfWidth, CenterPoint.Y + halfHeight);
	currentNode.TopRightPoint = FVector2D(CenterPoint.X + halfWidth, CenterPoint.Y + halfHeight);

	currentNode.Width = halfWidth;
	currentNode.Height = halfHeight;
	currentNode.Depth = Depth;

	return currentNode;
}

// Called every frame
void ATsQuadTreePlaneGen::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UWorld* world = GetWorld();

	if (world == nullptr)
		return;

	auto viewLocations = world->ViewLocationsRenderedLastFrame;

	if (viewLocations.Num() == 0)
		return;

	CameraLocation = viewLocations[0];


	//UE_LOG(LogTemp, Warning, TEXT("Distance: %f"), FVector::Distance(CameraLocation, ActorLocation));

	ResetQuadTreeData();

	BuildQuadTree();

	UpdatePlane();

}

