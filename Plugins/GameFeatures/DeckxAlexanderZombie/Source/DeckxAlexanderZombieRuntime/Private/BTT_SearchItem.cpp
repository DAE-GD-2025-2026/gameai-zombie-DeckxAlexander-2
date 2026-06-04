// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_SearchItem.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Common/InventoryComponent.h"
#include "DeckxAlexanderZombieRuntime/StudentPerceptor.h"

UBTT_SearchItem::UBTT_SearchItem()
{
}

EBTNodeResult::Type UBTT_SearchItem::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)return EBTNodeResult::Failed;
	APawn* Survivor = Cast<APawn>(AIController->GetPawn());
	if (!Survivor)return EBTNodeResult::Failed;
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB) return  EBTNodeResult::Failed;
	UStudentPerceptor* perceptor = Survivor->FindComponentByClass<UStudentPerceptor>();
	if (!perceptor) return EBTNodeResult::Failed;
	

	//Check Priorities
	TArray<EItemType> Priorities;
	//float health = BB->GetValueAsFloat(FName("Health"));
	//float stamina = BB->GetValueAsFloat(FName("Stamina"));
	//bool hasWeapon =  BB->GetValueAsBool(FName("HasWeapon"));
	//if (health < 30.f) priorities.Add(EItemType::Medkit);
	//if (stamina < 50.f) priorities.Add(EItemType::Food);
	//if (hasWeapon == false)
	//{
		//priorities.Add(EItemType::Pistol);
		//priorities.Add(EItemType::Shotgun);
	//}
	
	
	
	
	UInventoryComponent* inventory = Survivor->FindComponentByClass<UInventoryComponent>();
	
	if (perceptor->GetSeenItems().Num() == 0)
	{
		BB->SetValueAsBool(FName("SeenItem"), false);
		return EBTNodeResult::Failed;
	}
	
	ABaseItem* closestItem = Cast<ABaseItem>(perceptor->GetSeenItems()[0]);
	TArray<ABaseItem*> itemsSkipped{};
	
	for (AActor* actor : perceptor->GetSeenItems() )
	{
		auto item = Cast<ABaseItem>(actor);
		if ((Priorities.IsEmpty()&& !IsInventoryFull(inventory)) || Priorities.Contains(item->GetItemType()))
		{
			closestItem = item;
			break;
			
		}
		itemsSkipped.Add(item);
		continue;
		
	}
	
	for (auto item : itemsSkipped)
	{
		perceptor->SetItemInMemory(item);
	}
	

	
	/*if (!Priorities.IsEmpty())
	{
		//Inacse needed item not seen
		auto memory = perceptor->GetItemMemory();
		for (auto itemtype : Priorities)
		{
			if (AActor** result = memory.FindByPredicate([itemtype](const AActor* Actor){auto item = Cast<ABaseItem>(Actor);
				return item->GetItemType() == itemtype;}))
			{
				auto itemresult = Cast<ABaseItem>(*result);
				BB->SetValueAsObject(FName("TargetItem"), itemresult);
				BB->SetValueAsVector(FName("TargetLocation"), itemresult->GetActorLocation());
				return EBTNodeResult::Succeeded;
			}
		}
	}*/
	
	if (closestItem && !IsInventoryFull(inventory))
	{
		BB->SetValueAsObject(FName("TargetItem"), closestItem);
		BB->SetValueAsVector(FName("TargetLocation"), closestItem->GetActorLocation());
		return EBTNodeResult::Succeeded;
	}
	
	BB->SetValueAsBool(FName("SeenItem"), false);
	
	return EBTNodeResult::Failed;
}

bool UBTT_SearchItem::IsInventoryFull(UInventoryComponent* inventory)
{
	if (!inventory)return false;
	for (auto item : inventory->GetInventory())
	{
		if (!item)return false;
	}
	return true;
}

