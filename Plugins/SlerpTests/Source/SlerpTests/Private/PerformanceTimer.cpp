#include "PerformanceTimer.h"
#include "Logging/LogVerbosity.h"

DEFINE_LOG_CATEGORY(Timing_LOG);

FPerformanceTimer::FPerformanceTimer()
{
	timerLabel = "";
	level = ELogVerbosity::Warning;
	returnVal = nullptr;
	startTime = std::chrono::high_resolution_clock::now();
}

FPerformanceTimer::FPerformanceTimer(const FString& label, ELogVerbosity::Type type)
{
	timerLabel = label.Len() > 0 ? label + " " : "";
	level = type;
	returnVal = nullptr;
	startTime = std::chrono::high_resolution_clock::now();
}

FPerformanceTimer::FPerformanceTimer(const FString& label, ELogVerbosity::Type type, TSharedPtr<FString>& retStmt)
{
	timerLabel = label.Len() > 0 ? label + " " : "";
	level = type;
	returnVal = retStmt;
	startTime = std::chrono::high_resolution_clock::now();
}

FPerformanceTimer::~FPerformanceTimer()
{
	auto EndTime = std::chrono::high_resolution_clock::now();
	const long long time = std::chrono::duration_cast<std::chrono::nanoseconds>(EndTime - startTime).count();
	long long temp = time;
	uint8 timeIndex = 0;
	double divisor = 1;

	while (temp > timeDividers[timeIndex] && timeIndex < timeDividers.Num())
	{
		temp /= timeDividers[timeIndex];
		divisor *= timeDividers[timeIndex];
		timeIndex++;
	}

	const FString output = FString::Printf(TEXT("%sTimer took: %s %s "), *timerLabel, *FString::SanitizeFloat(static_cast<double>(time) / divisor, 2), *times[timeIndex]);

	if (returnVal != nullptr)
	{
		returnVal->Append(output);
		return;
	}

	switch (level)
	{
		case ELogVerbosity::Warning:
			UE_LOG(Timing_LOG, Warning, TEXT("%s"), *output);
			break;
		case ELogVerbosity::Error:
			UE_LOG(Timing_LOG, Error, TEXT("%s"), *output);
			break;
		default:
			UE_LOG(Timing_LOG, Log, TEXT("%s"), *output);
			break;
	}
	
}
