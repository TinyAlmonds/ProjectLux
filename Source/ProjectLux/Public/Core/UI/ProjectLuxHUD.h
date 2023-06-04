// Copyright TinyAlmonds (Alex Noerdemann)

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"

#include "ProjectLuxHUD.generated.h"

/**
 * Manages UI and HUD related functionality of the game.
 */
UCLASS()
class PROJECTLUX_API AProjectLuxHUD : public AHUD
{
	GENERATED_BODY()

public:
	/** Event for the Blueprint class to react to calls of showing/displaying the Pause Menu.*/
	UFUNCTION(BlueprintImplementableEvent, Category = "Pause Menu", DisplayName = "On Show Pause Menu Called")
	void ShowPauseMenuCalled();
};
