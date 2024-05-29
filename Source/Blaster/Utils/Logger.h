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

	void PrintOnScreen(FString Prefix, FString Value);	
};
