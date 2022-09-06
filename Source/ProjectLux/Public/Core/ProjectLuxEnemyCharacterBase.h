// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilitySystemInterface.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ProjectLuxEnemyCharacterBase.generated.h"

// Forward declarations
class UAbilitySystemComponent;
class UBehaviorTree;

UCLASS()
class PROJECTLUX_API AProjectLuxEnemyCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AProjectLuxEnemyCharacterBase();

	/** BehaviorTree which is used in the possessing AIController::RunBehaviorTree() method. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character|AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	/**
	 * Returns the AbilitySystemComponent (ASC) of this Actor.
	 * @return The AbilitySystemComponent (ASC) of this Actor.
	 */
	UFUNCTION(BlueprintCallable, Category = "Character|Abilities")
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	/** Runs logic when this Character is possessed. */
	virtual void PossessedBy(AController* NewController) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** The AbilitySystemComponent of this Actor. */
	UPROPERTY()
	UAbilitySystemComponent* AbilitySystemComponent;
};
