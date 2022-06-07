#pragma once

#include "CoreMinimal.h"
#include "HttpModule.h"
#include "OnlineSubsystem.h"

#include "Kismet/BlueprintAsyncActionBase.h"

#include "APIModel/LoginRequestData.h"

#include "Interfaces/OnlineIdentityInterface.h"

#include "LoginRequestAsyncAction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FLoginResult, int32, LocalUserNum, FString const&, UserId, FString const&, DisplayName);

/**
 * 
 */
UCLASS()
class ULoginRequestAsyncAction : public UBlueprintAsyncActionBase
{
private:
	GENERATED_BODY()
	FHttpModule* Http;

	FString LoginToken;
	UObject* WorldContextObject;

	FOnLoginCompleteDelegate OnLoginRequestCompleted;

	IOnlineIdentityPtr OnlineIdentity;
public:
	explicit ULoginRequestAsyncAction(const FObjectInitializer& ObjectInitializer);
	
	UPROPERTY(BlueprintAssignable, DisplayName = "Succeeded")
	FLoginResult OnSucceeded;
	
	UPROPERTY(BlueprintAssignable, DisplayName = "Failed")
	FLoginResult OnFailed;
	
	/**
	 * Create a new Online Subsystem Session.
	 * @param LoginToken
	 * @return 
	 */
	UFUNCTION(BlueprintCallable, Category = "Online Subsystem Moonwards | Identity", meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"))
	static ULoginRequestAsyncAction* TryLogin(UObject* WorldContextObject, FString const LoginToken);

	
protected:
	virtual void Activate() override;
	void TryLogin();
	void OnResponseReceived(int32 LocalUserNum, bool bSuccessful, const FUniqueNetId& UserId, const FString& Error) const;

};
