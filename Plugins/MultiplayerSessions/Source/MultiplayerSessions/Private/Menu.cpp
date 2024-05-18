// Fill out your copyright notice in the Description page of Project Settings.


#include "Menu.h"

#include "MultiplayerSessionsSubsystem.h"
#include "Components/Button.h"
#include "Engine/Engine.h"
#include "Engine/GameInstance.h"
#include "OnlineSessionSettings.h"
#include "Interfaces//OnlineSessionInterface.h"

void UMenu::MenuSetup(int32 NumberOfPublicConnections, FString TypeOfMatch, FString LobbyPath)
{
	PathToLobby = FString::Printf(TEXT("%s?listen"), *LobbyPath);
	NumPublicConnections = NumberOfPublicConnections;
	MatchType = TypeOfMatch;

	AddToViewport();
	SetVisibility(ESlateVisibility::Visible);
	bIsFocusable = true;

	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (PlayerController)
		{
			FInputModeUIOnly InputModeData;
			InputModeData.SetWidgetToFocus(TakeWidget());
			InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

			PlayerController->SetInputMode(InputModeData);
			PlayerController->SetShowMouseCursor(true);
		}
	}

	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		MultiplayerSessionsSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();
	}

	// binding of custom delegate handlers for menu
	if (MultiplayerSessionsSubsystem)
	{
		MultiplayerSessionsSubsystem->MultiplayerOnCreateSessionComplete_Menu.AddDynamic(
			this, &ThisClass::UMenu::OnCreateSession);

		MultiplayerSessionsSubsystem->MultiplayerOnFindSessionsComplete_Menu.AddUObject(
			this, &ThisClass::OnFindSessions);

		MultiplayerSessionsSubsystem->MultiplayerOnJoinSessionComplete_Menu.AddUObject(
			this, &ThisClass::OnJoinSession);

		MultiplayerSessionsSubsystem->MultiplayerOnStartSessionComplete_Menu.AddDynamic(
			this, &ThisClass::OnStartSession);

		MultiplayerSessionsSubsystem->MultiplayerOnDestroySessionComplete_Menu.AddDynamic(
			this, &ThisClass::OnDestroySession);
	}
}

void UMenu::OnFindSessions(const TArray<FOnlineSessionSearchResult> SessionResults, bool bWasSuccessful)
{
	if (MultiplayerSessionsSubsystem == nullptr)
	{
		return;
	}

	for (auto Result : SessionResults)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				15.f,
				FColor::Silver,
				FString(Result.GetSessionIdStr()));
		}


		FString SettingsMatchTypeValue;
		Result.Session.SessionSettings.Get(FName("MatchType"), SettingsMatchTypeValue);
		if (SettingsMatchTypeValue == MatchType)
		{
			MultiplayerSessionsSubsystem->JoinSession(Result);
			return;
		}
	}
	if (!bWasSuccessful || SessionResults.Num() == 0)
	{
		Btn_Join->SetIsEnabled(true);
	}
}

void UMenu::OnJoinSession(EOnJoinSessionCompleteResult::Type Result)
{
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem)
	{
		IOnlineSessionPtr SessionInterface = Subsystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			FString Address;
			SessionInterface->GetResolvedConnectString(NAME_GameSession, Address);

			APlayerController* PlayerController = GetGameInstance()->GetFirstLocalPlayerController();
			if (PlayerController)
			{
				PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
			}
		}
	}

	if (Result != EOnJoinSessionCompleteResult::Success)
	{
		Btn_Join->SetIsEnabled(true);
	}
}

void UMenu::OnStartSession(bool bWasSuccessful)
{
}

void UMenu::OnDestroySession(bool bWasSuccessful)
{
}

bool UMenu::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	if (Btn_Host)
	{
		Btn_Host->OnClicked.AddDynamic(this, &ThisClass::Btn_HostClicked);
	}

	if (Btn_Join)
	{
		Btn_Join->OnClicked.AddDynamic(this, &ThisClass::Btn_JoinClicked);
	}

	return true;
}

void UMenu::NativeDestruct()
{
	MenuTearDown();
	Super::NativeDestruct();
}

// Custom delegate handle, works after session creation
void UMenu::OnCreateSession(bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				15.f,
				FColor::Green,
				FString(TEXT("Session created successfully")));
		}

		UWorld* World = GetWorld();
		if (World)
		{
			World->ServerTravel(PathToLobby);
		}
	}
	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				15.f,
				FColor::Red,
				FString(TEXT("Session creation failed")));
		}
		Btn_Host->SetIsEnabled(true);
	}
}

void UMenu::Btn_HostClicked()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			15.f,
			FColor::Yellow,
			FString(TEXT("Host Clicked"))
		);
	}

	Btn_Host->SetIsEnabled(false);

	if (MultiplayerSessionsSubsystem)
	{
		MultiplayerSessionsSubsystem->CreateSession(NumPublicConnections, MatchType);
	}
}

void UMenu::Btn_JoinClicked()
{
	Btn_Join->SetIsEnabled(false);
	if (MultiplayerSessionsSubsystem)
	{
		MultiplayerSessionsSubsystem->FindSessions(10000);
	}
}

void UMenu::MenuTearDown()
{
	RemoveFromParent();
	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (PlayerController)
		{
			FInputModeGameOnly InputModeData;
			PlayerController->SetInputMode(InputModeData);
			PlayerController->SetShowMouseCursor(false);
		}
	}
}
