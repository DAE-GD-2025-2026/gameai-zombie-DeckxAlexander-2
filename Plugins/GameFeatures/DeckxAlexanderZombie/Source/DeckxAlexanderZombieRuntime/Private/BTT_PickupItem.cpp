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
	auto itemType = item->GetItemType();
	
	
	if (!item)
	{
		BB->SetValueAsBool(FName("SeenItem"), false);
		return EBTNodeResult::Failed;
	}
	perceptor->RemoveItemFromMemory(item);
	
	if (IsInventoryFull(inventory))
	{
		inventory->RemoveItem(4); //TODO remove item based on priority
	}
	
	for (int slot{}; slot < inventory->GetInventory().Num(); ++slot)
	{
		if (inventory->GrabItem(slot, item))
		{
			
			BB->SetValueAsObject(FName("TargetItem"), nullptr);
			
			if (item->GetItemType() == EItemType::Garbage) inventory->RemoveItem(slot); //Instantly remove garbage when picked up
			
			//CheckItems(inventory, BB);
			break;
		}

	}
	

	
	

	return EBTNodeResult::Succeeded;

}

bool UBTT_PickupItem::IsInventoryFull(UInventoryComponent* inventory)
{
	if (!inventory)return false;
	for (auto item : inventory->GetInventory())
	{
		if (!item)return false;
	}
	return true;
}

void UBTT_PickupItem::CheckItems(UInventoryComponent* inventory, UBlackboardComponent* bb)
{
	bool hasFood = false;
	bool hasMedkit = false;
	bool hasWeapon = false;
	
	for (auto item : inventory->GetInventory())
	{
		if (!item) continue;
		auto type = item->GetItemType();
		switch (type)
		{
		case EItemType::Shotgun:
			hasWeapon = true;
			break;
		case EItemType::Pistol:
			hasWeapon = true;
			break;
		case EItemType::Medkit:
			hasMedkit = true;
			break;
		case EItemType::Food:
			hasFood = true;
			break;
		case EItemType::Garbage:
			break;
		}
		
	}
	
	bb->SetValueAsBool(FName("hasMedkit"), hasMedkit);
	bb->SetValueAsBool(FName("hasWeapon"), hasWeapon);
	bb->SetValueAsBool(FName("hasFood"), hasFood);
	
	
}
