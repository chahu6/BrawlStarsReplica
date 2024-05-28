#pragma once

#include <functional>

class FDoOnce
{
	using DonOnceFunction = std::function<void()>;
public:
	inline FDoOnce(bool bStartClosed = true) : bDoOnce(bStartClosed) {};

	inline void Reset() { bDoOnce = true; }

	inline bool Execute(DonOnceFunction func)
	{
		if (bDoOnce)
		{
			bDoOnce = false;
			func();
			return true;
		}
		return false;
	}

private:
	bool bDoOnce;
};