// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ABaseItem.h"
#include "SlowingItem.generated.h"

/**
 * 
 */
UCLASS()
class SPARTAPROJECT_API ASlowingItem : public AABaseItem
{
	GENERATED_BODY()
	
public:
	ASlowingItem();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	float SlowingAmount;

	virtual void ActivateItem(AActor* Activator) override;
};
