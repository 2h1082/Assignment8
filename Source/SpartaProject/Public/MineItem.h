// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ABaseItem.h"
#include "MineItem.generated.h"

/**
 * 
 */
UCLASS()
class SPARTAPROJECT_API AMineItem : public AABaseItem
{
	GENERATED_BODY()
	
public:
	AMineItem();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Component")
	USphereComponent* ExplosionCollision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Effects")
	UParticleSystem* ExplosionParticle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Effects")
	USoundBase* ExplosionSound;

	//���߱��� �ɸ��� �ð�
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Mine")
	float ExplosionDelay;
	//���� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mine")
	float ExplosionRadius;
	//���� ������
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mine")
	float ExplosionDamage;

	bool bHasExploded;
	FTimerHandle ExplosionTimerHandle;
	FTimerHandle DestroyParticleTimerHandle;

	virtual void ActivateItem(AActor* Activator) override;

	void Explosion();

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
