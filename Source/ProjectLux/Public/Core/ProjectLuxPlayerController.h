// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ProjectLuxPlayerController.generated.h"

// TODO: add documentation
/**
 * 
 */
UCLASS()
class PROJECTLUX_API AProjectLuxPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	// TODO: add documentation
	AProjectLuxPlayerController();

protected:
	// TODO: add documentation
	virtual void SetupInputComponent() override;

private:
	// TODO: add documentation
	void JumpPress();

	// TODO: add documentation
	void JumpRelease();

	// TODO: add documentation
	void MoveRight(float AxisValue);

	// TODO: add documentation
	void MoveUp(float AxisValue);
	
};
