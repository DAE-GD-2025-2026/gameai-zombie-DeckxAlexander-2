// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_UseItemDeckxAlexander.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Common/InventoryComponent.h"
#include "DeckxAlexanderZombieRuntime/StudentPerceptorDeckxAlexander.h"

UBTT_UseItemDeckxAlexander::UBTT_UseItemDeckxAlexander()
{
}

EBTNodeResult::Type UBTT_UseItemDeckxAlexander::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)return EBTNodeResult::Failed;
	APawn* Survivor = Cast<APawn>(AIController->GetPawn());
	if (!Survivor)return EBTNodeResult::Failed;
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB) return  EBTNodeResult::Failed;
	auto perceptor = Survivor->FindComponentByClass<UStudentPerceptorDeckxAlexander>();
	if (!perceptor) return EBTNodeResult::Failed;
	
	auto inventory = Survivor->FindComponentByClass<UInventoryComponent>();
	int slot = FindItem(inventory);
	
	if (slot == -1)
	{
		if (itemType == EItemType::Medkit) BB->SetValueAsBool("HasMedkit", false);
		if (itemType == EItemType::Food) BB->SetValueAsBool("HasFood", false);
		return EBTNodeResult::Failed;
	}
	
	inventory->UseItem(slot);
	if (inventory->GetInventory()[slot]->GetValue() <= 0)
	{
		inventory->RemoveItem(slot);
	}
	

	
	
	return EBTNodeResult::Succeeded;
}

int UBTT_UseItemDeckxAlexander::FindItem(UInventoryComponent* inventory)
{
	int itemSlot = -1;
	for (int slot{}; slot < inventory->GetInventory().Num(); slot++)
	{
		auto item = inventory->GetInventory()[slot];
		if (!item) continue;
		if (item->GetItemType() == itemType && item->GetValue() > 0)
		{
			itemSlot = slot;
		}
	}
	return itemSlot;
}
