// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_LookAroundDeckxAlexander.generated.h"

class UStudentPerceptorDeckxAlexander;
class UBlackboardComponent;
/**
 * 
 */
UCLASS()
class DECKXALEXANDERZOMBIERUNTIME_API UBTT_LookAroundDeckxAlexander : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTT_LookAroundDeckxAlexander();


protected:
	
	virtual EBTNodeResult::Type ExecuteTask(
	UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory) override;
	
	virtual void TickTask(
	UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory,
	float DeltaSeconds) override;
	
private:
	float m_AccumulatedYaw = 0.f;
	float m_TargetYaw = 360.f;
	
	APawn* m_OwnerPawn{};
	UStudentPerceptorDeckxAlexander* m_Perceptor;
	UBlackboardComponent* m_Blackboard;
};
