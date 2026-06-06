// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_GetClosestHouseLocationDeckxAlexander.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DeckxAlexanderZombieRuntime/StudentPerceptorDeckxAlexander.h"

UBTT_GetClosestHouseLocationDeckxAlexander::UBTT_GetClosestHouseLocationDeckxAlexander()
{
	bNotifyTick = false;
}

EBTNodeResult::Type UBTT_GetClosestHouseLocationDeckxAlexander::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)return EBTNodeResult::Failed;
	APawn* Survivor = Cast<APawn>(AIController->GetPawn());
	if (!Survivor)return EBTNodeResult::Failed;
	UBlackboardComponent* BB =OwnerComp.GetBlackboardComponent();
	if (!BB) return  EBTNodeResult::Failed;
	UStudentPerceptorDeckxAlexander* perceptor = Survivor->FindComponentByClass<UStudentPerceptorDeckxAlexander>();
	if (!perceptor) return EBTNodeResult::Failed;
	
	auto houses = perceptor->GetSeenHouses();
	if (houses.IsEmpty()) return  EBTNodeResult::Failed;
	auto house = perceptor->GetSeenHouses()[0];
	FVector housePos = house->GetActorLocation();
	
	BB->SetValueAsVector(FName("TargetLocation"),housePos);
	
	
	return  EBTNodeResult::Succeeded;
}
