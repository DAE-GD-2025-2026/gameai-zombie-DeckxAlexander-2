// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BTT_BaseSteeringDeckxAlexander.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_WanderAroundDeckxAlexander.generated.h"

/**
 * 
 */
UCLASS()
class DECKXALEXANDERZOMBIERUNTIME_API UBTT_WanderAroundDeckxAlexander : public UBTT_BaseSteeringDeckxAlexander
{
	GENERATED_BODY()
public:
	UBTT_WanderAroundDeckxAlexander();


protected:
	
	virtual EBTNodeResult::Type ExecuteTask(
	UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory) override;
	
	virtual FVector CalculateDesiredVelocity() override;


private:
	float m_OffsetDistance = 150.f;
	float m_Radius = 100.f;
	float m_MaxAngleChange = FMath::DegreesToRadians(60.f);
	float m_WanderAngle = 0.f;
	

	
};
