// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_SearchForItemInMemoryDeckxAlexander.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Common/InventoryComponent.h"
#include "DeckxAlexanderZombieRuntime/StudentPerceptorDeckxAlexander.h"


UBTT_SearchForItemInMemoryDeckxAlexander::UBTT_SearchForItemInMemoryDeckxAlexander()
{
}

EBTNodeResult::Type UBTT_SearchForItemInMemoryDeckxAlexander::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)return EBTNodeResult::Failed;
	APawn* Survivor = Cast<APawn>(AIController->GetPawn());
	if (!Survivor)return EBTNodeResult::Failed;
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB) return  EBTNodeResult::Failed;
	UStudentPerceptorDeckxAlexander* perceptor = Survivor->FindComponentByClass<UStudentPerceptorDeckxAlexander>();
	if (!perceptor) return EBTNodeResult::Failed;
	
	for (auto type : ItemType)
	{
		auto neededItem = perceptor->GetClosestItemOfTypeInMemory(type);
		if (neededItem)
		{
			BB->SetValueAsObject(FName("TargetItem"), neededItem);
			BB->SetValueAsVector(FName("TargetLocation"), neededItem->GetActorLocation());
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}
