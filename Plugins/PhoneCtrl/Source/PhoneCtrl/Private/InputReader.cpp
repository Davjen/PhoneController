// Fill out your copyright notice in the Description page of Project Settings.


#include "InputReader.h"

#define HANDSHAKING (1)
#define PING (2)
#define DISCONNECT (3)
#define MOVEMENT (4)
#define BUTTONS (5)

FInputReader::FInputReader()
{
}

FInputReader::~FInputReader()
{
}

void FInputReader::PackageUnpacker(float* Data)
{
	if (Data[0] == MOVEMENT)
	{
		UE_LOG(LogTemp, Warning, TEXT("FUNZIONA"));
	}
}
