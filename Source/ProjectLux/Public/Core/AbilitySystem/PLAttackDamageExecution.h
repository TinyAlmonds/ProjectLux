// Copyright TinyAlmonds (Alex Noerdemann)
#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"

#include "PLAttackDamageExecution.generated.h"

/**
 * Damage calculation for attack abilities, which considers physical as well as emotional attributes.
 */
UCLASS()
class PROJECTLUX_API UPLAttackDamageExecution : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	UPLAttackDamageExecution();
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters &ExecutionParams, OUT FGameplayEffectCustomExecutionOutput &OutExecutionOutput) const override;

private:
	static float CalculateEmotionalDamageForEmotion(float EmotionResistanceTarget, float EmotionDamageMultiplierSource, float RawDamageSource);
};
