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
	//���� ȹ�� �� ���� ����(�ڽ� Ŭ�������� ��ӹ޾� �� ����)
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Item")
	int32 PointValue;

	//�θ� Ŭ�������� ��ӹ��� ActivateItem�Լ� �������̵�
	virtual void ActivateItem(AActor* Activator) override;
};
