// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "UpdateBlackboardService.generated.h"

/**
 * 
 */

class UStudentPerceptor;
class UBlackboardComponent;
UCLASS()
class DECKXALEXANDERZOMBIERUNTIME_API UUpdateBlackboardService : public UBTService
{
	GENERATED_BODY()
	
public:
	UUpdateBlackboardService();

protected:
	
	virtual void TickNode(
		UBehaviorTreeComponent& OwnerComp,
		uint8* NodeMemory,
		float DeltaSeconds) override;
	
	
	void CheckItems(UStudentPerceptor* perceptor, UBlackboardComponent* bb);
	void CheckPurgeZones(UStudentPerceptor* perceptor, UBlackboardComponent* bb);
	void CheckHouses(UStudentPerceptor* perceptor, UBlackboardComponent* bb);
	void CheckZombies(UStudentPerceptor* perceptor, UBlackboardComponent* bb, APawn* survivorLoc);
	
	void CheckSurvivorStats(APawn* survivorPawn, UBlackboardComponent* bb);
	void CheckItemsInInventory(UInventoryComponent* inventory, UBlackboardComponent* bb);
	
private:
	float m_PreviousHealth;
	bool m_ZombieClose;
	UStudentPerceptor* m_Perceptor;
	
};
