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
	FText Name;

	UPROPERTY(Category=Menu, EditAnywhere)
	class UEditorBuildMenuDataAsset* Submenu;

	UPROPERTY(Category=Menu, EditAnywhere)
	FDataTableRowHandle BlocksTableRow;
	
};

UCLASS()
class SPINNINGWHEELS_API UEditorBuildMenuDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(Category=Menu, EditAnywhere)
	TArray<FEditorBuildMenuItem> Items;
};

USTRUCT()
struct FEditorBuildMenu
{
	GENERATED_BODY()

private:
	UPROPERTY(Category=Menu, EditAnywhere)
	UEditorBuildMenuDataAsset* MenuData;

public:

	TArray<FEditorBuildMenuItem> Items;
	
	bool bInitialized = false;

public:
	FEditorBuildMenu() = default;

	FEditorBuildMenu(UEditorBuildMenuDataAsset* InMenuData)
		: MenuData(InMenuData)
	{
		if (MenuData)
		{
			Items = MenuData->Items;
			bInitialized = true;
		}
	}
	
};
