
#pragma once

#include "LoginRequestData.generated.h"

USTRUCT(BlueprintType)
struct FLoginRequestData
{
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintReadOnly)
	FString Id;
	
	UPROPERTY(BlueprintReadOnly)
	FString Username;

	FLoginRequestData() {}
};
