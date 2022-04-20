#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"

#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSubsystemTypes.h"
#include "Kismet/GameplayStatics.h"

#include "MWNCreateOnlineSession.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCreateSessionResult);

/**
 * 
 */
UCLASS()
class UMWNCreateOnlineSession : public UBlueprintAsyncActionBase
{
private:
	GENERATED_BODY()

public:
	explicit UMWNCreateOnlineSession(const FObjectInitializer& ObjectInitializer);
	
	UPROPERTY(BlueprintAssignable)
	FCreateSessionResult Succeeded;
	
	UPROPERTY(BlueprintAssignable)
	FCreateSessionResult Failed;
	FDelegateHandle OnCreateSessionCompleteDelegateHandle;
	FDelegateHandle OnDestroySessionCompleteDelegateHandle;
	
	/**
	 * Create a new Online Subsystem Session.
	 * @param ServerName The display name of the server
	 * @param bDestroyExistingSession Whether existing sessions should be destroyed if found. If false, creating a session will fail if a previous one exists.
	 * @return 
	 */
	UFUNCTION(BlueprintCallable, Category = "Moonwards Networking | Sessions", meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"))
	static UMWNCreateOnlineSession* CreateSession(const UObject* WorldContextObject, FString const ServerName, const bool bDestroyExistingSession);

	bool bPlayOffline = true;

	IOnlineSubsystem* OnlineSubsystem;
	IOnlineSessionPtr SessionInterface;

	//FRTOnlineSessionSearchResult JoinedSession;

protected:
	const UObject* WorldContextObject;
	// If the async node is currently running.
	bool bIsRunning = false;
    FString ServerName;
	bool bDestroyExistingSession;
	
	virtual void Activate() override;
	void CreateSession();
	void RecreateSession();

	void CreateSessionCompleted(FName SessionName, bool bSucceeded) const;
	void DestroySessionCompleted(FName SessionName, bool bSucceeded);
};
