// Copyright TinyAlmonds (Alex Noerdemann)
#include "Core/Component/TrackActor/PLTrackActorComponent.h"

#include "Kismet/GameplayStatics.h"

UPLTrackActorComponent::UPLTrackActorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPLTrackActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
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

EProjectLuxTrackActorMode UPLTrackActorComponent::GetTrackMode() const
{
	return TrackActorSettings.Mode;
}

void UPLTrackActorComponent::SetTrackMode(EProjectLuxTrackActorMode Mode)
{
	PreviousTrackMode = TrackActorSettings.Mode;
	TrackActorSettings.Mode = Mode;

	if (PreviousTrackMode != TrackActorSettings.Mode)
	{
		TrackModeChanged();
	}
}

const AActor *UPLTrackActorComponent::GetTrackActor() const
{
	return TrackActorSettings.Actor.Get();
}

void UPLTrackActorComponent::SetTrackActor(AActor *ActorToTrack)
{
	TrackActorSettings.Actor = ActorToTrack;
}

void UPLTrackActorComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UPLTrackActorComponent::TrackModeChanged()
{
	// invalidate tracked actor, since when we change form "Player to Actor" the TrackActor will not be automatically set
	TrackActorSettings.Actor = nullptr;
}

void UPLTrackActorComponent::SetTrackActorForPlayerTrackMode()
{
	APawn *Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (Player)
	{
		// since ACharacter is a AActor it is safe to cast without further checks
		SetTrackActor(Cast<AActor>(Player));
	}
}
