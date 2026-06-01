// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BTT_BaseSteering.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_WanderAround.generated.h"

/**
 * 
 */
UCLASS()
class DECKXALEXANDERZOMBIERUNTIME_API UBTT_WanderAround : public UBTT_BaseSteering
{
	GENERATED_BODY()
public:
	UBTT_WanderAround();


protected:
	virtual FVector CalculateDesiredVelocity() override;


private:
	float m_OffsetDistance = 150.f;
	float m_Radius = 100.f;
	float m_MaxAngleChange = FMath::DegreesToRadians(60.f);
	float m_WanderAngle = 0.f;
	

	
};
