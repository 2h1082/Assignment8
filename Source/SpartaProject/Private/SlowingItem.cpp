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
	//플레이어 태그 확인
	if (Activator && Activator->ActorHasTag("Player"))
	{
		if (ASpartaCharacter* PlayerCharacter = Cast<ASpartaCharacter>(Activator))
		{
			PlayerCharacter->SlowSpeed(SlowingAmount);
		}

		//부모 클래스에 정의된 아이템 파괴 함수 호출
		DestroyItem();
	}
}
