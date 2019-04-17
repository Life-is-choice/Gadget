#pragma once
#include <thread>
//STL���߳����
/*
thread
mutex
conditional_variable
atomic
future
*/

/*
thread(f_name,args) t;
1������֮ǰ����join����detach
2ģ���Ƶ���ȥ�����ã�ʹ��������Ϊ����ʱҪ����std::ref(var)
*/

/*
mutex m;
1�ٽ�����һ�����ڴ���εĸ������m.lock��m.unlock�����Ĵ����ٽ����������ٽ�������m
2lock_gard<T>��һ��RAII���wrapper
���´��빹���ٽ�������m��һ��Ԫ��lck
{
	lock_gurad<std::mutex> lck(m);
	//...critical code
}
3�������ϣ����ʾ�ö����ٽ����Ľ�����ʹ��unique_lock,����lock_gurad��������
4����m�е��ٽ������ǻ����(�����Լ����Լ���ͬʱִ��Ҳ�ǻ����)
*/

/*
condition_variable cv;
1һ������������¼m�����ص�Ԫ��(��Щ�ٽ��������ص�)
2����ͨ�����ء��ü�����ֻ��һ����ʾԪ���������ٽ�����ִ��˳��
3���´��뽫һ���ٽ�������ֱ������������ʹ�ñ�׼��д���Ա�����ٻ���
bool itsTimeForIt = false;
cv.wait(lck,[](){
	return itsTimeForIt;
});
4cv.notify_all();��m�е�ÿһ��Ԫ�س���һ�λ��ѡ�ע�⣬������ռʽ�ġ�
5notify֮ǰҪȷ����������ʾԪ��Ϊ0,������ע��ʧ��
	
atomic<T> a;
ʹ������a���ʱ��ԭ�ӵģ���֤��һ��ʱ��ֻ��һ���߳��ڷ���a
*/

/*
future
�첽����
*/

//������һ������˳�������,master��pretreat����ִ��֮��masterȷ��pretreat��ɼ���ִ��
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