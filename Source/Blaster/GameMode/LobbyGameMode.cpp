// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"

#include "Engine/Engine.h"
#include "Engine/World.h"
#include "GameFramework/GameStateBase.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	int32 NumberOfPlayers = GameState.Get()->PlayerArray.Num();
	if (NumberOfPlayers == 2)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			bUseSeamlessTravel = true;
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Purple, FString(TEXT("Before Travel")));
			World->ServerTravel(FString("/Game/Maps/BlasterMap?listen"));
		}
	}
}
