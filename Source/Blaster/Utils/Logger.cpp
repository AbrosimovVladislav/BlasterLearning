// Fill out your copyright notice in the Description page of Project Settings.


#include "Logger.h"

#include "Engine/Engine.h"

Logger::Logger()
{
}

Logger::~Logger()
{
}

void Logger::PrintOnScreenCyan(FString Prefix, FString Value)
{
	GEngine->AddOnScreenDebugMessage(
		-1,
		5.f,
		FColor::Cyan,
		FString::Printf(TEXT("%s: %s"), *Prefix, *Value));
}

void Logger::PrintOnScreenBlue(FString Prefix, FString Value)
{
	GEngine->AddOnScreenDebugMessage(
		-1,
		5.f,
		FColor::Blue,
		FString::Printf(TEXT("%s: %s"), *Prefix, *Value));
}

void Logger::PrintOnScreenRed(FString Prefix, FString Value)
{
	GEngine->AddOnScreenDebugMessage(
		-1,
		5.f,
		FColor::Red,
		FString::Printf(TEXT("%s: %s"), *Prefix, *Value));
}

void Logger::PrintOnScreenGreen(FString Prefix, FString Value)
{
	GEngine->AddOnScreenDebugMessage(
		-1,
		5.f,
		FColor::Green,
		FString::Printf(TEXT("%s: %s"), *Prefix, *Value));
}

void Logger::PrintOnScreenBlack(FString Prefix, FString Value)
{
	GEngine->AddOnScreenDebugMessage(
		-1,
		5.f,
		FColor::Black,
		FString::Printf(TEXT("%s: %s"), *Prefix, *Value));
}
