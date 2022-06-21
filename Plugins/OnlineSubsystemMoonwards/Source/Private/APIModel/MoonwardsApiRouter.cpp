
#include "APIModel/MoonwardsApiRouter.h"

FString FMoonwardsApiUrlBuilder::GetRouteUrl(FString route)
{
	return ApiUrl + route;
}
