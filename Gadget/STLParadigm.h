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

pCompare:����ָ�� bool (*id)(args) ����typeof,Ҳ����decltype()
*/

using namespace std;
#include<string>
//��������һЩ����,��ʹ�ò�ͬ�ķ�ʽ����������
struct Obj {
	string describe;
	int value;
	int id;
	friend bool operator== (const Obj& l, const Obj& r){
		return l.describe == r.describe;
	}
};

//�ǵ�Ҫ����Ϊ������
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

//�����ֻ��ϣ�������Ǵ�����
#include <vector>
vector<Obj> vObj{ {"Hu",0,0},{"Wife",100,2},{"me",120,4} };
//�����ϣ����������
#include <set>
//set ��Ҫһ������ָ��CMP,Ĭ��Ϊ<,����С����,��key��const
set<Obj, ObjCmp> sObj{ vObj.begin(),vObj.end()};

//�����ϣ��ӳ������,����Ҫ����==�͵�����һ��std::hash<T>()
#include<unordered_set>
unordered_set < Obj, ObjHash,ObjEq > hObj{ vObj.begin(),vObj.end() };

#include <iostream>

void doIt() {
	
	
	
}

