// Copyright TinyAlmonds (Alex Noerdemann)
#pragma once

#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "CoreMinimal.h"

#include "PLCharacterAttributeSet.generated.h"

// Uses macros from AttributeSet.h for accessing and initializing attributes
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName)           \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName)               \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName)               \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

// Forward declarations
template <typename OptionalType>
struct TOptional;

/**
 * Class holding all attributes needed for the abilities of the APLCharacter.
 */
UCLASS()
class PROJECTLUX_API UPLCharacterAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	UPLCharacterAttributeSet();
	virtual ~UPLCharacterAttributeSet() = default;

	/**
	 * Called just before any modification happens to an attribute. There is no additional context provided here since anything can trigger this: Executed effects, duration based effects, effects being removed, immunity being applied, stacking rules changing, etc.
	 * @param Attribute - The attribute whose value will be changed.
	 * @param OutNewValue - The attribute's new value. This is a mutable reference so you are able to clamp the newly applied value as well.
	 */
	virtual void PreAttributeChange(const FGameplayAttribute &Attribute, float &OutNewValue) override;

	/**
	 * Called just before a GameplayEffect is executed to modify the base value of an attribute. No more changes can be made. Note: This is only called during an 'execute'. E.g., a modification to the 'base value' of an attribute. It is not called during an application of a GameplayEffect, such as a 5 ssecond +10 movement speed buff.
	 * @param Data - Data struct holding necessary information of the executed/applied GameplayEffect.
	 */
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData &Data) override;

	/** Health of the character.*/
	UPROPERTY(BlueprintReadOnly, Category = "Physical")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UPLCharacterAttributeSet, Health)

	/** Maximum health of the character. Used for clamping the maximum value of the Health attribute.*/
	UPROPERTY(BlueprintReadOnly, Category = "Physical")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UPLCharacterAttributeSet, MaxHealth)

	/** Raw/base physical damage the character can inflict to something.*/
	UPROPERTY(BlueprintReadOnly, Category = "Physical")
	FGameplayAttributeData RawDamage;
	ATTRIBUTE_ACCESSORS(UPLCharacterAttributeSet, RawDamage)

	/** Physical armor value of the character, which reduces physical raw/base damage taken by the character.*/
	UPROPERTY(BlueprintReadOnly, Category = "Physical")
	FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS(UPLCharacterAttributeSet, Armor)

	/** Minimum multiplier value of emotional damage inflicted by the character. Used for setting the minimal value of the related attributes.*/
	UPROPERTY(BlueprintReadOnly, Category = "Emotional")
	FGameplayAttributeData MinEmotionalDamageMultiplier;
	ATTRIBUTE_ACCESSORS(UPLCharacterAttributeSet, MinEmotionalDamageMultiplier)

	/** Minimum resistance value against emotional damage of the character. Used for clamping related attributes.*/
	UPROPERTY(BlueprintReadOnly, Category = "Emotional")
	FGameplayAttributeData MinEmotionalResistance;
	ATTRIBUTE_ACCESSORS(UPLCharacterAttributeSet, MinEmotionalResistance)

	/** Maximum resistance value against emotional damage of the character. Used for clamping related attributes.*/
	UPROPERTY(BlueprintReadOnly, Category = "Emotional")
	FGameplayAttributeData MaxEmotionalResistance;
	ATTRIBUTE_ACCESSORS(UPLCharacterAttributeSet, MaxEmotionalResistance)

	/** Multiplier value of emotional fear damage inflicted by the character.*/
	UPROPERTY(BlueprintReadOnly, Category = "Emotional")
	FGameplayAttributeData FearDamageMultiplier;
	ATTRIBUTE_ACCESSORS(UPLCharacterAttributeSet, FearDamageMultiplier)

	/** Resistance value against fear damage of the character.*/
	UPROPERTY(BlueprintReadOnly, Category = "Emotional")
	FGameplayAttributeData FearResistance;
	ATTRIBUTE_ACCESSORS(UPLCharacterAttributeSet, FearResistance)

	/** Multiplier value of emotional anger damage inflicted by the character.*/
	UPROPERTY(BlueprintReadOnly, Category = "Emotional")
	FGameplayAttributeData AngerDamageMultiplier;
	ATTRIBUTE_ACCESSORS(UPLCharacterAttributeSet, AngerDamageMultiplier)

	/** Resistance value against anger damage of the character.*/
	UPROPERTY(BlueprintReadOnly, Category = "Emotional")
	FGameplayAttributeData AngerResistance;
	ATTRIBUTE_ACCESSORS(UPLCharacterAttributeSet, AngerResistance)

	/** Multiplier value of emotional joy damage inflicted by the character.*/
	UPROPERTY(BlueprintReadOnly, Category = "Emotional")
	FGameplayAttributeData JoyDamageMultiplier;
	ATTRIBUTE_ACCESSORS(UPLCharacterAttributeSet, JoyDamageMultiplier)

	/** Resistance value against joy damage of the character.*/
	UPROPERTY(BlueprintReadOnly, Category = "Emotional")
	FGameplayAttributeData JoyResistance;
	ATTRIBUTE_ACCESSORS(UPLCharacterAttributeSet, JoyResistance)

	/** Multiplier value of emotional sadness damage inflicted by the character.*/
	UPROPERTY(BlueprintReadOnly, Category = "Emotional")
	FGameplayAttributeData SadnessDamageMultiplier;
	ATTRIBUTE_ACCESSORS(UPLCharacterAttributeSet, SadnessDamageMultiplier)

	/** Resistance value against sadness damage of the character.*/
	UPROPERTY(BlueprintReadOnly, Category = "Emotional")
	FGameplayAttributeData SadnessResistance;
	ATTRIBUTE_ACCESSORS(UPLCharacterAttributeSet, SadnessResistance)

	/** Multiplier value of emotional trust damage inflicted by the character.*/
	UPROPERTY(BlueprintReadOnly, Category = "Emotional")
	FGameplayAttributeData TrustDamageMultiplier;
	ATTRIBUTE_ACCESSORS(UPLCharacterAttributeSet, TrustDamageMultiplier)

	/** Resistance value against trust damage of the character.*/
	UPROPERTY(BlueprintReadOnly, Category = "Emotional")
	FGameplayAttributeData TrustResistance;
	ATTRIBUTE_ACCESSORS(UPLCharacterAttributeSet, TrustResistance)

	/** Multiplier value of emotional loathing damage inflicted by the character.*/
	UPROPERTY(BlueprintReadOnly, Category = "Emotional")
	FGameplayAttributeData LoathingDamageMultiplier;
	ATTRIBUTE_ACCESSORS(UPLCharacterAttributeSet, LoathingDamageMultiplier)

	/** Resistance value against loathing damage of the character.*/
	UPROPERTY(BlueprintReadOnly, Category = "Emotional")
	FGameplayAttributeData LoathingResistance;
	ATTRIBUTE_ACCESSORS(UPLCharacterAttributeSet, LoathingResistance)

	/** Multiplier value of emotional anticipation damage inflicted by the character.*/
	UPROPERTY(BlueprintReadOnly, Category = "Emotional")
	FGameplayAttributeData AnticipationDamageMultiplier;
	ATTRIBUTE_ACCESSORS(UPLCharacterAttributeSet, AnticipationDamageMultiplier)

	/** Resistance value against anticipation damage of the character.*/
	UPROPERTY(BlueprintReadOnly, Category = "Emotional")
	FGameplayAttributeData AnticipationResistance;
	ATTRIBUTE_ACCESSORS(UPLCharacterAttributeSet, AnticipationResistance)

	/** Multiplier value of emotional suprise damage inflicted by the character.*/
	UPROPERTY(BlueprintReadOnly, Category = "Emotional")
	FGameplayAttributeData SupriseDamageMultiplier;
	ATTRIBUTE_ACCESSORS(UPLCharacterAttributeSet, SupriseDamageMultiplier)

	/** Resistance value against suprise damage of the character.*/
	UPROPERTY(BlueprintReadOnly, Category = "Emotional")
	FGameplayAttributeData SupriseResistance;
	ATTRIBUTE_ACCESSORS(UPLCharacterAttributeSet, SupriseResistance)

	/** Temporary/Meta attribute used by the PLAttackDamageExecution to hold the final calculated damage the character will receive.*/
	UPROPERTY(BlueprintReadOnly, Category = "Misc")
	FGameplayAttributeData ReceivedDamage;
	ATTRIBUTE_ACCESSORS(UPLCharacterAttributeSet, ReceivedDamage)

private:
	/**
	 * Clamps the passed value of the passed attribute.
	 * @param Attribute - The attribute whose value should be clamped.
	 * @param Value - The attribute's value to clamp.
	 * @return An Optional with the clamped value, if the attribute is known to this AttributeSet; else an empty Optional.
	 */
	TOptional<float> ClampAttributeValue(const FGameplayAttribute &Attribute, const float &Value);
};
