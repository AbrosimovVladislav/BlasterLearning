// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class BLASTER_API Logger
{
public:
	Logger();
	~Logger();

	void PrintOnScreenCyan(FString Prefix, FString Value);	
	void PrintOnScreenBlue(FString Prefix, FString Value);	
	void PrintOnScreenRed(FString Prefix, FString Value);	
	void PrintOnScreenGreen(FString Prefix, FString Value);	
	void PrintOnScreenBlack(FString Prefix, FString Value);	
};
