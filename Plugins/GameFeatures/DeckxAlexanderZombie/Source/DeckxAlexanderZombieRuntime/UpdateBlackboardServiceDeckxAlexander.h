// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "UpdateBlackboardServiceDeckxAlexander.generated.h"

/**
 * 
 */

class UStudentPerceptorDeckxAlexander;
class UBlackboardComponent;
UCLASS()
class DECKXALEXANDERZOMBIERUNTIME_API UUpdateBlackboardServiceDeckxAlexander : public UBTService
{
	GENERATED_BODY()
	
public:
	UUpdateBlackboardServiceDeckxAlexander();

protected:
	
	virtual void TickNode(
		UBehaviorTreeComponent& OwnerComp,
		uint8* NodeMemory,
		float DeltaSeconds) override;
	
	
	void CheckItems(UStudentPerceptorDeckxAlexander* perceptor, UBlackboardComponent* bb);
	void CheckPurgeZones(UStudentPerceptorDeckxAlexander* perceptor, UBlackboardComponent* bb);
	void CheckHouses(UStudentPerceptorDeckxAlexander* perceptor, UBlackboardComponent* bb);
	void CheckZombies(UStudentPerceptorDeckxAlexander* perceptor, UBlackboardComponent* bb, APawn* survivorLoc);
	
	void CheckSurvivorStats(APawn* survivorPawn, UBlackboardComponent* bb);
	void CheckItemsInInventory(UInventoryComponent* inventory, UBlackboardComponent* bb);
	
private:
	float m_PreviousHealth{10};
	bool m_ZombieClose{false};
	UStudentPerceptorDeckxAlexander* m_Perceptor;
	
};
