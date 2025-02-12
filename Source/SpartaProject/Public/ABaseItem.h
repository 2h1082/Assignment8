// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemInterface.h"
#include "ABaseItem.generated.h"

//�浹 ������Ʈ ���� ����
class USphereComponent; 

UCLASS()
class SPARTAPROJECT_API AABaseItem : public AActor, public IItemInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AABaseItem();

protected:
	//������ Ÿ�� ���� �����ϰ� ����
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Item")
	FName ItemType;
	//��Ʈ ������Ʈ(��)
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Item|Component")
	USceneComponent* Scene;
	//�浹 ������Ʈ (�÷��̾� ���� ����)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Component")
	USphereComponent* Collision;
	//������ �ð� ǥ���� ����ƽ �޽�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Component")
	UStaticMeshComponent* StaticMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Effects")
	UParticleSystem* PickupParticle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Effects")
	USoundBase* PickupSound;

	//�������̽����� �䱸�ϴ� �Լ���
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

	//������ ���� �Լ�
	virtual void DestroyItem();
};
