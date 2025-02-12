// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemInterface.h"
#include "ABaseItem.generated.h"

//충돌 컴포넌트 전방 선언
class USphereComponent; 

UCLASS()
class SPARTAPROJECT_API AABaseItem : public AActor, public IItemInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AABaseItem();

protected:
	//아이템 타입 편집 가능하게 지정
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Item")
	FName ItemType;
	//루트 컴포넌트(씬)
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Item|Component")
	USceneComponent* Scene;
	//충돌 컴포넌트 (플레이어 진입 감지)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Component")
	USphereComponent* Collision;
	//아이템 시각 표현용 스태틱 메시
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Component")
	UStaticMeshComponent* StaticMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Effects")
	UParticleSystem* PickupParticle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Effects")
	USoundBase* PickupSound;

	//인터페이스에서 요구하는 함수들
	virtual void OnItemOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult) override;
	virtual void OnItemEndOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex) override;
	virtual void ActivateItem(AActor* Activator) override;
	virtual FName GetItemType() const override;

	//아이템 제거 함수
	virtual void DestroyItem();
};
