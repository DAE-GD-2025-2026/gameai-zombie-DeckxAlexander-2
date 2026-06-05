// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_ClearMemory.generated.h"

class UStudentPerceptor;
/**
 * 
 */
UCLASS()
class DECKXALEXANDERZOMBIERUNTIME_API UBTS_ClearMemory : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTS_ClearMemory();

protected:
	
	virtual void TickNode(
		UBehaviorTreeComponent& OwnerComp,
		uint8* NodeMemory,
		float DeltaSeconds) override;
	
private:
	UStudentPerceptor* m_Perceptor{};
};
