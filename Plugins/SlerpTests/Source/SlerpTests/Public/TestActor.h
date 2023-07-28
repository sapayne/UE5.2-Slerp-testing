// Written by Samuel Payne

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TestActor.generated.h"

UCLASS()
class SLERPTESTS_API ATestActor : public ACharacter
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATestActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SlerpTiming", meta = (UIMin = 0, UIMax = 20000, SliderExpononent = 1))
	int numOfLoops;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SlerpTiming")
	FQuat CurrentQuat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SlerpTiming")
	FQuat TargetQuat;

protected:	
	UFUNCTION(BlueprintCallable, Category = "SlerpTiming")
	FString oldSlerpEngineTiming();

	UFUNCTION(BlueprintCallable, Category = "SlerpTiming")
	FString oldSlerpTiming();

	UFUNCTION(BlueprintCallable, Category = "SlerpTiming")
	FString newSlerpTiming();

	UFUNCTION(BlueprintCallable, Category = "SlerpTiming")
	FString newSlerpTimingWAdd();

	UFUNCTION(BlueprintCallable, Category = "SlerpTiming")
	FString newSlerpTimingSIMDAtt();
};
