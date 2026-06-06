// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BTT_BaseSteering.h"
#include "BTT_SteeringMoveTo.generated.h"

/**
 * 
 */
UCLASS()
class DECKXALEXANDERZOMBIERUNTIME_API UBTT_SteeringMoveTo : public UBTT_BaseSteering
{
	GENERATED_BODY()
public:
	UBTT_SteeringMoveTo();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsTargetHouse = false;
	

protected:
	
	virtual EBTNodeResult::Type ExecuteTask(
	UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory) override;

	virtual void TickTask(
		UBehaviorTreeComponent& OwnerComp,
		uint8* NodeMemory,
		float DeltaSeconds) override;
	
	virtual FVector CalculateDesiredVelocity() override;

private:
	TArray<FVector> m_Path{};
	int m_CurrentIndex{};
	FVector m_CurrentTargetLocation{};
	
	static void DrawPath(const TArray<FVector>& Points, UWorld* World);

	
};
