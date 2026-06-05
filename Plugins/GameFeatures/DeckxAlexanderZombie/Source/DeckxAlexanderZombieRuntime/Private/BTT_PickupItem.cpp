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

	
	if (!item || !IsValid(item)) //incase the item is invalid
	{
		BB->SetValueAsObject(FName("TargetItem"), nullptr);
		BB->SetValueAsBool(FName("SeenItem"), false);
		return EBTNodeResult::Failed;
	}
	perceptor->RemoveItemFromMemory(item);
	
	if (inventory->GetInventory().Contains(item)) //Incase the item is already equipped
	{
		BB->SetValueAsObject(FName("TargetItem"), nullptr);
		return EBTNodeResult::Succeeded;
	}
	
	if (IsInventoryFull(inventory)) //Remove duplicate when inventory is full
	{
		int slotThrowAway = CheckPriorityThrowSlot(inventory,BB);
		
		if (slotThrowAway == -1) return EBTNodeResult::Failed;
		
		FString name = StaticEnum<EItemType>()->GetNameStringByValue(static_cast<int64>(inventory->GetInventory()[slotThrowAway]->GetItemType()));
		GEngine->AddOnScreenDebugMessage(INDEX_NONE,5.f,FColor::Purple,FString::Printf(TEXT("Item removed %s"),*name), true);
		
		inventory->RemoveItem(slotThrowAway); 

	}
	
	for (int slot{}; slot < inventory->GetInventory().Num(); ++slot)
	{
		if (inventory->GrabItem(slot, item))
		{
			BB->SetValueAsObject(FName("TargetItem"), nullptr);
			if (item->GetItemType() == EItemType::Garbage) inventory->RemoveItem(slot); //Instantly remove garbage when picked up
			CheckItems(inventory, BB);
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
		if (!item || !IsValid(item))return false;
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
	bb->SetValueAsBool(FName("HasMedkit"), hasMedkit);
	bb->SetValueAsBool(FName("hasWeapon"), hasWeapon);
	bb->SetValueAsBool(FName("HasFood"), hasFood);
}

int UBTT_PickupItem::CheckPriorityThrowSlot(UInventoryComponent* inventory, UBlackboardComponent* bb)
{
	int medkitAmount{};
	int foodAmount{};
	int pistolAmount{};
	int shotgunAmount{};
	for (int slot{}; slot < inventory->GetInventory().Num(); ++slot)
	{
		auto item = inventory->GetInventory()[slot];
		switch (item->GetItemType())
		{
		case EItemType::Shotgun:
			shotgunAmount++;
			if (shotgunAmount > 1) return slot;
			break;
		case EItemType::Pistol:
			pistolAmount++;
			if (pistolAmount > 1) return slot;
			break;
		case EItemType::Medkit:
			medkitAmount++;
			if (medkitAmount > 1) return slot;
			break;
		case EItemType::Food:
			foodAmount++;
			if (foodAmount > 1) return slot;
			break;
		}
	}
	return -1;
}
