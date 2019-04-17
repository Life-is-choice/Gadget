#pragma once
#include <thread>
//STL多线程组成
/*
thread
mutex
conditional_variable
atomic
future
*/

/*
thread(f_name,args) t;
1在析构之前必须join或者detach
2模板推导会去掉引用，使用引用作为参数时要加上std::ref(var)
*/

/*
mutex m;
1临界区是一个对于代码段的概念，被其m.lock、m.unlock包含的代码临界区域即属于临界区集合m
2lock_gard<T>是一个RAII风格wrapper
以下代码构造临界区集合m的一个元素lck
{
	lock_gurad<std::mutex> lck(m);
	//...critical code
}
3我们如果希望显示得定义临界区的结束，使用unique_lock,它是lock_gurad的升级版
4集合m中的临界区们是互斥的(包括自己与自己的同时执行也是互斥的)
*/

/*
condition_variable cv;
1一个条件变量记录m中隐藏的元素(哪些临界区是隐藏的)
2我们通过隐藏、让集合中只有一个显示元素来定义临界区的执行顺序
3以下代码将一个临界区隐藏直到该它出场，使用标准的写法以避免虚假唤醒
bool itsTimeForIt = false;
cv.wait(lck,[](){
	return itsTimeForIt;
});
4cv.notify_all();让m中的每一个元素尝试一次唤醒。注意，这是抢占式的。
5notify之前要确保集合中显示元素为0,否则唤醒注定失败
	
atomic<T> a;
使对数据a访问变成原子的，保证了一个时刻只有一个线程在访问a
*/

/*
future
异步工具
*/

//以下是一个控制顺序的例子,master和pretreat并行执行之后，master确认pretreat完成继续执行
#include<iostream>
#include<mutex>
#include<condition_variable>
bool timeToContinue = false;
std::condition_variable cv;
std::mutex m;

void master() {
	std::cout << "master executing\n";
	std::unique_lock<std::mutex> lck(m);
	cv.wait(lck, []() {return timeToContinue; });
	std::cout << "master executing thing need preparation\n";
	std::cout << "master finish\n";
}

void pretreat() {
	std::unique_lock<std::mutex> lck(m);
	std::cout << "callback executing\n";
	std::cout << "callback finish\n";
	timeToContinue = true;
	lck.unlock();
	cv.notify_one();
}

void doThreading() {
	std::thread t1(master), t2(pretreat);
	t1.join();
	t2.join();
}