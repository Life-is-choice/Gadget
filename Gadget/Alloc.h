#pragma once
//
//  Alloc.h
//  HomeWork
//
//  Created by 刘泽滨 on 2018/12/5.
//  Copyright © 2018年 刘泽滨. All rights reserved.
//

#ifndef Alloc_h
#define Alloc_h
/*
 （1）用C语言分别实现采用首次适应算法和最佳适应算法的动态分区分配过程alloc( )和回收过程free( )。其中，空闲分区通过空闲分区链来管理：在进行内存分配时，系统优先使用空闲区低端的空间。
 （2）假设初始状态下，可用的内存空间为640KB，并有下列的请求序列：
 •作业1申请130KB。
 •作业2申请60KB。
 •作业3申请100KB。
 •作业2释放60KB。
 •作业4申请200KB。
 •作业3释放100KB。
 •作业1释放130KB。
 •作业5申请140KB。
 •作业6申请60KB。
 •作业7申请50KB。
 •作业6释放60KB。
 请分别采用首次适应算法和最佳适应算法，对内存块进行分配和回收，要求每次分配和回收后显示出空闲分区链的情况。

 */

#include <iostream>
#include <set>
#include <algorithm>
#include <vector>
struct FirstAvailable {};
struct BestAdapt {};
#define KB 1024
#define MB 1048576
static char* memStart;
struct Block {
	char* start;
	char* end;
	inline size_t size()const {
		return end - start;
	}
	Block(char* s, char* e) :start(s), end(e) {}
};

class BlockCmp {
public:
	bool operator()(const Block& l, const Block& r) {
		return l.size() < r.size();
	}
};

std::set<Block, BlockCmp> blockIndex;
std::vector<Block> blockArray;

void init(char* p) {
	blockIndex.insert(Block{ p,p + 640 * KB });
	blockArray.push_back(Block{ p,p + 640 * KB });
	std::cout << "首地址:0" << std::endl;
}

void printAddr(void* addr, int num, size_t sz) {
	std::cout << "Assign" << num << "分配地址:[" << std::hex << (char*)addr - memStart << "," << std::hex << (char*)addr - memStart + sz << "]" << std::endl;
}
//alloc
template<class T>
void* Malloc(size_t byteNum) {}

template<>
void* Malloc<FirstAvailable>(size_t byteNum) {
	auto finded = std::find_if(blockArray.begin(), blockArray.end(), [=](Block& b) {
		return b.size() >= byteNum;
	});
	if (finded != blockArray.end()) {
		char* res = finded->start;
		finded->start += byteNum + 1;
		if (finded->size() == 0) {
			blockArray.erase(finded);
		}
		return res;
	}
	else {
		std::cout << "Alloc Fail\n";
		return nullptr;
	}
}
template<>
void* Malloc<BestAdapt>(size_t byteNum) {
	auto itr = blockIndex.lower_bound(Block{ (char*)0, (char*)byteNum });
	auto res = itr->start;
	if (itr != blockIndex.end()) {
		Block tmp = *itr;
		tmp.start += byteNum + 1;
		blockIndex.erase(itr);
		blockIndex.insert(tmp);
		return res;
	}
	else {
		std::cout << "Alloc Fail\n";
		return nullptr;
	}
}
//free
template<typename T>
bool Mfree(void* p, size_t byteNum) {}

template<>
bool Mfree<FirstAvailable>(void* p, size_t byteNum) {
	char* tmp = (char*)p;
	blockArray.push_back(Block{ tmp,tmp + byteNum });
	return true;
}
template<>
bool Mfree<BestAdapt>(void* p, size_t byteNum) {
	char* tmp = (char*)p;
	blockIndex.insert(Block{ tmp,tmp + byteNum });
	return true;
}

void printArray() {
	std::cout << "所有可用的内存块\n";
	int counter = 0;
	for (const auto& e : blockArray) {
		counter++;
		std::cout << "第" << counter << "块:[" << std::hex << e.start - memStart << "," << std::hex << e.end - memStart << "]" << std::endl;
	}
	std::cout << std::endl;
}

void printIndex() {
	std::cout << "所有可用的内存块\n";
	int counter = 0;
	for (const auto& e : blockIndex) {
		counter++;
		std::cout << "第" << counter << "块:[" << std::hex << e.start - memStart << "," << std::hex << e.end - memStart << "]" << std::endl;
	}
	std::cout << std::endl;
}

void first_available() {
	std::cout << "\nshowing first available----------" << std::endl;

	void* assignment1 = Malloc<FirstAvailable>(130 * KB);
	printAddr(assignment1, 1, 130 * KB); printArray();

	void* assignment2 = Malloc<FirstAvailable>(60 * KB);
	printAddr(assignment2, 2, 60 * KB); printArray();

	void* assignment3 = Malloc<FirstAvailable>(100 * KB);
	printAddr(assignment3, 3, 100 * KB); printArray();

	Mfree<FirstAvailable>(assignment2, 60 * KB);
	printArray();

	void* assignment4 = Malloc<FirstAvailable>(200 * KB);
	printAddr(assignment4, 4, 200 * KB); printArray();

	Mfree<FirstAvailable>(assignment3, 100 * KB);
	printArray();

	Mfree<FirstAvailable>(assignment1, 130 * KB);
	printArray();

	void* assignment5 = Malloc<FirstAvailable>(140 * KB);
	printAddr(assignment5, 5, 140 * KB); printArray();

	void* assignment6 = Malloc<FirstAvailable>(60 * KB);
	printAddr(assignment6, 6, 60 * KB); printArray();

	void* assignment7 = Malloc<FirstAvailable>(50 * KB);
	printAddr(assignment7, 7, 50 * KB); printArray();

	Mfree<FirstAvailable>(assignment6, 60 * KB);
	printArray();
}

void best_adpat() {
	std::cout << "\nshowing best adapt-----------" << std::endl;

	void* assignment1 = Malloc<BestAdapt>(130 * KB);
	printAddr(assignment1, 1, 130 * KB); printArray();

	void* assignment2 = Malloc<BestAdapt>(60 * KB);
	printAddr(assignment2, 2, 60 * KB); printArray();

	void* assignment3 = Malloc<BestAdapt>(100 * KB);
	printAddr(assignment3, 3, 100 * KB); printArray();

	Mfree<BestAdapt>(assignment2, 60 * KB);
	printArray();

	void* assignment4 = Malloc<BestAdapt>(200 * KB);
	printAddr(assignment4, 4, 200 * KB); printArray();

	Mfree<BestAdapt>(assignment3, 100 * KB);
	printArray();

	Mfree<BestAdapt>(assignment1, 130 * KB);
	printArray();

	void* assignment5 = Malloc<BestAdapt>(140 * KB);
	printAddr(assignment5, 5, 140 * KB); printArray();

	void* assignment6 = Malloc<BestAdapt>(60 * KB);
	printAddr(assignment6, 6, 60 * KB); printArray();

	void* assignment7 = Malloc<BestAdapt>(50 * KB);
	printAddr(assignment7, 7, 60 * KB); printArray();

	Mfree<BestAdapt>(assignment6, 60 * KB);
	printArray();
}

void HomeWork1() {
	memStart = (char*)malloc(640 * KB);
	init(memStart);
	first_available();
	best_adpat();
}

#endif /* Alloc_h */
