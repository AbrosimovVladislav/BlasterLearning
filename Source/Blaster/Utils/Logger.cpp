// Fill out your copyright notice in the Description page of Project Settings.


#include "Logger.h"

#include "Engine/Engine.h"

Logger::Logger()
{
}

Logger::~Logger()
{
}

void Logger::PrintOnScreen(FString Prefix, FString Value)
{
	GEngine->AddOnScreenDebugMessage(
	-1,
	5.f,
	FColor::Cyan,
	FString::Printf(TEXT("%s: %s"), *Prefix, *Value));
}
