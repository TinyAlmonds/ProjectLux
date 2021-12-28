// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/ProjectLuxCharacterAttributeSet.h"
#include "Misc/Optional.h"

UProjectLuxCharacterAttributeSet::UProjectLuxCharacterAttributeSet() : 
	Health{1.0f},
	MaxHealth{1.0f},
	RawDamage{1.0f},
	Armor{1.0f},
	MinEmotionalDamageMultiplier{0.0f},
	MinEmotionalResistance{0.0f},
	MaxEmotionalResistance{1.0f},
	FearDamageMultiplier{0.0f},
	FearResistance{1.0f},
	AngerDamageMultiplier{0.0f},
	AngerResistance{1.0f},
	JoyDamageMultiplier{0.0f},
	JoyResistance{1.0f},
	SadnessDamageMultiplier{0.0f},
	SadnessResistance{1.0f},
	TrustDamageMultiplier{0.0f},
	TrustResistance{1.0f},
	LoathingDamageMultiplier{0.0f},
	LoathingResistance{1.0f},
	AnticipationDamageMultiplier{0.0f},
	AnticipationResistance{1.0f},
	SupriseDamageMultiplier{0.0f},
	SupriseResistance{1.0f}
{}

void UProjectLuxCharacterAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& OutNewValue)
{
	Super::PreAttributeChange(Attribute, OutNewValue);

	// clamp value
	{
		TOptional<float> ClampedValue = ClampAttributeValue(Attribute, OutNewValue);
		if (ClampedValue)
		{
			OutNewValue = ClampedValue.GetValue();
		}
	}
}

void UProjectLuxCharacterAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	// TODO: react to base value changes (was not necessary for the initialization process)
	// TODO: clamp value changes
}

TOptional<float> UProjectLuxCharacterAttributeSet::ClampAttributeValue(const FGameplayAttribute& Attribute, const float& Value)
{
	if (Attribute == GetHealthAttribute())
	{		
		return TOptional<float>{FMath::Clamp(Value, 0.0f, GetMaxHealth())};
	}
	else if (Attribute == GetMaxHealthAttribute())
	{
		return TOptional<float>{FMath::Max(1.0f, Value)};
	}
	else if (Attribute == GetRawDamageAttribute())
	{
		return TOptional<float>{FMath::Max(0.0f, Value)};
	}
	else if (Attribute == GetArmorAttribute())
	{
		return TOptional<float>{FMath::Max(0.0f, Value)};
	}
	else if (Attribute.AttributeName.Contains(FString("Resistance")))
	{
		if ((Attribute == GetMinEmotionalResistanceAttribute()) || (Attribute == GetMaxEmotionalResistanceAttribute()))
		{
			return TOptional<float>{FMath::Clamp(Value, 0.0f, 1.0f)};
		}
		else
		{
			return TOptional<float>{FMath::Clamp(Value, GetMinEmotionalResistance(), GetMaxEmotionalResistance())};
		}
	}
	else if (Attribute.AttributeName.Contains(FString("DamageMultiplier")))
	{
		if (Attribute == GetMinEmotionalDamageMultiplierAttribute())
		{
			return TOptional<float>{FMath::Max(0.0f, Value)};
		}
		else
		{
			return TOptional<float>{FMath::Max(GetMinEmotionalDamageMultiplier(), Value)};
		}
	}
	else
	{
		return TOptional<float>{};
	}
}