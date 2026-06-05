// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_BaseSteering.h"
#include "AIController.h"

UBTT_BaseSteering::UBTT_BaseSteering()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UBTT_BaseSteering::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	m_OwnerPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	m_CollisionQueryParams.AddIgnoredActor(m_OwnerPawn);
	return EBTNodeResult::InProgress;
}

void UBTT_BaseSteering::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (m_OwnerPawn == nullptr) return;
	
	FVector movementdir = CalculateDesiredVelocity();
	m_OwnerPawn->AddMovementInput(movementdir);
	RotateTowardsMovement(m_OwnerPawn, movementdir, DeltaSeconds);
	
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
}

void UBTT_BaseSteering::RotateTowardsMovement(APawn* Pawn, FVector movementdir, float DeltaTime)
{
	if (!Pawn) return;
	
	const FRotator currentRot = Pawn->GetActorRotation();
	const FRotator movementRot = movementdir.Rotation();

	const FRotator newRot =FMath::RInterpTo(currentRot,movementRot,DeltaTime,5.f);

	Pawn->SetActorRotation(newRot);
}



FVector UBTT_BaseSteering::CalculateObstacleAvoidance()
{
	FVector pawnPosition = m_OwnerPawn->GetActorLocation();
	FVector forwardVector = m_OwnerPawn->GetActorForwardVector();
	forwardVector.Z = 0.f;
	forwardVector.Normalize();

	constexpr float lookAheadDistance = 100.f;
	constexpr float sphereRadius = 10.f;

	FVector avoidanceVector = FVector::ZeroVector;

	avoidanceVector += CalculateAvoidanceInDirection(pawnPosition,forwardVector,lookAheadDistance,5.f);

	avoidanceVector += CalculateAvoidanceInDirection(pawnPosition,forwardVector.RotateAngleAxis(-20.f, FVector::UpVector),lookAheadDistance,3.f);

	avoidanceVector += CalculateAvoidanceInDirection(pawnPosition,forwardVector.RotateAngleAxis(20.f, FVector::UpVector),lookAheadDistance,3.f);
	avoidanceVector.Z = 0.f;

	if (!avoidanceVector.IsNearlyZero())
	{
		avoidanceVector.Normalize();
	}
	
	if (avoidanceVector.IsNearlyZero())
	{
		return FVector::ZeroVector;
	}
	
	return avoidanceVector;
	
}

FVector UBTT_BaseSteering::CalculateAvoidanceInDirection(const FVector& position, const FVector& direction, float lookAheadDistance, float weight)
{
	FHitResult hitResult;
	
	const bool bHit = GetWorld()->LineTraceSingleByChannel(hitResult,position,position + direction * lookAheadDistance,
		ECC_WorldStatic,m_CollisionQueryParams);
	
	
	if (!bHit) return FVector::ZeroVector;
	
	DrawDebugSphere(
		GetWorld(),
		position + direction * lookAheadDistance,
		50.f,
		12,
		bHit ? FColor::Red : FColor::Green,
		false,
		0.f
	);


	
	
	const float Alpha = 1.f - (hitResult.Distance / lookAheadDistance);

	return hitResult.ImpactNormal * Alpha * weight;
}
