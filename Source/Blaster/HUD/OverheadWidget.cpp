// Fill out your copyright notice in the Description page of Project Settings.


#include "OverheadWidget.h"

#include "Components/TextBlock.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerState.h"

void UOverheadWidget::SetDisplayText(FString TextToDisplay)
{
	if (DisplayText)
	{
		DisplayText->SetText(FText::FromString(TextToDisplay));
	}
}

void UOverheadWidget::ShowPlayerNetRole(APawn* InPawn)
{
	ENetRole LocalRole = InPawn->GetLocalRole();
	FString Name = InPawn->GetPlayerState()->GetPlayerName();
	FString Role;
	switch (LocalRole)
	{
	case ENetRole::ROLE_Authority:
		Role = FString(":Authority");
		break;
	case ENetRole::ROLE_AutonomousProxy:
		Role = FString(":AutonomousProxy");
		break;
	case ENetRole::ROLE_SimulatedProxy:
		Role = FString(":SimulatedProxy");
		break;
	case ENetRole::ROLE_None:
		Role = FString(":None");
		break;
	}

	SetDisplayText(Name + Role);
}

void UOverheadWidget::NativeDestruct()
{
	RemoveFromParent();
	Super::NativeDestruct();
}
