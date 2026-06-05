// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Common/InventoryComponent.h"
#include "BTT_PickupItem.generated.h"

/**
 * 
 */
UCLASS()
class DECKXALEXANDERZOMBIERUNTIME_API UBTT_PickupItem : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTT_PickupItem();

protected:
	
	virtual EBTNodeResult::Type ExecuteTask(
	UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory) override;
	
	bool IsInventoryFull(UInventoryComponent* inventory);
	void CheckItems(UInventoryComponent* inventory, UBlackboardComponent* bb);
	int CheckPriorityThrowSlot(UInventoryComponent* inventory, UBlackboardComponent* bb);
};
