// Fill out your copyright notice in the Description page of Project Settings.

#include "AIController.h"
#include "StudentPerceptor.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Items/BaseItem.h"
#include "UpdateBlackboardService.h"
#include "Common/InventoryComponent.h"

UUpdateBlackboardService::UUpdateBlackboardService()
{
	NodeName = TEXT("Update Blackboard");

	Interval = 0.5f;
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
	CheckSurvivorStats(Survivor, BB);
	CheckPurgeZones(perceptor, BB);
	auto inventory = Survivor->GetComponentByClass<UInventoryComponent>();
	CheckItemsInInventory(inventory, BB);
	
	
	
	
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

void UUpdateBlackboardService::CheckPurgeZones(UStudentPerceptor* perceptor, UBlackboardComponent* bb)
{
	bb->SetValueAsObject(FName("TargetPurge"), perceptor->GetClosestPurgeZone());
}

void UUpdateBlackboardService::CheckHouses(UStudentPerceptor* perceptor, UBlackboardComponent* bb)
{
	bb->SetValueAsBool(FName("SeenHouse"), perceptor->GetSeenHouses().Num() > 0);
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
	bb->SetValueAsBool(FName("ZombieClose"), zombieClose);
}

void UUpdateBlackboardService::CheckSurvivorStats(APawn* survivorPawn, UBlackboardComponent* bb)
{
	auto healthComponent = survivorPawn->GetComponentByClass<UHealthComponent>();
	bool lowHealth = false;
	bool lowStamina = false;
	
	float health = healthComponent->GetHealth();
	if (healthComponent && health < 3.f)
	{
		lowHealth = true;
	}
	
	auto staminaComponent = survivorPawn->GetComponentByClass<UStaminaComponent>();
	
	if (staminaComponent && staminaComponent->GetCurrentStamina() < 3.f)
	{
		lowStamina = true;
	}
	bb->SetValueAsBool(FName("LowHealth"), lowHealth);
	bb->SetValueAsBool(FName("LowStamina"), lowStamina);
	
	//Check Health for unknown damaged
	if (health < m_PreviousHealth && bb->GetValueAsBool(FName("ZombieClose")) == false)
	{
		bb->SetValueAsBool(FName("LookAround"), true);
	}
	m_PreviousHealth = health;
}

void UUpdateBlackboardService::CheckItemsInInventory(UInventoryComponent* inventory, UBlackboardComponent* bb)
{
	bool hasFood = false;
	bool hasMedkit = false;
	bool hasWeapon = false;
	

	for (auto item : inventory->GetInventory())
	{
		if (!item) continue;
		auto type = item->GetItemType();
		switch (type)
		{
		case EItemType::Shotgun:
			hasWeapon = true;
			break;
		case EItemType::Pistol:
			hasWeapon = true;
			break;
		case EItemType::Medkit:
			hasMedkit = true;
			break;
		case EItemType::Food:
			hasFood = true;
			break;
		case EItemType::Garbage:
			break;
		}
		
	}
	
	bb->SetValueAsBool(FName("HasMedkit"), hasMedkit);
	bb->SetValueAsBool(FName("hasWeapon"), hasWeapon);
	bb->SetValueAsBool(FName("HasFood"), hasFood);
}



