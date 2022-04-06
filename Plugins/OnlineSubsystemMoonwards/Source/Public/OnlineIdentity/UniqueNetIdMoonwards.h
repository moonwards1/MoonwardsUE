// #pragma once
// #ifndef FUniqueNetIdMoonwards.h_H_
// #define FUniqueNetIdMoonwards.h_H_
// 
// #endif

#pragma once
#include "APIModel/LoginRequestData.h"

class FUniqueNetIdMoonwards : public FUniqueNetId
{
public:

	FUniqueNetIdMoonwards() {};
	virtual ~FUniqueNetIdMoonwards() override = default;

	FUniqueNetIdMoonwards(const FString& UserId)
		: UserId(UserId)
	{
	}

	FUniqueNetIdMoonwards(const FUniqueNetId& Src)
		: FUniqueNetId(Src)
	{
	}

	FString UserId;

	virtual FName GetType() const override;
	virtual const uint8* GetBytes() const override;
	virtual int32 GetSize() const override;
	virtual bool IsValid() const override;
	virtual FString ToString() const override;
	virtual FString ToDebugString() const override;

protected:
	virtual bool Compare(const FUniqueNetId& Other) const override;
};

typedef TSharedPtr<FUniqueNetIdMoonwards, ESPMode::Fast> FUniqueNetIdMoonwardsPtr;
typedef TSharedRef<FUniqueNetIdMoonwards, ESPMode::Fast> FUniqueNetIdMoonwardsRef;