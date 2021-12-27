// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "CoreMinimal.h"
#include "ProjectLuxCharacterAttributeSet.generated.h"

// Uses macros from AttributeSet.h for accessing and initializing attributes
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * Class holding all attributes needed for the abilities of the AProjectLuxCharacter.
 */
UCLASS()
class PROJECTLUX_API UProjectLuxCharacterAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	UProjectLuxCharacterAttributeSet();
	virtual ~UProjectLuxCharacterAttributeSet() = default;

	/** Health of the character.*/
	UPROPERTY(BlueprintReadOnly, Category = "Physical")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UProjectLuxCharacterAttributeSet, Health)

	/** Maximum health of the character. Used for clamping the maximum value of the Health attribute.*/
	UPROPERTY(BlueprintReadOnly, Category = "Physical")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UProjectLuxCharacterAttributeSet, MaxHealth)

	/** Raw/base physical damage the character can inflict to something.*/
	UPROPERTY(BlueprintReadOnly, Category = "Physical")
	FGameplayAttributeData RawDamage;
	ATTRIBUTE_ACCESSORS(UProjectLuxCharacterAttributeSet, RawDamage)

	/** Physical armor value of the character, which reduces physical raw/base damage taken by the character.*/
	UPROPERTY(BlueprintReadOnly, Category = "Physical")
	FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS(UProjectLuxCharacterAttributeSet, Armor)

	/** Minimum multiplier value of emotional damage inflicted by the character. Used for setting the minimal value of the related attributes.*/
	UPROPERTY(BlueprintReadOnly, Category = "Emotional")
	FGameplayAttributeData MinEmotionalDamageMultiplier;
	ATTRIBUTE_ACCESSORS(UProjectLuxCharacterAttributeSet, MinEmotionalDamageMultiplier)

	/** Minimum resistance value against emotional damage of the character. Used for clamping related attributes.*/
	UPROPERTY(BlueprintReadOnly, Category = "Emotional")
	FGameplayAttributeData MinEmotionalResistance;
	ATTRIBUTE_ACCESSORS(UProjectLuxCharacterAttributeSet, MinEmotionalResistance)

	/** Maximum resistance value against emotional damage of the character. Used for clamping related attributes.*/
	UPROPERTY(BlueprintReadOnly, Category = "Emotional")
	FGameplayAttributeData MaxEmotionalResistance;
	ATTRIBUTE_ACCESSORS(UProjectLuxCharacterAttributeSet, MaxEmotionalResistance)

	/** Multiplier value of emotional fear damage inflicted by the character.*/
	UPROPERTY(BlueprintReadOnly, Category = "Emotional")
	FGameplayAttributeData FearDamageMultiplier;
	ATTRIBUTE_ACCESSORS(UProjectLuxCharacterAttributeSet, FearDamageMultiplier)

	/** Resistance value against fear damage of the character.*/
	UPROPERTY(BlueprintReadOnly, Category = "Emotional")
	FGameplayAttributeData FearResistance;
	ATTRIBUTE_ACCESSORS(UProjectLuxCharacterAttributeSet, FearResistance)

	/** Multiplier value of emotional anger damage inflicted by the character.*/
	UPROPERTY(BlueprintReadOnly, Category = "Emotional")
	FGameplayAttributeData AngerDamageMultiplier;
	ATTRIBUTE_ACCESSORS(UProjectLuxCharacterAttributeSet, AngerDamageMultiplier)

	/** Resistance value against anger damage of the character.*/
	UPROPERTY(BlueprintReadOnly, Category = "Emotional")
	FGameplayAttributeData AngerResistance;
	ATTRIBUTE_ACCESSORS(UProjectLuxCharacterAttributeSet, AngerResistance)

	/** Multiplier value of emotional joy damage inflicted by the character.*/
	UPROPERTY(BlueprintReadOnly, Category = "Emotional")
	FGameplayAttributeData JoyDamageMultiplier;
	ATTRIBUTE_ACCESSORS(UProjectLuxCharacterAttributeSet, JoyDamageMultiplier)

	/** Resistance value against joy damage of the character.*/
	UPROPERTY(BlueprintReadOnly, Category = "Emotional")
	FGameplayAttributeData JoyResistance;
	ATTRIBUTE_ACCESSORS(UProjectLuxCharacterAttributeSet, JoyResistance)

	/** Multiplier value of emotional sadness damage inflicted by the character.*/
	UPROPERTY(BlueprintReadOnly, Category = "Emotional")
	FGameplayAttributeData SadnessDamageMultiplier;
	ATTRIBUTE_ACCESSORS(UProjectLuxCharacterAttributeSet, SadnessDamageMultiplier)

	/** Resistance value against sadness damage of the character.*/
	UPROPERTY(BlueprintReadOnly, Category = "Emotional")
	FGameplayAttributeData SadnessResistance;
	ATTRIBUTE_ACCESSORS(UProjectLuxCharacterAttributeSet, SadnessResistance)

	/** Multiplier value of emotional trust damage inflicted by the character.*/
	UPROPERTY(BlueprintReadOnly, Category = "Emotional")
	FGameplayAttributeData TrustDamageMultiplier;
	ATTRIBUTE_ACCESSORS(UProjectLuxCharacterAttributeSet, TrustDamageMultiplier)

	/** Resistance value against trust damage of the character.*/
	UPROPERTY(BlueprintReadOnly, Category = "Emotional")
	FGameplayAttributeData TrustResistance;
	ATTRIBUTE_ACCESSORS(UProjectLuxCharacterAttributeSet, TrustResistance)

	/** Multiplier value of emotional loathing damage inflicted by the character.*/
	UPROPERTY(BlueprintReadOnly, Category = "Emotional")
	FGameplayAttributeData LoathingDamageMultiplier;
	ATTRIBUTE_ACCESSORS(UProjectLuxCharacterAttributeSet, LoathingDamageMultiplier)

	/** Resistance value against loathing damage of the character.*/
	UPROPERTY(BlueprintReadOnly, Category = "Emotional")
	FGameplayAttributeData LoathingResistance;
	ATTRIBUTE_ACCESSORS(UProjectLuxCharacterAttributeSet, LoathingResistance)

	/** Multiplier value of emotional anticipation damage inflicted by the character.*/
	UPROPERTY(BlueprintReadOnly, Category = "Emotional")
	FGameplayAttributeData AnticipationDamageMultiplier;
	ATTRIBUTE_ACCESSORS(UProjectLuxCharacterAttributeSet, AnticipationDamageMultiplier)

	/** Resistance value against anticipation damage of the character.*/
	UPROPERTY(BlueprintReadOnly, Category = "Emotional")
	FGameplayAttributeData AnticipationResistance;
	ATTRIBUTE_ACCESSORS(UProjectLuxCharacterAttributeSet, AnticipationResistance)

	/** Multiplier value of emotional suprise damage inflicted by the character.*/
	UPROPERTY(BlueprintReadOnly, Category = "Emotional")
	FGameplayAttributeData SupriseDamageMultiplier;
	ATTRIBUTE_ACCESSORS(UProjectLuxCharacterAttributeSet, SupriseDamageMultiplier)

	/** Resistance value against suprise damage of the character.*/
	UPROPERTY(BlueprintReadOnly, Category = "Emotional")
	FGameplayAttributeData SupriseResistance;
	ATTRIBUTE_ACCESSORS(UProjectLuxCharacterAttributeSet, SupriseResistance)
};
