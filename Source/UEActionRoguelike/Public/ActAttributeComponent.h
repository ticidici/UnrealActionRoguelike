// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "ActAttributeComponent.generated.h"

//class keyword is an inline forward declaration
DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FOnHealthChanged, AActor*, InstigatorActor, class UActAttributeComponent*, OwningComp, float, NewHealth, float, Delta, float, ActualDelta, FGameplayTagContainer, HealthVariationTags);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnRageChanged, class UActAttributeComponent*, OwningComp, float, NewRage, float, Delta, float, ActualDelta);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UEACTIONROGUELIKE_API UActAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	static UActAttributeComponent* GetAttributes(AActor* FromActor);

	UFUNCTION(BlueprintCallable, Category = "Attributes", meta = (DisplayName))
	static bool IsActorAlive(AActor* Actor);
	
	UActAttributeComponent();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Attributes")
	float MaxHealth;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Attributes")
	float Health;
	UPROPERTY(EditDefaultsOnly, meta = (UIMin = 1, UIMax = 100))
	float PercentageConsideredLowHealth;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Attributes")
	float MaxRage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Attributes")
	float Rage;
	
	UFUNCTION(NetMulticast, Reliable) // @FIXME: mark as unreliable once we moved the 'state' out of actcharacter
	void MulticastHealthChanged(AActor* InstigatorActor, float NewHealth, float Delta, float ActualDelta, FGameplayTagContainer HealthVariationTags);
	
public:

	UFUNCTION(BlueprintCallable)
	bool Kill(AActor* InstigatorActor);
	
	UFUNCTION(BlueprintCallable)
	bool IsAlive() const;
	bool isFullHealth();
	bool isLowHealth();
	FORCEINLINE float GetHealthMax() { return MaxHealth; }
	
	FORCEINLINE bool isFullRage() {	return Rage >= MaxRage; }
	FORCEINLINE float GetRageMax() { return MaxRage; }
	bool TrySpendRage(float Quantity); 


	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;
	
	UPROPERTY(BlueprintAssignable)
	FOnRageChanged OnRageChanged;
	
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool ApplyHealthChange(AActor* InstigatorActor, float Delta, FGameplayTagContainer HealthVariationTags);
	
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool ApplyRageChange(float Delta);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
