#pragma once
#include "CoreMinimal.h"
#include "Containers/UnrealString.h"
#include <chrono>

DECLARE_LOG_CATEGORY_EXTERN(Timing_LOG, Log, All);

class SLERPTESTS_API FPerformanceTimer
{
	FString timerLabel;
	std::chrono::time_point<std::chrono::steady_clock> startTime;
	ELogVerbosity::Type level;
	TSharedPtr<FString> returnVal;
	const TArray<FString> times = {"nanoseconds", "microseconds", "milliseconds", "seconds", "minutes"};
	const TArray<int> timeDividers = {1000, 1000, 1000, 60};

public:
	FPerformanceTimer();

	FPerformanceTimer(const FString& label, ELogVerbosity::Type type);

	FPerformanceTimer(const FString& label, ELogVerbosity::Type type, TSharedPtr<FString>& retStmt);

	FPerformanceTimer(const FPerformanceTimer&) = default;
	FPerformanceTimer& operator=(FPerformanceTimer&& other) = default;
	FPerformanceTimer& operator=(const FPerformanceTimer& other)
	{
		timerLabel = other.timerLabel;
		startTime = other.startTime;
		level = other.level;
		returnVal = other.returnVal;
		return *this;
	}

	~FPerformanceTimer();
};