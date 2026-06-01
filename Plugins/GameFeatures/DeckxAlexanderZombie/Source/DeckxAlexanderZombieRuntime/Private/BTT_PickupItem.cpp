// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_PickupItem.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Common/InventoryComponent.h"
#include "Items/BaseItem.h"
#include "DeckxAlexanderZombieRuntime/StudentPerceptor.h"

UBTT_PickupItem::UBTT_PickupItem()
{
}

EBTNodeResult::Type UBTT_PickupItem::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)return EBTNodeResult::Failed;
	APawn* Survivor = Cast<APawn>(AIController->GetPawn());
	if (!Survivor)return EBTNodeResult::Failed;
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB) return  EBTNodeResult::Failed;
	UStudentPerceptor* perceptor = Survivor->FindComponentByClass<UStudentPerceptor>();
	if (!perceptor) return EBTNodeResult::Failed;
	
	UInventoryComponent* inventory = Survivor->FindComponentByClass<UInventoryComponent>();
	
	auto item = Cast<ABaseItem>(BB->GetValueAsObject(FName("TargetItem")));
	if (!item) return EBTNodeResult::Failed;
	int slot = static_cast<int>(item->GetItemType());
	
	perceptor->RemoveItemFromMemory(item);
	inventory->GrabItem(slot, item);
	
	BB->SetValueAsObject(FName("TargetItem"), nullptr);
	return EBTNodeResult::Succeeded;
}
