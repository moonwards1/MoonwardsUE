// Fill out your copyright notice in the Description page of Project Settings.


#include "MWNCreateOnlinesession.h"


UMWNCreateOnlinesession* UMWNCreateOnlinesession::CreateSession(const UObject* WorldContextObject, const bool SomeInputVariables)
{
	UMWNCreateOnlinesession* BlueprintNode = NewObject<UMWNCreateOnlinesession>();
	BlueprintNode->WorldContextObject = (UObject*) WorldContextObject;
	BlueprintNode->InputValue = SomeInputVariables;
	return BlueprintNode;
}

void UMWNCreateOnlinesession::Activate()
{
	if (InputValue)
	{
		Succeeded.Broadcast();
	}
	else {
		Failed.Broadcast();
	}
}
