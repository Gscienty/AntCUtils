// AntUtils.Test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <iostream>
#include "..\AntUtils\Array.h"

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	Array<int> a(20);
	for (int i = 0; i < 10; i++){
		a.Push(i);
	}
	a.Find([](int i){ return i > 2; })
		.Sort([](const int &a, const int &b){
		return a > b;
	}).ForEach([](int i){
		cout << i;
	});
	getchar();
	return 0;
}
