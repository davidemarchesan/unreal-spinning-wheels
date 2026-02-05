// Fill out your copyright notice in the Description page of Project Settings.


#include "MainController.h"

void AMainController::BeginPlay()
{
	Super::BeginPlay();

	SetShowMouseCursor(true);
	SetInputMode(FInputModeUIOnly());
	
}
