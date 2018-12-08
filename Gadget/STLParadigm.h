#pragma once
//STL API FORM
/*
swap()
max_size()
empty()
size()
emplace(inits)
insert()
erase()
clear()
begin()
end()

pCompare:函数指针 bool (*id)(args) 或者typeof,也就是decltype()
*/

using namespace std;
#include<string>
//假设我有一些对象,我使用不同的方式来处理它们
struct Obj {
	string describe;
	int value;
	int id;
	friend bool operator== (const Obj& l, const Obj& r){
		return l.describe == r.describe;
	}
};

//记得要声明为常函数
class ObjCmp {
public:
	bool operator()(const Obj& l, const Obj& r) {
		return l.id > r.id;
	}
};
class ObjEq {
public:
	bool operator()(const Obj& l, const Obj& r) const {
		return l.describe == r.describe;
	}
};
class ObjHash {
public:
	size_t operator() (const Obj& o)const{
		return std::hash<string>()(o.describe);
	}
};

//如果你只是希望把它们存起来
#include <vector>
vector<Obj> vObj{ {"Hu",0,0},{"Wife",100,2},{"me",120,4} };
//如果你希望索引它们
#include <set>
//set 需要一个函数指针CMP,默认为<,即从小到大,且key是const
set<Obj, ObjCmp> sObj{ vObj.begin(),vObj.end()};

//如果你希望映射它们,你需要定义==和调用哪一个std::hash<T>()
#include<unordered_set>
unordered_set < Obj, ObjHash,ObjEq > hObj{ vObj.begin(),vObj.end() };

#include <iostream>

void doIt() {
	
	
	
}

