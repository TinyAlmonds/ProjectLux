// Copyright TinyAlmonds (Alex Noerdemann)
#pragma once

#include "CoreMinimal.h"

#include "PLChaseActorAlongSplineSettings.generated.h"

/** Enumeration for the actor mode of chasing actors along a spline. */
UENUM(BlueprintType)
enum class EPLChaseActorAlongSplineActorMode : uint8
{
	Actor,
	Player
};

/** Enumeration for the chasing mode of chasing actors along a spline. */
UENUM(BlueprintType)
enum class EPLChaseActorAlongSplineChaseMode : uint8
{
	ClosestPointOnChaseSpline,
	FollowPositionOnReferenceSpline
};

/** Struct holding information about chasing an actor along a spline. */
USTRUCT(BlueprintType)
struct FPLChaseActorAlongSplineSet
{
	GENERATED_BODY()

	/**	The used ChaseActorAlongSplineActorMode. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	EPLChaseActorAlongSplineActorMode ActorMode{};

	/**	The actor to chase. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config", meta = (EditCondition = "ActorMode == EPLChaseActorAlongSplineActorMode::Actor", EditConditionHides))
	TWeakObjectPtr<AActor> ActorToChase{nullptr};

	/**	The used ChaseActorAlongSplineChaseMode. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	EPLChaseActorAlongSplineChaseMode ChaseMode{};

	/**	The reference spline actor for the EPLChaseActorAlongSplineChaseMode::ClosestPointOnReferenceSpline state. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config", meta = (EditCondition = "ChaseMode == EPLChaseActorAlongSplineChaseMode::FollowPositionOnReferenceSpline", EditConditionHides))
	TWeakObjectPtr<AActor> ReferenceSplineActor{nullptr};

	/**	Parameter stating how fast we chase after the actor. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float ChaseSpeed{0.0F};
};
