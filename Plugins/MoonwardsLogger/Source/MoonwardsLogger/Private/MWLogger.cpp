#include "MWLogger.h"
#include "MoonwardsLogger.h"

#if WITH_EDITOR
#include "Engine.h"
#include "Logging/MessageLog.h"
#include "Misc/UObjectToken.h"
#include "Kismet2/KismetEditorUtilities.h"
#endif

void UMWLogger::OnBringToFocus(TSharedRef<IMessageToken> const& Token)
{
#if WITH_EDITOR
	if (Token->GetType() == EMessageToken::Object)
	{
		const TSharedRef<FUObjectToken> UObjectToken = StaticCastSharedRef<FUObjectToken>(Token);
		if (UObjectToken->GetObject().IsValid())
		{
			FKismetEditorUtilities::BringKismetToFocusAttentionOnObject(UObjectToken->GetObject().Get());
		}
	}
#endif
}

void UMWLogger::LogMessage(FString const Message, UObject const* Caller, EMWLogCategory const LogCategory, EMWLogMessageSeverity const Severity, bool const bLogToViewport)
{
#if WITH_EDITOR
	if (bLogToViewport)
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.f, FColor::Blue, Message);
	}

	auto const TokenizedMessage = FTokenizedMessage::Create(static_cast<EMessageSeverity::Type>(Severity), FText::FromString(Message));

	// @TODO: Clean this up. Oughta be some better ways to get the stack & check for validity.

	if (IsValid(Caller))
	{
		// Add the caller token
		TokenizedMessage->AddToken(FTextToken::Create(FText::FromString("- at")))
			->AddToken(FUObjectToken::Create(Caller->GetClass())
				->OnMessageTokenActivated(FOnMessageTokenActivated::CreateStatic(&UMWLogger::OnBringToFocus)));

		auto const Frame = FBlueprintContextTracker::Get().GetScriptStack().Last();

		UBlueprintGeneratedClass* GraphClass = Cast<UBlueprintGeneratedClass>(Caller->GetClass());

		// Calculate the node location based on it's offset in bp generated code
		const int32 BreakpointOffset = Frame->Code - Frame->Node->Script.GetData() - 1;
		UEdGraphNode const* BlueprintNode = GraphClass->GetDebugData().FindSourceNodeFromCodeLocation(Frame->Node, BreakpointOffset, true);

		if (IsValid(BlueprintNode))
		{
			TokenizedMessage->AddToken(FTextToken::Create(FText::FromString("at node")))
				->AddToken(FUObjectToken::Create(BlueprintNode, BlueprintNode->GetNodeTitle(ENodeTitleType::Type::ListView))
					->OnMessageTokenActivated(FOnMessageTokenActivated::CreateStatic(&UMWLogger::OnBringToFocus))
				);
		}
	}

	// Code to dump a callstack, if we ever need to.
	//FDebug::DumpStackTraceToLog(*FString("---STACK TRACE---\n"), ELogVerbosity::Error);

	FMessageLog(FName("RizingTidez")).AddMessage(TokenizedMessage);
#endif 
}

void UMWLogger::LogMessage(FString Message, FString CallerName, EMWLogCategory LogCategory, EMWLogMessageSeverity Severity, bool bLogToViewport)
{
#if WITH_EDITOR
	Message = Message + " - at " + CallerName;

	if (bLogToViewport)
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.f, FColor::Blue, Message);
	}

	auto const TokenizedMessage = FTokenizedMessage::Create(static_cast<EMessageSeverity::Type>(Severity), FText::FromString(Message));

	FMessageLog(FName("RizingTidez")).AddMessage(TokenizedMessage);
#endif 
}
