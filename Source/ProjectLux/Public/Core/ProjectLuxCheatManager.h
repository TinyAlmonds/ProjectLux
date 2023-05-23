// Copyright TinyAlmonds (Alex Noerdemann)

#pragma once

#include "CoreMinimal.h"

#include "BUICheatManagerBase.h"
#include "ProjectLuxCheatManager.generated.h"

/**
 * Custom CheatManager class of the project (e.g. for custom console commands).
 */
UCLASS(meta = (CheatPrefix = "projectlux."))
class PROJECTLUX_API UProjectLuxCheatManager : public UBUICheatManagerBase
{
	GENERATED_BODY()

public:
	/**
	 * Teleports the player to the specified PlayerStart.
	 * @param PlayerStartTag - The tag of the PlayerStart the player should be teleported.
	 * @note If the tag is not given or "None", the player will be teleported to the last Checkpoint/PlayerStart of the MainSaveGame.
	 */
	UFUNCTION(exec, BlueprintNativeEvent, meta = (Cheat = "TeleportToPlayerStart"))
	void TeleportToPlayerStart(FName PlayerStartTag);

	/**
	 * Implementation for the Native Event TeleportToPlayerStart(). Does nothing for now, since we expect an implementation in Blueprint.
	 * @param PlayerStartTag - The tag of the PlayerStart the player should be teleported.
	 */
	virtual void TeleportToPlayerStart_Implementation(FName PlayerStartTag);
};
