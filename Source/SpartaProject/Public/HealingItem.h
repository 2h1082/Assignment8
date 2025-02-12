// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ABaseItem.h"
#include "HealingItem.generated.h"

/**
 * 
 */
UCLASS()
class SPARTAPROJECT_API AHealingItem : public AABaseItem
{
	GENERATED_BODY()
	
public:
	AHealingItem();

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Item")
	float HealAmount;

	virtual void ActivateItem(AActor* Activator) override;
};
