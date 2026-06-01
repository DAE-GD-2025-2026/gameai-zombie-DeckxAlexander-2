// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Perception/AISense_Damage.h"
#include "StudentPerceptor.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DECKXALEXANDERZOMBIERUNTIME_API UStudentPerceptor : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UStudentPerceptor();
	
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
	
	
	const TArray<AActor*>& GetSeenItems() {return m_SeenItems;};
	const TArray<AActor*>& GetItemMemory() {return m_ItemsInMemory;};
	const TArray<AActor*>& GetSeenHouses() {return m_SeenUnExploredHouses;};
	const TArray<AActor*>& GetSeenZombies() {return m_SeenZombies;};
	
	void RemoveItemFromMemory(AActor* Actor);
	void SetItemInMemory(AActor* Actor);
	
	void SetHouseAsExplored(AActor* Actor);
	bool CheckIfInHouse();
	
	AActor* GetCurrentHouse() const {return m_CurrentHouse;}
	
private:
	
	//Itens
	void AddItemToSeen(AActor* Actor);

	TArray<AActor*> m_SeenItems{};
	TArray<AActor*> m_ItemsInMemory{};
	
	
	//Houses
	void AddHouseToMemory(AActor* Actor);

	
	TArray<AActor*> m_SeenUnExploredHouses{};
	TArray<AActor*> m_ExploredHouses{};
	
	AActor* m_CurrentHouse{};
	
	//Zombies
	void AddZombieToMemory(AActor* Actor);
	TArray<AActor*> m_SeenZombies{};
};
