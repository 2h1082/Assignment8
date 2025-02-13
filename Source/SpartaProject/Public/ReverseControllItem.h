// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ABaseItem.h"
#include "ReverseControllItem.generated.h"

/**
 * 
 */
UCLASS()
class SPARTAPROJECT_API AReverseControllItem : public AABaseItem
{
	GENERATED_BODY()
	
public:
	AReverseControllItem();

	virtual void ActivateItem(AActor* Activator) override;
};
