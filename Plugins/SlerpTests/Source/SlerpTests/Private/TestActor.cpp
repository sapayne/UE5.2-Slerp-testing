// Written by Samuel Payne

#include "TestActor.h"
#include "SlerpMath.h"
#include "PerformanceTimer.h"

// Sets default values
ATestActor::ATestActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ATestActor::BeginPlay()
{
	Super::BeginPlay();
	
}

FString ATestActor::oldSlerpEngineTiming()
{
	TSharedPtr<FString> ans(new FString(""));
	FQuat result = FQuat::Identity;

	{
		FPerformanceTimer slerp("Engine Slerp_NotNormalized", ELogVerbosity::Warning, ans);

		const int loop = numOfLoops;
		double alpha;

		for (int i = 0; i < loop; i++)
		{
			alpha = static_cast<double>(i) / static_cast<double>(loop);
			result += FQuat::Slerp_NotNormalized(CurrentQuat, TargetQuat, alpha);
		}
	}

	return (ans ? *ans.Get() : "") + result.ToString()[0];
}

FString ATestActor::oldSlerpTiming()
{
	TSharedPtr<FString> ans(new FString(""));
	FQuat result = FQuat::Identity;

	{
		FPerformanceTimer slerp("Plugin Slerp_NotNormalized", ELogVerbosity::Warning, ans);

		const int loop = numOfLoops;
		double alpha;

		for (int i = 0; i < loop; i++)
		{
			alpha = static_cast<double>(i) / static_cast<double>(loop);
			result += SlerpMath::Slerp_NotNormalized(CurrentQuat, TargetQuat, alpha);
		}
	}	

	return (ans ? *ans.Get() : "") + result.ToString()[0];
}

FString ATestActor::newSlerpTiming()
{
	TSharedPtr<FString> ans(new FString(""));
	FQuat result = FQuat::Identity;

	{
		FPerformanceTimer slerp("Fast Slerp", ELogVerbosity::Warning, ans);

		const int loop = numOfLoops;
		double alpha;

		for (int i = 0; i < loop; i++)
		{
			alpha = static_cast<double>(i) / static_cast<double>(loop);
			result += SlerpMath::FastSlerp(CurrentQuat, TargetQuat, alpha);
		}
	}

	return (ans ? *ans.Get() : "") + result.ToString()[0];
}

FString ATestActor::newSlerpTimingWAdd()
{
	TSharedPtr<FString> ans(new FString(""));
	FQuat result = FQuat::Identity;

	{
		FPerformanceTimer slerp("FastSlerpWAdd", ELogVerbosity::Warning, ans);

		const int loop = numOfLoops;
		double alpha;

		for (int i = 0; i < loop; i++)
		{
			alpha = static_cast<double>(i) / static_cast<double>(loop);
			result += SlerpMath::FastSlerpWAdd(CurrentQuat, TargetQuat, alpha);
		}
	}

	return (ans ? *ans.Get() : "") + result.ToString()[0];
}

FString ATestActor::newSlerpTimingSIMDAtt()
{
	TSharedPtr<FString> ans(new FString(""));
	FQuat result = FQuat::Identity;

	{
		FPerformanceTimer slerp("FastSlerpSIMDAtt", ELogVerbosity::Warning, ans);

		const int loop = numOfLoops;
		double alpha;

		for (int i = 0; i < loop; i++)
		{
			alpha = static_cast<double>(i) / static_cast<double>(loop);
			result += SlerpMath::FastSlerpSIMDAtt(CurrentQuat, TargetQuat, alpha);
		}
	}

	return (ans ? *ans.Get() : "") + result.ToString()[0];
}

