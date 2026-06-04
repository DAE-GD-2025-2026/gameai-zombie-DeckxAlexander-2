// Fill out your copyright notice in the Description page of Project Settings.

#include "AIController.h"
#include "StudentPerceptor.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Items/BaseItem.h"
#include "UpdateBlackboardService.h"

UUpdateBlackboardService::UUpdateBlackboardService()
{
	NodeName = TEXT("Update Blackboard");

	Interval = 1.0f;
	RandomDeviation = 0.2f;
}

void UUpdateBlackboardService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	//Get Controller and Blackboard
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)return;
	APawn* Survivor = Cast<APawn>(AIController->GetPawn());
	if (!Survivor)return;
	UBlackboardComponent* BB =OwnerComp.GetBlackboardComponent();
	if (!BB) return;
	UStudentPerceptor* perceptor = Survivor->FindComponentByClass<UStudentPerceptor>();
	if (!perceptor) return;
	
	CheckItems(perceptor, BB);
	CheckHouses(perceptor, BB);
	CheckZombies(perceptor, BB, Survivor->GetActorLocation());

	
	
	
	
}

void UUpdateBlackboardService::CheckItems(UStudentPerceptor* perceptor, UBlackboardComponent* bb)
{
	bb->SetValueAsBool(FName("SeenItem"), !perceptor->GetSeenItems().IsEmpty());
	
	GEngine->AddOnScreenDebugMessage(
		9,
		1.f,
		FColor::Blue,
		FString::Printf(
			TEXT("Items Seen: %.2f"),
			static_cast<float>(perceptor->GetSeenItems().Num())
		)
	);
	
}

void UUpdateBlackboardService::CheckHouses(UStudentPerceptor* perceptor, UBlackboardComponent* bb)
{
	if (perceptor->GetSeenHouses().Num() > 0) bb->SetValueAsBool(FName("SeenHouse"), true);
	bool newInHouse = perceptor->CheckIfInHouse();
	bb->SetValueAsBool(FName("isInHouse"), newInHouse);
	bb->SetValueAsObject(FName("CurrentHouse"), perceptor->GetCurrentHouse());
	
}

void UUpdateBlackboardService::CheckZombies(UStudentPerceptor* perceptor, UBlackboardComponent* bb, FVector survivorLoc)
{
	bool zombieClose = false;
	for (auto actor : perceptor->GetSeenZombies())
	{
		if (FVector::DistSquared(actor->GetActorLocation(), survivorLoc) > (600*600)) continue;
		zombieClose = true;

	}
	bb->SetValueAsBool(FName("zombieClose"), zombieClose);
}

