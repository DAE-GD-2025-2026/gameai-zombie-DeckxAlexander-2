// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_WanderAround.h"
#include "AIController.h"

UBTT_WanderAround::UBTT_WanderAround()
{
	NodeName = "WanderAround";
	bNotifyTick = true;
}

FVector UBTT_WanderAround::CalculateDesiredVelocity()
{
	
	FVector2D AgentPos = FVector2D(m_OwnerPawn->GetActorLocation());
	FVector2D Forward2D(
		m_OwnerPawn->GetActorForwardVector().X,
		m_OwnerPawn->GetActorForwardVector().Y
	);

	if (Forward2D.IsNearlyZero())
	{
		Forward2D = FVector2D(1.f, 0.f);
	}

	FVector2D CircleCenter = AgentPos + Forward2D * m_OffsetDistance;
	m_WanderAngle += FMath::FRandRange(-m_MaxAngleChange, m_MaxAngleChange );
	FVector2D Displacement(
		FMath::Cos(m_WanderAngle),
		FMath::Sin(m_WanderAngle)
	);
	
	Displacement *= m_Radius;
	FVector2D WanderTarget = CircleCenter + Displacement;
	FVector2D DesiredVelocity = (WanderTarget - AgentPos).GetSafeNormal();
	
	FVector wanderVelocity = {DesiredVelocity.X, DesiredVelocity.Y, 0};
	FVector avoidanceVelocity = CalculateObstacleAvoidance();
	FVector purgeAvoidanceVelocity = PurgeAvoidance();
	
	
	FVector finalVelocity = wanderVelocity + (avoidanceVelocity) + (purgeAvoidanceVelocity*5.f);
	finalVelocity.Z = 0;
	finalVelocity.Normalize();
	
	return finalVelocity;
}









