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
	// FUniqueNetIdMoonwards(): UserIdBytes(nullptr), UserIdByteSize(0)
	// {
	// };
	
	virtual ~FUniqueNetIdMoonwards() override = default;

	FUniqueNetIdMoonwards(const FString& UserId);

	FUniqueNetIdMoonwards(const FUniqueNetId& Src)
		: FUniqueNetId(Src)
	{
	}

private:
	FString UserId;
	uint8* UserIdBytes;
	int32 UserIdByteSize;

public:
	virtual FName GetType() const override;
	virtual const uint8* GetBytes() const override;
	virtual int32 GetSize() const override;
	virtual bool IsValid() const override;
	virtual FString ToString() const override;
	virtual FString ToDebugString() const override;

protected:
	virtual bool Compare(const FUniqueNetId& Other) const override;
};

typedef TSharedPtr<FUniqueNetIdMoonwards, ESPMode::ThreadSafe> FUniqueNetIdMoonwardsPtr;
typedef TSharedRef<FUniqueNetIdMoonwards, ESPMode::ThreadSafe> FUniqueNetIdMoonwardsRef;