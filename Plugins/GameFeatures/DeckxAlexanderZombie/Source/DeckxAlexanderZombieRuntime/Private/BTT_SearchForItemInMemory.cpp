// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_SearchForItemInMemory.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Common/InventoryComponent.h"
#include "DeckxAlexanderZombieRuntime/StudentPerceptor.h"


UBTT_SearchForItemInMemory::UBTT_SearchForItemInMemory()
{
}

EBTNodeResult::Type UBTT_SearchForItemInMemory::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)return EBTNodeResult::Failed;
	APawn* Survivor = Cast<APawn>(AIController->GetPawn());
	if (!Survivor)return EBTNodeResult::Failed;
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB) return  EBTNodeResult::Failed;
	UStudentPerceptor* perceptor = Survivor->FindComponentByClass<UStudentPerceptor>();
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
