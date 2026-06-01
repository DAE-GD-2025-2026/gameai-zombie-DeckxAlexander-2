// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_GetClosestHouseLocation.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DeckxAlexanderZombieRuntime/StudentPerceptor.h"

UBTT_GetClosestHouseLocation::UBTT_GetClosestHouseLocation()
{
	bNotifyTick = false;
}

EBTNodeResult::Type UBTT_GetClosestHouseLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)return EBTNodeResult::Failed;
	APawn* Survivor = Cast<APawn>(AIController->GetPawn());
	if (!Survivor)return EBTNodeResult::Failed;
	UBlackboardComponent* BB =OwnerComp.GetBlackboardComponent();
	if (!BB) return  EBTNodeResult::Failed;
	UStudentPerceptor* perceptor = Survivor->FindComponentByClass<UStudentPerceptor>();
	if (!perceptor) return EBTNodeResult::Failed;
	
	auto houses = perceptor->GetSeenHouses();
	if (houses.IsEmpty()) return  EBTNodeResult::Failed;
	auto house = perceptor->GetSeenHouses()[0];
	FVector housePos = house->GetActorLocation();
	
	BB->SetValueAsVector(FName("TargetLocation"),housePos);
	
	
	return  EBTNodeResult::Succeeded;
}
