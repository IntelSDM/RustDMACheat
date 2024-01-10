#pragma once
class CheatFunction
{
	int MsSleep = 0;
	std::function<void()> Function;
	int LastExecution;
public:
	CheatFunction(int time, std::function<void()>func);
	void Execute();
};