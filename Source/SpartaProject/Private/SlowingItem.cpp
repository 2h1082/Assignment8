// Fill out your copyright notice in the Description page of Project Settings.


#include "SlowingItem.h"
#include "SpartaCharacter.h"

ASlowingItem::ASlowingItem()
{
	SlowingAmount = 50.0f;
	ItemType = "Slowing";
}

void ASlowingItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);
	//�÷��̾� �±� Ȯ��
	if (Activator && Activator->ActorHasTag("Player"))
	{
		if (ASpartaCharacter* PlayerCharacter = Cast<ASpartaCharacter>(Activator))
		{
			PlayerCharacter->SlowSpeed(SlowingAmount);
		}

		//�θ� Ŭ������ ���ǵ� ������ �ı� �Լ� ȣ��
		DestroyItem();
	}
}
