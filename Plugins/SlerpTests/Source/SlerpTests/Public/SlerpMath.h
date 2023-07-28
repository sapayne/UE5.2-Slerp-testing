// Written by Samuel Payne

#pragma once

#include "CoreMinimal.h"

class SlerpMath
{
public:
	template <typename T>
	using Quat = UE::Math::TQuat<T>;

	//	original code written by Epic
	template<typename T>
	static Quat<T> Slerp_NotNormalized(const Quat<T>& Quat1, const Quat<T>& Quat2, T Slerp)
	{
		// Get cosine of angle between quats.
		const T RawCosom =
			Quat1.X * Quat2.X +
			Quat1.Y * Quat2.Y +
			Quat1.Z * Quat2.Z +
			Quat1.W * Quat2.W;
		// Unaligned quats - compensate, results in taking shorter route.
		const T Cosom = FMath::FloatSelect(RawCosom, RawCosom, -RawCosom);

		T Scale0, Scale1;

		if (Cosom < T(0.9999f))
		{
			const T Omega = FMath::Acos(Cosom);
			const T InvSin = T(1.f) / FMath::Sin(Omega);
			Scale0 = FMath::Sin((T(1.f) - Slerp) * Omega) * InvSin;
			Scale1 = FMath::Sin(Slerp * Omega) * InvSin;
		}
		else
		{
			// Use linear interpolation.
			Scale0 = T(1.0f) - Slerp;
			Scale1 = Slerp;
		}

		// In keeping with our flipped Cosom:
		Scale1 = FMath::FloatSelect(RawCosom, Scale1, -Scale1);

		Quat<T> Result;

		Result.X = Scale0 * Quat1.X + Scale1 * Quat2.X;
		Result.Y = Scale0 * Quat1.Y + Scale1 * Quat2.Y;
		Result.Z = Scale0 * Quat1.Z + Scale1 * Quat2.Z;
		Result.W = Scale0 * Quat1.W + Scale1 * Quat2.W;

		return Result;
	}

	//	use double as the base values to be casted as FQuat's components are now doubles instead of floats
	template <typename T>
	static Quat<T> FastSlerp(const Quat<T>& Quat1, const Quat<T>& Quat2, T alpha)
	{
		// Get cosine of angle between quats.
		T RawCosom =
			Quat1.X * Quat2.X +
			Quat1.Y * Quat2.Y +
			Quat1.Z * Quat2.Z +
			Quat1.W * Quat2.W;

		// Unaligned quats - compensate, results in taking shorter route.
		const T sign = (RawCosom >= (T)0.0) ? (T)1.0 : (T)-1.0;
		RawCosom *= sign;

		/*	this allows the else statement in the original function to be removed
		*	and the Scales inside the if block to be written in terms of themselves
		*/
		T Scale0 = (T)1.0 - alpha, Scale1 = alpha * sign;

		//	changed to 1.0 instead of 0.9999
		if (RawCosom < (T)1.0)
		{
			const T Omega = FMath::Acos(RawCosom);
			//	cosecant is equal to sin(arccos x) = sqrt(1 - x * x)
			const T InvSin = (T)1.0 / FMath::Sin(Omega);
			//	const T InvSin = 1.0 / sqrt(1.0 - pow(RawCosom, 2));

			Scale0 = FMath::Sin(Scale0 * Omega) * InvSin;
			Scale1 = FMath::Sin(Scale1 * Omega) * InvSin;
			/*	
			*	unfortunately, no identity for sin(scale_ * arccos(RawCosom))
			*	if there was then the scales could possibly be written as:
			*
			*	identity / sqrt(1.0 - pow(RawCosom, 2));
			*
			*	as a way of seeing if the equation reduces further into a simpler expression
			* 
			*	maybe someone with a premium wolfram alpha account could plug in the equation
			*	and see if the calculations spit out a simpler formula as a free account hits 
			*	the computation time limit.
			*/
		}

		//	Calculation done while constructing
		return Quat<T>(Scale0 * Quat1.X + Scale1 * Quat2.X,
			Scale0 * Quat1.Y + Scale1 * Quat2.Y,
			Scale0 * Quat1.Z + Scale1 * Quat2.Z,
			Scale0 * Quat1.W + Scale1 * Quat2.W);
	}

