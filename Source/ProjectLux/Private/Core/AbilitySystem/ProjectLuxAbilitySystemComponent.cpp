// Copyright TinyAlmonds (Alex Noerdemann)

#include "Core/AbilitySystem/ProjectLuxAbilitySystemComponent.h"

bool UProjectLuxAbilitySystemComponent::CanActivateAbilityOfClass(const TSubclassOf<UGameplayAbility> &InAbilityToCheck)
{
	const UGameplayAbility *const InAbilityCDO = InAbilityToCheck.GetDefaultObject();

	for (const FGameplayAbilitySpec &ActivatableSpec : ActivatableAbilities.Items)
	{
		if (ActivatableSpec.Ability == InAbilityCDO)
		{
			const FGameplayAbilitySpec *const Spec = FindAbilitySpecFromHandle(ActivatableSpec.Handle);
			if (!Spec)
			{
				return false;
			}
			if (Spec->PendingRemove || Spec->RemoveAfterActivation)
			{
				return false;
			}

			const UGameplayAbility *const Ability = Spec->Ability;
			if (!Ability)
			{
				return false;
			}

			const FGameplayAbilityActorInfo *const ActorInfo = AbilityActorInfo.Get();
			if (ActorInfo == nullptr || !ActorInfo->OwnerActor.IsValid() || !ActorInfo->AvatarActor.IsValid())
			{
				return false;
			}

			FGameplayTagContainer FailureTags;
			return Ability->CanActivateAbility(ActivatableSpec.Handle, ActorInfo, nullptr, nullptr, &FailureTags);
		}
	}

	return false;
}

UGameplayAbility *UProjectLuxAbilitySystemComponent::ActivateAbilityOfClass(const TSubclassOf<UGameplayAbility> &InAbilityToActivate, bool &OutIsInstance)
{
	if (!TryActivateAbilityByClass(InAbilityToActivate))
	{
		OutIsInstance = false;
		return nullptr;
	}

	const UGameplayAbility *const InAbilityCDO = InAbilityToActivate.GetDefaultObject();

	for (const FGameplayAbilitySpec &ActivatableSpec : ActivatableAbilities.Items)
	{
		if (ActivatableSpec.Ability == InAbilityCDO)
		{
			const FGameplayAbilitySpec *const Spec = FindAbilitySpecFromHandle(ActivatableSpec.Handle);

			// try to get the ability instance
			// Note: we need a instanced ability to be able to bind to its "Event Dispatchers"
			UGameplayAbility *AbilityInstance = Spec->GetPrimaryInstance();

			if (AbilityInstance)
			{
				OutIsInstance = true;
				return AbilityInstance;
			}
			else
			{
				// default to the CDO if we can't
				OutIsInstance = false;
				return Spec->Ability;
			}
		}
	}

	return nullptr;
}