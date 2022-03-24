// #pragma once
// #ifndef FOnlineFactoryMoonwards.h_H_
// #define FOnlineFactoryMoonwards.h_H_
// 
// #endif

#pragma once
#include "OnlineSubsystem.h"
#include "OnlineSubsystemMoonwards.h"

/**
 * Class responsible for creating instance(s) of the subsystem
 */
class FOnlineFactoryMoonwards : public IOnlineFactory
{
private:
	/** Single instantiation of the interface */
	static FOnlineSubsystemMoonwardsPtr OnlineSub;
public:

	FOnlineFactoryMoonwards();
	virtual ~FOnlineFactoryMoonwards() override;
	virtual IOnlineSubsystemPtr CreateSubsystem(FName InstanceName) override;

};
