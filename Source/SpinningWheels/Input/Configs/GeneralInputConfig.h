#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GeneralInputConfig.generated.h"

class UInputAction;

UCLASS()
class SPINNINGWHEELS_API UGeneralInputConfig : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, Category = "Input") UInputAction* IA_OpenMenu;
	
};