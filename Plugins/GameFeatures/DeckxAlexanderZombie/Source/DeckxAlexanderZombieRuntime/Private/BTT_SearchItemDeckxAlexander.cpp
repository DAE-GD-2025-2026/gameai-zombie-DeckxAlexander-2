// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_SearchItemDeckxAlexander.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Common/InventoryComponent.h"
#include "DeckxAlexanderZombieRuntime/StudentPerceptorDeckxAlexander.h"

UBTT_SearchItemDeckxAlexander::UBTT_SearchItemDeckxAlexander()
{
}

EBTNodeResult::Type UBTT_SearchItemDeckxAlexander::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)return EBTNodeResult::Failed;
	APawn* Survivor = Cast<APawn>(AIController->GetPawn());
	if (!Survivor)return EBTNodeResult::Failed;
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB) return  EBTNodeResult::Failed;
	UStudentPerceptorDeckxAlexander* perceptor = Survivor->FindComponentByClass<UStudentPerceptorDeckxAlexander>();
	if (!perceptor) return EBTNodeResult::Failed;
	
	UInventoryComponent* inventory = Survivor->FindComponentByClass<UInventoryComponent>();
	TArray<EItemType> Priorities = GetPriorities(BB, inventory);

	
	if (perceptor->GetSeenItems().Num() == 0)
	{
		BB->SetValueAsBool(FName("SeenItem"), false);
		return EBTNodeResult::Failed;
	}
	
	ABaseItem* closestItem = nullptr;
	TArray<ABaseItem*> itemsSkipped{};
	
	for (AActor* actor : perceptor->GetSeenItems() )
	{
		auto item = Cast<ABaseItem>(actor);
		
		if (!item || !IsValid(item))
		{
			itemsSkipped.Add(item);
			continue;
		}
		
		if (Priorities.Contains(item->GetItemType()))
		{
			closestItem = item;
			break;
		}
		
		if ( !IsInventoryFull(inventory))
		{
			if (Priorities.IsEmpty())
			{
				closestItem = item;
				break;
			}
			
			closestItem = item;
			
		}

		
		if (closestItem != item) itemsSkipped.Add(item);
	}
	
	for (auto item : itemsSkipped)
	{
		if (!item || !IsValid(item))
		{
			perceptor->RemoveItemFromMemory(item);
			continue;
		}
		perceptor->SetItemInMemory(item);
	}
	
	
	if (closestItem)
	{
		BB->SetValueAsObject(FName("TargetItem"), closestItem);
		BB->SetValueAsVector(FName("TargetLocation"), closestItem->GetActorLocation());
		return EBTNodeResult::Succeeded;
	}
	
	BB->SetValueAsBool(FName("SeenItem"), false);
	
	return EBTNodeResult::Failed;
}

bool UBTT_SearchItemDeckxAlexander::IsInventoryFull(UInventoryComponent* inventory)
{
	if (!inventory)return false;
	for (auto item : inventory->GetInventory())
	{
		if (!item)return false;
	}
	return true;
}

int UBTT_SearchItemDeckxAlexander::GetInventoryItemCount(UInventoryComponent* inventory)
{
	if (!inventory) return 0;
	int count = 0;
	for (auto item : inventory->GetInventory())
	{
		if (item) count++;
	}
	return count;
}

TArray<EItemType> UBTT_SearchItemDeckxAlexander::GetPriorities( UBlackboardComponent* bb, UInventoryComponent* inventory)
{
	if (!bb) return {};
	
	//Prioritize based survivor state
	if (bb->GetValueAsBool(FName("ZombieClose")) && !bb->GetValueAsBool(FName("hasWeapon")))
	{
		return {EItemType::Pistol, EItemType::Shotgun};
	}
	if (bb->GetValueAsBool(FName("LowHealth")) && !bb->GetValueAsBool(FName("HasMedkit")))
	{
		return {EItemType::Medkit};
	}
	if (bb->GetValueAsBool(FName("LowStamina")) && !bb->GetValueAsBool(FName("HasFood")))
	{
		return {EItemType::Food};
	}
	
	//Prioritize based on inventory state
	int itemCount = GetInventoryItemCount(inventory);
	
	bool needsWeapon = (itemCount > 2 && !bb->GetValueAsBool(FName("hasWeapon")) );
	bool needsMedkit = ( itemCount > 2 && !bb->GetValueAsBool(FName("HasMedkit")) );
	bool needsFood = ( itemCount > 2 && !bb->GetValueAsBool(FName("HasFood")) ) ;
	
	
	TArray<EItemType> prioritiesReturn;
	
	if (needsMedkit) prioritiesReturn.Add(EItemType::Medkit);
	if (needsFood) prioritiesReturn.Add(EItemType::Food);
	if (needsWeapon)
	{
		prioritiesReturn.Add(EItemType::Pistol);
		prioritiesReturn.Add(EItemType::Shotgun);
	}
	
	
	FString ItemsString;

	for (const EItemType Item : prioritiesReturn)
	{
		if (!ItemsString.IsEmpty())
		{
			ItemsString += TEXT(", ");
		}

		ItemsString += StaticEnum<EItemType>()->GetNameStringByValue(
			static_cast<int64>(Item)
		);
	}

	GEngine->AddOnScreenDebugMessage(
		20,
		1.f,
		FColor::Blue,
		FString::Printf(TEXT("Items Priority: %s"), *ItemsString)
	);
	
	
	return prioritiesReturn;

}

