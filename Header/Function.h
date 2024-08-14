#pragma once

template <typename T>
void SafeDelete(T& ptr)
{
	if (ptr)
	{
		delete ptr;
		ptr = nullptr;
	}
}

template <typename T2, typename T1>
void SafeDeleteObserver(T1& ptr)
{
	T2::GetInstance()->UnSubscribe(ptr);
	SafeDelete(ptr);
}