// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EditorBuildMenu.generated.h"

USTRUCT()
struct FEditorBuildMenuItem
{
	GENERATED_BODY()
	
	UPROPERTY(Category=Menu, EditAnywhere)
	class UEditorBuildMenu* Submenu;

	UPROPERTY(Category=Menu, EditAnywhere)
	TSubclassOf<class ABlock> Block;
	
};

// UCLASS()
// class SPINNINGWHEELS_API UEditorBuildMenuItem : public UObject
// {
// 	GENERATED_BODY()
//
// 	UPROPERTY(Category=Menu, EditAnywhere)
// 	FText Name;
// 	
// };

// UCLASS()
// class SPINNINGWHEELS_API UEditorBuildMenuItem_Submenu : public UObject
// {
// 	GENERATED_BODY()
//
// 	UPROPERTY(Category=Menu, EditAnywhere)
// 	TObjectPtr<class UEditorBuildMenu> Submenu;
// };
//
// UCLASS()
// class SPINNINGWHEELS_API UEditorBuildMenuItem_Block : public UObject
// {
// 	GENERATED_BODY()
//
// 	UPROPERTY(Category=Menu, EditAnywhere)
// 	TSubclassOf<class ABlock> Block;
// };

UCLASS()
class SPINNINGWHEELS_API UEditorBuildMenu : public UDataAsset
{
	GENERATED_BODY()

	UPROPERTY(Category=Menu, EditAnywhere)
	FText Name;

	UPROPERTY(Category=Menu, EditAnywhere)
	TArray<FEditorBuildMenuItem> Items;
	
};
