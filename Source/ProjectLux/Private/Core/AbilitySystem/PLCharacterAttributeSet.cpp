// Copyright TinyAlmonds (Alex Noerdemann)

#include "Core/AbilitySystem/PLCharacterAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "Misc/Optional.h"

UPLCharacterAttributeSet::UPLCharacterAttributeSet() : Health{1.0f},
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
																	   SupriseResistance{1.0f},
																	   ReceivedDamage{0.0f}
{
}

void UPLCharacterAttributeSet::PreAttributeChange(const FGameplayAttribute &Attribute, float &OutNewValue)
{
	Super::PreAttributeChange(Attribute, OutNewValue);

	TOptional<float> ClampedValue = ClampAttributeValue(Attribute, OutNewValue);
	if (ClampedValue)
	{
		OutNewValue = ClampedValue.GetValue();
	}
}

void UPLCharacterAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData &Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetReceivedDamageAttribute())
	{
		// store a local copy of the amount of damage received and consume the damage
		const float DamageReceivedTmp = GetReceivedDamage();
		SetReceivedDamage(0.0f);
		if (DamageReceivedTmp > 0.0f)
		{
			// calculate health after damage received, clamp it and apply it
			TOptional<float> HealthAfterDamage{ClampAttributeValue(GetHealthAttribute(), GetHealth() - DamageReceivedTmp)};
			if (HealthAfterDamage)
			{
				SetHealth(HealthAfterDamage.GetValue());
			}
		}
	}
}

TOptional<float> UPLCharacterAttributeSet::ClampAttributeValue(const FGameplayAttribute &Attribute, const float &Value)
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