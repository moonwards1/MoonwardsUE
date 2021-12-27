// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "MWNCreateOnlinesession.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCreateSessionResult);

/**
 * 
 */
UCLASS()
class MOONWARDSNETWORKING_API UMWNCreateOnlinesession : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FCreateSessionResult Succeeded;
	
	UPROPERTY(BlueprintAssignable)
	FCreateSessionResult Failed;

	UFUNCTION(BlueprintCallable, Category = "Moonwards Networking | Sessions", meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"))
	static UMWNCreateOnlinesession* CreateSession(const UObject* WorldContextObject, const bool SomeInputVariables);

private:
	UObject* WorldContextObject;
	bool InputValue;
	virtual void Activate() override;

};
