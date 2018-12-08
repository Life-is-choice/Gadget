#pragma once
//
//  Pages.h
//  HomeWork
//
//  Created by 刘泽滨 on 2018/12/6.
//  Copyright © 2018年 刘泽滨. All rights reserved.
//

#ifndef Pages_h
#define Pages_h
#define     OPT     1
#define     FIFO    2
#define     LRU     3
using namespace std;
#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>
vector<int> path;
deque<int*> memoryQueue;
int instruction[320];
int instructions[320];
int* page[32];
int pageCounter;
int pc;
int numOfMissPage;

int pc_generator(int pc) {
	int speed = rand() % 100;
	//before
	if (speed < 25) {
		return rand() % (pc + 1);
	}
	//after
	else if (speed < 50) {
		return pc + (rand() % (319 - pc));
	}
	//+1
	else {
		return pc = pc + 1 > 319 ? 319 : pc + 1;
	}

}

int pageNum(int* pi) {
	return (pi - instructions) / 10;
}

void reset() {
	pc = 0;
	numOfMissPage = 0;
	pageCounter = 0;
	memoryQueue.clear();
}

void init_instruction() {
	for (int i = 0; i < 320; i++) {
		instructions[i] = i;
	}
	for (int i = 0; i < 320; i++) {
		instruction[i] = pc_generator(i);
	}
	for (int i = 0; i < 32; i++) {
		page[i] = instructions + i * 10;
	}
}

bool is_page_in_memoryQueue() {
	for (const auto& e : memoryQueue) {
		for (int i = 0; i < 10; i++) {
			if (e[i] == instruction[pc]) {
				return true;
			}
		}
	}
	return false;
}

void print_memoryQueue() {
	cout << "内存中的页:\n";
	for (const auto& e : memoryQueue) {
		cout << "第" << pageNum(e) + 1 << "页\t";
	}
	cout << endl;
}

int* find_instruction(int insNum) {
	//循环展开
	//    bool tmp1 = false;
	//    bool tmp2 = false;
	//    for(int i=0;i!=320;i++){
	//        tmp1 = instruction[i]==insNum;
	//        tmp2 = instruction[i+1]==insNum;
	//    }
	for (int i = 0; i != 320; i++) {
		if (instructions[i] == insNum) {
			return instructions + i;
		}
	}
	return nullptr;
}

int distance(int pageNum) {
	int res = 0; auto itr = path.begin() + pageCounter;
	for (; itr != path.end(); itr++) {
		if (*itr == pageNum) {
			return res;
		}
		else {
			res++;
		}
	}
	return res;
}

int get_farest_page_num() {
	int res = -1; int tmpMax = -1;
	int distance[4];
	for (int i = 0; i < 4; i++) {
		distance[i] = ::distance(pageNum(memoryQueue[i]));
	}
	for (int i = 0; i < 4; i++) {
		if (distance[i] > tmpMax) {
			tmpMax = distance[i];
			res = i;
		}
	}
	return pageNum(memoryQueue[res]);
}

void remove_page(int pageNum) {
	deque<int*> tmpQue;
	int* tmp;
	while (1) {
		tmp = memoryQueue.back();
		if (::pageNum(tmp) == pageNum) {
			memoryQueue.pop_back();
			break;
		}
		else {
			tmpQue.push_back(tmp);
			memoryQueue.pop_back();
		}
	}
	while (!tmpQue.empty()) {
		int* temp = tmpQue.back();
		tmpQue.pop_back();
		memoryQueue.push_back(temp);
	}
}
//1 12 2 8 ||17 1 15 2 13 24 3 1 16 2
//dis(1)=1
//dis(12)>32
//dis(2)=3
//dis(8)=32
void run_OPT() {
	while (pc != 320) {
		cout << "当前指令序号:" << instruction[pc] << endl;
		//不用置换
		if (is_page_in_memoryQueue()) {}
		//置换
		else {
			if (memoryQueue.size() < 4) {
				int* finded = find_instruction(instruction[pc]);
				memoryQueue.push_back(page[pageNum(finded)]);
			}
			else {
				int farestPageNum = get_farest_page_num();
				remove_page(farestPageNum);
				int* finded = find_instruction(instruction[pc]);
				memoryQueue.push_back(page[pageNum(finded)]);
			}
			print_memoryQueue();
			numOfMissPage++;
			pageCounter++;
		}
		pc++;
	}
	cout << "320条指令模拟完毕 置换方式<OPT> 缺页率:" << numOfMissPage / 3.2 << "%" << endl;
}

void run_LRU() {
	while (pc != 320) {
		cout << "当前指令序号:" << instructions[pc] << endl;
		//不用置换
		if (is_page_in_memoryQueue()) {
			int counter = 0; int pageNumber = -1;
			//find number
			for (const auto& e : memoryQueue) {
				for (int i = 0; i < 10; i++) {
					if (e[i] == instruction[pc]) {
						pageNumber = counter;
					}
				}
				counter++;
			}

			deque<int*> tmpQue;
			int* tmp;
			while (1) {
				tmp = memoryQueue.back();
				if (pageNum(tmp) == pageNumber) {
					memoryQueue.pop_back();
					break;
				}
				else {
					tmpQue.push_back(tmp);
					memoryQueue.pop_back();
				}
			}
			while (!tmpQue.empty()) {
				int* temp = tmpQue.back();
				tmpQue.pop_back();
				memoryQueue.push_back(temp);
			}
			memoryQueue.push_back(tmp);
		}
		//
		else {
			if (memoryQueue.size() < 4) {
				int* finded = find_instruction(pc);
				memoryQueue.push_back(page[pageNum(finded)]);
			}
			else {
				memoryQueue.pop_front();
				int* finded = find_instruction(instruction[pc]);
				memoryQueue.push_back(page[pageNum(finded)]);
			}
			print_memoryQueue();
			numOfMissPage++;
		}
		pc++;
	}
	cout << "320条指令模拟完毕 置换方式<LRU> 缺页率:" << numOfMissPage / 3.2 << "%" << endl;
}

void run_FIFO() {

	while (pc != 320) {
		cout << "当前指令序号:" << instruction[pc] << endl;
		//不用置换
		if (is_page_in_memoryQueue()) {}
		//置换
		else {
			if (memoryQueue.size() < 4) {
				int* finded = find_instruction(instruction[pc]);
				memoryQueue.push_back(page[pageNum(finded)]);
				path.push_back(pageNum(finded));
			}
			else {
				memoryQueue.pop_front();
				int* finded = find_instruction(instruction[pc]);
				memoryQueue.push_back(page[pageNum(finded)]);
				path.push_back(pageNum(finded));
			}

			print_memoryQueue();
			numOfMissPage++;
		}
		pc++;
	}
	cout << "320条指令模拟完毕 置换方式<FIFO> 缺页率:" << numOfMissPage / 3.2 << "%" << endl;
}


void execute(int style) {
	reset();
	switch (style) {
	case OPT:
		run_OPT();
		break;

	case FIFO:
		run_FIFO();
		break;

	case LRU:
		run_LRU();
		break;

	default:
		std::cout << "illegal style\n";
		break;
	}
}

void HomeWork2() {
	init_instruction();
	execute(FIFO);
	execute(LRU);
	execute(OPT);
}

#endif /* Pages_h */
