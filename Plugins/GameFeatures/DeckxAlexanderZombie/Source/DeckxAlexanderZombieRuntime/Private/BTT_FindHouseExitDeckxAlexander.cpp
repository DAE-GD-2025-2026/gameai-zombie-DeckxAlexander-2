// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_FindHouseExitDeckxAlexander.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DeckxAlexanderZombieRuntime/StudentPerceptorDeckxAlexander.h"
#include "Village/House/House.h"

UBTT_FindHouseExitDeckxAlexander::UBTT_FindHouseExitDeckxAlexander()
{
}

EBTNodeResult::Type UBTT_FindHouseExitDeckxAlexander::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)return EBTNodeResult::Failed;
	APawn* Survivor = Cast<APawn>(AIController->GetPawn());
	if (!Survivor)return EBTNodeResult::Failed;
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB) return  EBTNodeResult::Failed;
	UStudentPerceptorDeckxAlexander* perceptor = Survivor->FindComponentByClass<UStudentPerceptorDeckxAlexander>();
	if (!perceptor) return EBTNodeResult::Failed;
	
	auto house = Cast<AHouse>(BB->GetValueAsObject(FName("CurrentHouse")));
	if (!house) return EBTNodeResult::Failed;
	
	const FHouseBounds Bounds = house->GetBounds();
	FBox Box(Bounds.Origin - Bounds.Extent,Bounds.Origin + Bounds.Extent);
	
	FVector Exit = GetRandomPointOutsideBox(Box, 200);
	BB->SetValueAsVector(FName("TargetLocation"), Exit);
	
	return EBTNodeResult::Succeeded;
}

FVector UBTT_FindHouseExitDeckxAlexander::GetRandomPointOutsideBox(const FBox& Box, float MaxDistance)
{
	const FVector Center = Box.GetCenter();
	const FVector Extent = Box.GetExtent();

	FVector Dir = FMath::VRand(); // normalized random direction

	// Point on the box surface in that direction
	FVector SurfacePoint = Center + FVector(
		Dir.X * Extent.X,
		Dir.Y * Extent.Y,
		Dir.Z * Extent.Z);

	float ExtraDistance = FMath::FRandRange(0.f, MaxDistance);

	return SurfacePoint + Dir * ExtraDistance;
}
