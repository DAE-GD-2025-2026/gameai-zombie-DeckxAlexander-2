// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_CanFindWeapon.h"
#include "AIController.h"
#include "Items/BaseItem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DeckxAlexanderZombieRuntime/StudentPerceptor.h"

UBTT_CanFindWeapon::UBTT_CanFindWeapon()
{
}

EBTNodeResult::Type UBTT_CanFindWeapon::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)return EBTNodeResult::Failed;
	APawn* Survivor = Cast<APawn>(AIController->GetPawn());
	if (!Survivor)return EBTNodeResult::Failed;
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB) return  EBTNodeResult::Failed;
	UStudentPerceptor* perceptor = Survivor->FindComponentByClass<UStudentPerceptor>();
	if (!perceptor) return EBTNodeResult::Failed;
	
	auto neededItem = perceptor->GetClosestItemOfTypeInMemory(EItemType::Pistol);
	if (neededItem)
	{
		BB->SetValueAsBool(FName("NeedWeapon"), true);
		return EBTNodeResult::Succeeded;	
	}
	
	neededItem = perceptor->GetClosestItemOfTypeInMemory(EItemType::Shotgun);
	if (neededItem)
	{
		BB->SetValueAsBool(FName("NeedWeapon"), true);
		return EBTNodeResult::Succeeded;	
	}
	
	BB->SetValueAsBool(FName("NeedWeapon"), false);
	return EBTNodeResult::Succeeded;
}
