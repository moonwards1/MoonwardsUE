// 

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/PlayerState.h"

#include "UObject/Object.h"
#include "MWPlayerStateExtended.generated.h"

/**
 * 
 */
UCLASS(DisplayName = "PlayerState Extended")
class MULTIPLAYEREXTENSIONMOONWARDS_API AMWPlayerStateExtended : public APlayerState
{
	GENERATED_BODY()
public:
	virtual void OnSetUniqueId() override;
};
