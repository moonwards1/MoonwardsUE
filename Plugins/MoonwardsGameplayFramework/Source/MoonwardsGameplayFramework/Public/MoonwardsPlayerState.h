// 

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/PlayerState.h"

#include "UObject/Object.h"
#include "MoonwardsPlayerState.generated.h"

/**
 * 
 */
UCLASS(DisplayName = "PlayerState Extended")
class MOONWARDSGAMEPLAYFRAMEWORK_API AMoonwardsPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	virtual void OnSetUniqueId() override;
};
