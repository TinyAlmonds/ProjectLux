// Copyright TinyAlmonds (Alex Noerdemann)
#pragma once

#include "AbilitySystemComponent.h"
#include "CoreMinimal.h"

#include "PLAbilitySystemComponent.generated.h"

/**
 * Subclass of UAbilitySystemComponent to extend with game-specific features.
 */
UCLASS()
class PROJECTLUX_API UPLAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	/**
	 * Checks, whether the given ability class can be activated.
	 * @param InAbilityToCheck - Ability class to check.
	 * @return True if the given ability class can be activated; False otherwise.
	 */
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	virtual bool CanActivateAbilityOfClass(const TSubclassOf<UGameplayAbility> &InAbilityToCheck);

	/**
	 * Activates the given ability class.
	 * @param InAbilityToActivate - Ability class to activate.
	 * @param[out] OutIsInstance - Set to true if this is an instanced ability instead of a shared CDO.
	 * @return Instance of the activated ability of the given class if successful; nullptr otherwise.
	 */
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	virtual UGameplayAbility *ActivateAbilityOfClass(const TSubclassOf<UGameplayAbility> &InAbilityToActivate, bool &OutIsInstance);
};
