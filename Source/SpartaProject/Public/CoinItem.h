// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ABaseItem.h"
#include "CoinItem.generated.h"

/**
 * 
 */
UCLASS()
class SPARTAPROJECT_API ACoinItem : public AABaseItem
{
	GENERATED_BODY()
	
public:
	ACoinItem();

protected:
	//코인 획득 시 얻을 점수(자식 클래스에서 상속받아 값 변경)
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Item")
	int32 PointValue;

	//부모 클래스에서 상속받은 ActivateItem함수 오버라이드
	virtual void ActivateItem(AActor* Activator) override;
};
