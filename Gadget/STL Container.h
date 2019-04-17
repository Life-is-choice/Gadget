#pragma once
//基类接口
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

*/

//以下是平衡排序树、哈希表的使用范式
using namespace std;
#include<string>
#include <vector>
#include <set>
#include<unordered_set>
#include<algorithm>

//模板需要的类
struct Obj {
	string describe;
	int value;
	int id;
	friend bool operator== (const Obj& l, const Obj& r){
		return l.describe == r.describe;
	}
};
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
vector<Obj> vObj{ {"Hu",0,0},{"Wife",100,2},{"me",120,4} };

//set----order by key
//key为const,当Cmp为<时,从小到大排序,with O(log)
set<Obj, ObjCmp> sObj{ vObj.begin(),vObj.end()};

//hash----map by key
//key为const,with O(1)
unordered_set < Obj, ObjHash,ObjEq > hObj{ vObj.begin(),vObj.end() };

void doContainer() {

}

