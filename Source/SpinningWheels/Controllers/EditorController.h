// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RaceController.h"
#include "EditorController.generated.h"

class UInputMappingContext;
class UEditorInputConfig;
class AEditorPawn;

UCLASS()
class SPINNINGWHEELS_API AEditorController : public ARaceController
{
	GENERATED_BODY()

private:

	TWeakObjectPtr<AEditorPawn> EditorPawn;

	void MoveCamera(const FInputActionValue& Value);
	void RotateCamera(const FInputActionValue& Value);

protected:

	UPROPERTY(Category=Input, EditDefaultsOnly, meta=(AllowPrivateAccess="true"))
	TObjectPtr<UInputMappingContext> EditorMappingContext;

	UPROPERTY(Category=Input, EditDefaultsOnly, meta=(AllowPrivateAccess="true"))
	TObjectPtr<UEditorInputConfig> EditorInputConfig;

	//~ Begin AController Interface
	virtual void SetPawn(APawn* InPawn) override;
	//~ End AController Interface

	virtual void SetupInputBindings() override;
	void SetupEditorInputBindings();

public:
	
};
