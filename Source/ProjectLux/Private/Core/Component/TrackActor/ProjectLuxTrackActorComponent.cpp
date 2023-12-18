// Copyright TinyAlmonds (Alex Noerdemann)
#include "Core/Component/TrackActor/ProjectLuxTrackActorComponent.h"

#include "Kismet/GameplayStatics.h"

UProjectLuxTrackActorComponent::UProjectLuxTrackActorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UProjectLuxTrackActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// We have to query the player on every tick, since the player can die and therefore the actor changes.
	if (TrackActorSettings.Mode == EProjectLuxTrackActorMode::Player)
	{
		SetTrackActorForPlayerTrackMode();
	}

	AActor *OwnerActor = GetOwner();
	if (TrackActorSettings.Actor.IsValid(false, false) && IsValid(OwnerActor))
	{
		const FVector LookAt = TrackActorSettings.Actor.Get()->GetActorLocation() - OwnerActor->GetActorLocation();
		const FRotator LookAtRotation = FMath::RInterpTo(OwnerActor->GetActorRotation(), LookAt.Rotation(), DeltaTime, TrackActorSettings.RotationRate);

		OwnerActor->SetActorRotation(LookAtRotation);
	}
}

EProjectLuxTrackActorMode UProjectLuxTrackActorComponent::GetTrackMode() const
{
	return TrackActorSettings.Mode;
}

void UProjectLuxTrackActorComponent::SetTrackMode(EProjectLuxTrackActorMode Mode)
{
	PreviousTrackMode = TrackActorSettings.Mode;
	TrackActorSettings.Mode = Mode;

	if (PreviousTrackMode != TrackActorSettings.Mode)
	{
		TrackModeChanged();
	}
}

const AActor *UProjectLuxTrackActorComponent::GetTrackActor() const
{
	return TrackActorSettings.Actor.Get();
}

void UProjectLuxTrackActorComponent::SetTrackActor(AActor *ActorToTrack)
{
	TrackActorSettings.Actor = ActorToTrack;
}

void UProjectLuxTrackActorComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UProjectLuxTrackActorComponent::TrackModeChanged()
{
	// invalidate tracked actor, since when we change form "Player to Actor" the TrackActor will not be automatically set
	TrackActorSettings.Actor = nullptr;
}

void UProjectLuxTrackActorComponent::SetTrackActorForPlayerTrackMode()
{
	APawn *Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (Player)
	{
		// since ACharacter is a AActor it is safe to cast without further checks
		SetTrackActor(Cast<AActor>(Player));
	}
}
