// Fill out your copyright notice in the Description page of Project Settings.


#include "ReverseControllItem.h"
#include "SpartaCharacter.h"


AReverseControllItem::AReverseControllItem()
{
	ItemType = "ReverseControll";
}

void AReverseControllItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);
	//�÷��̾� �±� Ȯ��
	if (Activator && Activator->ActorHasTag("Player"))
	{
		if (ASpartaCharacter* PlayerCharacter = Cast<ASpartaCharacter>(Activator))
		{
			PlayerCharacter->ReverseControl();
		}

		//�θ� Ŭ������ ���ǵ� ������ �ı� �Լ� ȣ��
		DestroyItem();
	}
}
