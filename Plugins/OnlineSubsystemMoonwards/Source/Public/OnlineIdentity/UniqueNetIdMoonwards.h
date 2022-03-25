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
	
	explicit FUniqueNetIdMoonwards(const FString& UserId)
		: UserId(UserId)
	{
	}

	FUniqueNetIdMoonwards(const FUniqueNetId& Src, const FString& UserId)
		: FUniqueNetId(Src),
		  UserId(UserId)
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
