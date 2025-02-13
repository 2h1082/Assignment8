// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SpartaCharacter.generated.h"

class USpringArmComponent; //������ �� ���� Ŭ���� ���
class UCameraComponent; //ī�޶� ���� Ŭ���� ���� ����
class UWidgetComponent;
//Enhanced Input���� �׼� �� ���� �� ����� ����ü
struct FInputActionValue;

UCLASS()
class SPARTAPROJECT_API ASpartaCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASpartaCharacter();

	//��������
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArmComp;
	//ī�޶�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComp;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	UWidgetComponent* OverheadWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> WBP_HPClass;

	//���� ü�� �������� �Լ�
	UFUNCTION(BlueprintPure, Category = "Health")
	float GetHealth() const;
	//�ִ� ü�� �������� �Լ�
	UFUNCTION(BlueprintPure, Category = "Health")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Health")
	void AddHealth(float Amount);
	//�ӵ� ����

	UFUNCTION(BlueprintCallable, Category = "Debuff")
	void SlowSpeed(float SlowAmount);
	UFUNCTION(BlueprintCallable, Category = "Debuff")
	void EndSlow();

	//���� ����
	UFUNCTION(BlueprintCallable, Category = "Debuff")
	void ReverseControl();
	UFUNCTION(BlueprintCallable, Category = "Debuff")
	void EndReverseControl();

	UFUNCTION(BlueprintPure, Category = "Debuff")
	bool bIsSlow() const;
	UFUNCTION(BlueprintPure, Category = "Debuff")
	bool bIsInversion() const;

	//���� Ÿ�̸� �ڵ鷯
	FTimerHandle SlowTimerHandle;
	FTimerHandle InversionTimerHandle;

protected:

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Movement")
	float NormalSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float SprintSpeedMultiplier;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float SprintSpeed;
	//���ο� ���� ǥ��
	bool bHasSlowDebuff;
	//���� ���� ���� ǥ��
	bool bHasControlInversionDebuff;

	//�ִ� ü��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHealth;
	//���� ü��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float Health;
	//��� ó�� �Լ�
	UFUNCTION(BlueprintCallable, Category = "Health")
	virtual void OnDeath();
	void UpdateOverheadHP();

	virtual void BeginPlay() override;
	//������ ó�� �Լ�
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UFUNCTION()
	void Move(const FInputActionValue& value);
	UFUNCTION()
	void StartJump(const FInputActionValue& value);
	UFUNCTION()
	void StopJump(const FInputActionValue& value);
	UFUNCTION()
	void Look(const FInputActionValue& value);
	UFUNCTION()
	void StartSprint(const FInputActionValue& value);
	UFUNCTION()
	void StopSprint(const FInputActionValue& value);
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
