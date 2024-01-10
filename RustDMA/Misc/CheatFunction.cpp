#include "Pch.h"
#include "CheatFunction.h"

CheatFunction::CheatFunction(int time, std::function<void()> func)
{
	MsSleep = time;
	Function = func;
}

void CheatFunction::Execute()
{

	if (GetTickCount64() - LastExecution > MsSleep)
	{
		Function();
		LastExecution = GetTickCount64();
	}
}