	//	new with additions
	template <typename T>
	static Quat<T> FastSlerpWAdd(const Quat<T>& Quat1, const Quat<T>& Quat2, T alpha)
	{
		/*	compiler should probably have already pulled the 
		*	duplicate cast into a variable but just incase.
		*/
		const T one = (T)1.0;

		T RawCosom =
			Quat1.X * Quat2.X +
			Quat1.Y * Quat2.Y +
			Quat1.Z * Quat2.Z +
			Quat1.W * Quat2.W;

		/*	after peeking the geometry tools slerp function(Boost library), 
		*	I realized the RawCosom mutliplication only matters when it's 
		*	negative so like the reorganizing of the RawCosom < 0.9999.
		*	We can do it here as well.
		*/ 
		T sign = one;

		if (RawCosom < (T)0.0)
		{
			sign = -one;	//	equivalent to sign = -sign;
			RawCosom *= -one;	//	equivalent to RawCosom *= sign;
		} 

		T Scale0 = one - alpha, Scale1 = alpha * sign;

		if (RawCosom < one)
		{
			const T Omega = FMath::Acos(RawCosom);
			const T InvSin = one / FMath::Sin(Omega);
			Scale0 = FMath::Sin(Scale0 * Omega) * InvSin;
			Scale1 = FMath::Sin(Scale1 * Omega) * InvSin;
		}	

		return Quat<T>(Scale0 * Quat1.X + Scale1 * Quat2.X,
			Scale0 * Quat1.Y + Scale1 * Quat2.Y,
			Scale0 * Quat1.Z + Scale1 * Quat2.Z,
			Scale0 * Quat1.W + Scale1 * Quat2.W);
	}

	/*	SIMD attempt, probably did this wrong
	* 
	*	Update before committing to repo: 
	*	I mistakingly used floats instead of T for x, y, z, w, but
	*	this is the function used in the videos so I won't change it.
	*/	
	template <typename T>
	static Quat<T> FastSlerpSIMDAtt(const Quat<T>& Quat1, const Quat<T>& Quat2, T alpha)
	{
		const T one = (T)1.0;

		T RawCosom =
			Quat1.X * Quat2.X +
			Quat1.Y * Quat2.Y +
			Quat1.Z * Quat2.Z +
			Quat1.W * Quat2.W;

		T sign = one;

		if (RawCosom < (T)0.0)
		{
			sign = -one;
			RawCosom *= -one;
		}

		T Scale0 = one - alpha, Scale1 = alpha * sign;

		if (RawCosom < one)
		{
			const T Omega = FMath::Acos(RawCosom);
			const T InvSin = one / FMath::Sin(Omega);
			Scale0 = FMath::Sin(Scale0 * Omega) * InvSin;
			Scale1 = FMath::Sin(Scale1 * Omega) * InvSin;
		}

		/*	I believe the 4 line multiplication for each component
		*	of the quaternion was for SIMD so I'll try testing
		*	4 float variables for each component then pass them
		*	in as the constructor parameters in an attempt to
		*	allow the compiler to optimize the initialization
		*	of the quaternion instead of possibly setting the
		*	components to 0 or another value then updating.
		*/
		const float x = Scale0 * Quat1.X + Scale1 * Quat2.X;
		const float y = Scale0 * Quat1.Y + Scale1 * Quat2.Y;
		const float z = Scale0 * Quat1.Z + Scale1 * Quat2.Z;
		const float w = Scale0 * Quat1.W + Scale1 * Quat2.W;

		return Quat<T>(x, y, z, w);
	}
};