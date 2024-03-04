// Copyright TinyAlmonds (Alex Noerdemann)

#include "Core/AbilitySystem/PLAttackDamageExecution.h"

#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "GenericPlatform/GenericPlatformMath.h"

#include "Core/AbilitySystem/PLCharacterAttributeSet.h"

/**
 * Struct to capture all needed attributes for the damage calculation from character attacks.
 */
struct PLAttackDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(RawDamage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(FearDamageMultiplier);
	DECLARE_ATTRIBUTE_CAPTUREDEF(FearResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(AngerDamageMultiplier);
	DECLARE_ATTRIBUTE_CAPTUREDEF(AngerResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(JoyDamageMultiplier);
	DECLARE_ATTRIBUTE_CAPTUREDEF(JoyResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(SadnessDamageMultiplier);
	DECLARE_ATTRIBUTE_CAPTUREDEF(SadnessResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(TrustDamageMultiplier);
	DECLARE_ATTRIBUTE_CAPTUREDEF(TrustResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(LoathingDamageMultiplier);
	DECLARE_ATTRIBUTE_CAPTUREDEF(LoathingResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(AnticipationDamageMultiplier);
	DECLARE_ATTRIBUTE_CAPTUREDEF(AnticipationResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(SupriseDamageMultiplier);
	DECLARE_ATTRIBUTE_CAPTUREDEF(SupriseResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ReceivedDamage);

	PLAttackDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPLCharacterAttributeSet, RawDamage, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPLCharacterAttributeSet, Armor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPLCharacterAttributeSet, FearDamageMultiplier, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPLCharacterAttributeSet, FearResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPLCharacterAttributeSet, AngerDamageMultiplier, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPLCharacterAttributeSet, AngerResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPLCharacterAttributeSet, JoyDamageMultiplier, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPLCharacterAttributeSet, JoyResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPLCharacterAttributeSet, SadnessDamageMultiplier, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPLCharacterAttributeSet, SadnessResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPLCharacterAttributeSet, TrustDamageMultiplier, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPLCharacterAttributeSet, TrustResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPLCharacterAttributeSet, LoathingDamageMultiplier, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPLCharacterAttributeSet, LoathingResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPLCharacterAttributeSet, AnticipationDamageMultiplier, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPLCharacterAttributeSet, AnticipationResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPLCharacterAttributeSet, SupriseDamageMultiplier, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPLCharacterAttributeSet, SupriseResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPLCharacterAttributeSet, ReceivedDamage, Target, false);
	}
};

static const PLAttackDamageStatics &AttackDamageStatics()
{
	static PLAttackDamageStatics AttDmgStatics;
	return AttDmgStatics;
}

UPLAttackDamageExecution::UPLAttackDamageExecution()
{
	RelevantAttributesToCapture.Add(AttackDamageStatics().RawDamageDef);
	RelevantAttributesToCapture.Add(AttackDamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(AttackDamageStatics().FearDamageMultiplierDef);
	RelevantAttributesToCapture.Add(AttackDamageStatics().FearResistanceDef);
	RelevantAttributesToCapture.Add(AttackDamageStatics().AngerDamageMultiplierDef);
	RelevantAttributesToCapture.Add(AttackDamageStatics().AngerResistanceDef);
	RelevantAttributesToCapture.Add(AttackDamageStatics().JoyDamageMultiplierDef);
	RelevantAttributesToCapture.Add(AttackDamageStatics().JoyResistanceDef);
	RelevantAttributesToCapture.Add(AttackDamageStatics().SadnessDamageMultiplierDef);
	RelevantAttributesToCapture.Add(AttackDamageStatics().SadnessResistanceDef);
	RelevantAttributesToCapture.Add(AttackDamageStatics().TrustDamageMultiplierDef);
	RelevantAttributesToCapture.Add(AttackDamageStatics().TrustResistanceDef);
	RelevantAttributesToCapture.Add(AttackDamageStatics().LoathingDamageMultiplierDef);
	RelevantAttributesToCapture.Add(AttackDamageStatics().LoathingResistanceDef);
	RelevantAttributesToCapture.Add(AttackDamageStatics().AnticipationDamageMultiplierDef);
	RelevantAttributesToCapture.Add(AttackDamageStatics().AnticipationResistanceDef);
	RelevantAttributesToCapture.Add(AttackDamageStatics().SupriseDamageMultiplierDef);
	RelevantAttributesToCapture.Add(AttackDamageStatics().SupriseResistanceDef);
	RelevantAttributesToCapture.Add(AttackDamageStatics().ReceivedDamageDef);
}

void UPLAttackDamageExecution::Execute_Implementation(const FGameplayEffectCustomExecutionParameters &ExecutionParams, OUT FGameplayEffectCustomExecutionOutput &OutExecutionOutput) const
{
	const FGameplayEffectSpec &Spec = ExecutionParams.GetOwningSpec();

	// Gather the tags from the source and target as that can affect which buffs should be used
	const FGameplayTagContainer *SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer *TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	// calculate the raw damage the target will receive
	float RawDamageTargetReceives{0.0f};
	float RawDamageSource{0.0f};
	float ArmorTarget{0.0f};
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(AttackDamageStatics().RawDamageDef, EvaluationParameters, RawDamageSource);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(AttackDamageStatics().ArmorDef, EvaluationParameters, ArmorTarget);

	RawDamageTargetReceives = (5.0f * RawDamageSource * RawDamageSource) / (ArmorTarget + (5.0f * RawDamageSource)) + 1.0f;

	// calculate the emotional damage the target will receive
	float EmotionalDamageTargetReceives{0.0f};
	float FearDamageMultiplierSource{0.0f};
	float FearResistanceTarget{1.0f};
	float AngerDamageMultiplierSource{0.0f};
	float AngerResistanceTarget{1.0f};
	float JoyDamageMultiplierSource{0.0f};
	float JoyResistanceTarget{1.0f};
	float SadnessDamageMultiplierSource{0.0f};
	float SadnessResistanceTarget{1.0f};
	float TrustDamageMultiplierSource{0.0f};
	float TrustResistanceTarget{1.0f};
	float LoathingDamageMultiplierSource{0.0f};
	float LoathingResistanceTarget{1.0f};
	float AnticipationDamageMultiplierSource{0.0f};
	float AnticipationResistanceTarget{1.0f};
	float SupriseDamageMultiplierSource{0.0f};
	float SupriseResistanceTarget{1.0f};
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(AttackDamageStatics().FearDamageMultiplierDef, EvaluationParameters, FearDamageMultiplierSource);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(AttackDamageStatics().AngerDamageMultiplierDef, EvaluationParameters, AngerDamageMultiplierSource);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(AttackDamageStatics().JoyDamageMultiplierDef, EvaluationParameters, JoyDamageMultiplierSource);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(AttackDamageStatics().SadnessDamageMultiplierDef, EvaluationParameters, SadnessDamageMultiplierSource);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(AttackDamageStatics().TrustDamageMultiplierDef, EvaluationParameters, TrustDamageMultiplierSource);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(AttackDamageStatics().LoathingDamageMultiplierDef, EvaluationParameters, LoathingDamageMultiplierSource);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(AttackDamageStatics().AnticipationDamageMultiplierDef, EvaluationParameters, AnticipationDamageMultiplierSource);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(AttackDamageStatics().SupriseDamageMultiplierDef, EvaluationParameters, SupriseDamageMultiplierSource);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(AttackDamageStatics().FearResistanceDef, EvaluationParameters, FearResistanceTarget);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(AttackDamageStatics().AngerResistanceDef, EvaluationParameters, AngerResistanceTarget);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(AttackDamageStatics().JoyResistanceDef, EvaluationParameters, JoyResistanceTarget);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(AttackDamageStatics().SadnessResistanceDef, EvaluationParameters, SadnessResistanceTarget);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(AttackDamageStatics().TrustResistanceDef, EvaluationParameters, TrustResistanceTarget);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(AttackDamageStatics().LoathingResistanceDef, EvaluationParameters, LoathingResistanceTarget);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(AttackDamageStatics().AnticipationResistanceDef, EvaluationParameters, AnticipationResistanceTarget);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(AttackDamageStatics().SupriseResistanceDef, EvaluationParameters, SupriseResistanceTarget);

	EmotionalDamageTargetReceives = CalculateEmotionalDamageForEmotion(FearResistanceTarget, FearDamageMultiplierSource, RawDamageSource) +
									CalculateEmotionalDamageForEmotion(AngerResistanceTarget, AngerDamageMultiplierSource, RawDamageSource) +
									CalculateEmotionalDamageForEmotion(JoyResistanceTarget, JoyDamageMultiplierSource, RawDamageSource) +
									CalculateEmotionalDamageForEmotion(SadnessResistanceTarget, SadnessDamageMultiplierSource, RawDamageSource) +
									CalculateEmotionalDamageForEmotion(TrustResistanceTarget, TrustDamageMultiplierSource, RawDamageSource) +
									CalculateEmotionalDamageForEmotion(LoathingResistanceTarget, LoathingDamageMultiplierSource, RawDamageSource) +
									CalculateEmotionalDamageForEmotion(AnticipationResistanceTarget, AnticipationDamageMultiplierSource, RawDamageSource) +
									CalculateEmotionalDamageForEmotion(SupriseResistanceTarget, SupriseDamageMultiplierSource, RawDamageSource);

	// calculate the total damage and apply to target
	float TotalDamageTargetReceives = RawDamageTargetReceives + EmotionalDamageTargetReceives;
	if (TotalDamageTargetReceives > 0.0f)
	{
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(AttackDamageStatics().ReceivedDamageProperty, EGameplayModOp::Additive, TotalDamageTargetReceives));

		// trigger conditional effect to apply immunity from attack ability to prevent multiple hits in one swing
		OutExecutionOutput.MarkConditionalGameplayEffectsToTrigger();
	}
}

float UPLAttackDamageExecution::CalculateEmotionalDamageForEmotion(float EmotionResistanceTarget, float EmotionDamageMultiplierSource, float RawDamageSource)
{
	float ResistanceDiff{1.0f - EmotionResistanceTarget};

	if (FGenericPlatformMath::Abs(ResistanceDiff) < 0.00001f)
	{
		return 0.0f;
	}
	else
	{
		return ResistanceDiff * EmotionDamageMultiplierSource * RawDamageSource;
	}
}