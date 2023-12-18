// Copyright TinyAlmonds (Alex Noerdemann)
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "Core/Component/TrackActor/ProjectLuxTrackActorSettings.h"
#include "ProjectLuxTrackActorComponent.generated.h"

/**
 * ActorComponent class to track actors or the player actor.
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROJECTLUX_API UProjectLuxTrackActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	/** Sets default values for this component's properties. */
	UProjectLuxTrackActorComponent();

	/** The tick method called every frame. */
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	/**
	 * Returns the TrackActorMode.
	 * @return The TrackActorMode.
	 */
	UFUNCTION(BlueprintCallable, Category = "")
	EProjectLuxTrackActorMode GetTrackMode() const;

	/**
	 * Sets the TrackActorMode.
	 * @param Mode - The TrackActorMode to set.
	 */
	UFUNCTION(BlueprintCallable, Category = "")
	void SetTrackMode(EProjectLuxTrackActorMode Mode);

	/**
	 * Returns the tracked actor.
	 * @return The tracked actor.
	 */
	UFUNCTION(BlueprintCallable, Category = "")
	const AActor *GetTrackActor() const;

	/**
	 * Sets the tracked actor.
	 * @param ActorToTrack - The tracked actor to set.
	 */
	UFUNCTION(BlueprintCallable, Category = "")
	void SetTrackActor(AActor *ActorToTrack);

protected:
	/** Method called when the game starts. */
	virtual void BeginPlay() override;

private:
	/** The settings of the tracking. */
	UPROPERTY(EditAnywhere, Category = "Config")
	FProjectLuxTrackActorSet TrackActorSettings{};

	/** The previously used TrackMode. */
	EProjectLuxTrackActorMode PreviousTrackMode{EProjectLuxTrackActorMode::Actor};

	/** Method used when the TrackMode changed. */
	void TrackModeChanged();

	/** Method for setting the player as the tracked actor. */
	void SetTrackActorForPlayerTrackMode();
};
