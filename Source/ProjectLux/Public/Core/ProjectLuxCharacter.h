// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ProjectLuxCharacter.generated.h"

UCLASS()
class PROJECTLUX_API AProjectLuxCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AProjectLuxCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// TODO: add documentation
	void JumpPress();

	// TODO: add documentation
	void JumpRelease();

	// TODO: add documentation
	void MoveRight(float AxisValue);

	// TODO: add documentation
	void MoveUp(float AxisValue);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:	

};
