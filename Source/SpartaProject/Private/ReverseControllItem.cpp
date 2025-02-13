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
	//플레이어 태그 확인
	if (Activator && Activator->ActorHasTag("Player"))
	{
		if (ASpartaCharacter* PlayerCharacter = Cast<ASpartaCharacter>(Activator))
		{
			PlayerCharacter->ReverseControl();
		}

		//부모 클래스에 정의된 아이템 파괴 함수 호출
		DestroyItem();
	}
}
