// Copyright TinyAlmonds (Alex Nördemann)

#pragma once

#include "AbilitySystemInterface.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameplayTagContainer.h"

#include "ProjectLuxEnemyCharacterBase.generated.h"

// Forward declarations
struct FOnAttributeChangeData;
class UProjectLuxAbilitySystemComponent;
class UGameplayAbility;
class UGameplayEffect;
class UProjectLuxCharacterAttributeSet;

UCLASS()
class PROJECTLUX_API AProjectLuxEnemyCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AProjectLuxEnemyCharacterBase();

	/** GameplayEffect which is used to initialize the AttributeSet of the character. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character|Abilities")
	TSubclassOf<UGameplayEffect> AttributeSetInitEffect;

	/** Default GameplayAbilities for this character. These will be added on character possession. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character|Abilities")
	TArray<TSubclassOf<UGameplayAbility>> DefaultAbilities;

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

	/** Reacts to Health attribute changes and calls the Blueprint event.*/
	virtual void OnHealthChanged(const FOnAttributeChangeData& Data);

	/** Event for the Blueprint class to react to Health changes of the AttributeSet.*/
	UFUNCTION(BlueprintNativeEvent, Category = "Character|Attributes", DisplayName = "On Health Changed")
	void HealthChanged(float OldValue, float NewValue);

	/** Implementation for the Native Event reacting to Health changes of the AttributeSet.*/
	virtual void HealthChanged_Implementation(float OldValue, float NewValue);

	/**
	 * Reacts to changes of the ASC, when the Dead GameplayTag is applied or removed.
	 * @param ChangedTag - The changed GameplayTag. Unused, only for interface compliance.
	 * @param NewCount - The new count of this tag.
	 */
	virtual void OnDeadTagChanged(const FGameplayTag ChangedTag, int32 NewCount);

	/**
	 * Event for the Blueprint class to react to Dead GameplayTag changes of the ASC.
	 * @param NewCount - The new count of this tag.
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Character|Abilities", DisplayName = "On DeadTag Changed")
	void DeadTagChanged(int32 NewCount);

	/**
	 * Implementation of the Native Event reacting to Dead GameplayTag changes of the ASC.
	 * @param NewCount - The new count of this tag.
	 */
	virtual void DeadTagChanged_Implementation(int32 NewCount);

	/** The AbilitySystemComponent of this Actor. */
	UPROPERTY()
	UProjectLuxAbilitySystemComponent* AbilitySystemComponent;
	
	/** List of attributes modified by the ASC. */
	UPROPERTY()
	UProjectLuxCharacterAttributeSet* AttributeSet;

	/** Member holding the tag which describes the death of the character. */
	FGameplayTag DeadTag;
};
