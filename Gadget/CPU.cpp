#pragma once
//A brief abstract for general CPU
#include<iostream>
class CPU {
private:
	int cacheSize;
	int clockSpeed;
	int issNum;
	struct Instruction;
	struct Operation;
	struct EU {
		static bool issue();
	};
	struct ICU {
		static void Ins2Ops();
		static void flush();
	};
public:
	CPU(int X_GHZ,int IssueNum,int cacheSize):clockSpeed(cacheSize),issNum(IssueNum),cacheSize(cacheSize) {};
	void Run() {
		bool isError = EU::issue(ICU::Ins2Ops());
		if (!isError) {
			ICU::flush();
		}
	}
};