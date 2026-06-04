// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include"Items/BaseItem.h"
#include "BTT_UseItem.generated.h"

class UInventoryComponent;
/**
 * 
 */
UCLASS()
class DECKXALEXANDERZOMBIERUNTIME_API UBTT_UseItem : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTT_UseItem();
		
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EItemType itemType{};
	
protected:
	
	virtual EBTNodeResult::Type ExecuteTask(
	UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory) override;
	
	int FindItem(UInventoryComponent* inventory);
};
