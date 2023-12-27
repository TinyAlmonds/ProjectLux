// Copyright TinyAlmonds (Alex Noerdemann)
#pragma once

#include "CoreMinimal.h"

#include "PLTrackActorSettings.generated.h"

/** Enumeration for the mode of tracking actors. */
UENUM(BlueprintType)
enum class EPLTrackActorMode : uint8
{
	Actor,
	Player
};

/** Struct holding information about tracking an actor. */
USTRUCT(BlueprintType)
struct FPLTrackActorSet
{
	GENERATED_BODY()

	/**	The used TrackActorMode. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	EPLTrackActorMode Mode{};

	/**	The actor to track. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config", meta = (EditCondition = "Mode == EPLTrackActorMode::Actor", EditConditionHides))
	TWeakObjectPtr<AActor> Actor{nullptr};

	/**	The rotation rate indicating how fast the actor should be tracked (similar to lag). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float RotationRate{0.0F};
};
