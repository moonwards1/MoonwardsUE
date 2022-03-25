// #pragma once
// #ifndef ApiRouter.h_H_
// #define ApiRouter.h_H_
// 
// #endif

#pragma once
#include "OSMWCommon.h"

// Todo: See if this needs to be finished or discarded.
class FMoonwardsApiUrlBuilder
{
private:
	FString ApiUrl = MOONWARDS_API_URL; 
public:
	FString GetRouteUrl(FString route);
};
