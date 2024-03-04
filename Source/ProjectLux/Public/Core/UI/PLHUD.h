// Copyright TinyAlmonds (Alex Noerdemann)
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"

#include "PLHUD.generated.h"

/**
 * Manages UI and HUD related functionality of the game.
 */
UCLASS()
class PROJECTLUX_API APLHUD : public AHUD
{
	GENERATED_BODY()

public:
	/** Event for the Blueprint class to react to calls of showing/displaying the Pause Menu.*/
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Pause Menu", DisplayName = "On Show Pause Menu Called")
	void ShowPauseMenuCalled();

	/** Event for the Blueprint class to react to calls of showing/displaying the Main Menu.*/
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Main Menu", DisplayName = "On Show Main Menu Called")
	void ShowMainMenuCalled();
};
