// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_ClearMemoryDeckxAlexander.generated.h"

class UStudentPerceptorDeckxAlexander;
/**
 * 
 */
UCLASS()
class DECKXALEXANDERZOMBIERUNTIME_API UBTS_ClearMemoryDeckxAlexander : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTS_ClearMemoryDeckxAlexander();

protected:
	
	virtual void TickNode(
		UBehaviorTreeComponent& OwnerComp,
		uint8* NodeMemory,
		float DeltaSeconds) override;
	
private:
	UStudentPerceptorDeckxAlexander* m_Perceptor{};
};
