/*
 * test.cpp
 *
 *  Created on: 2011-9-13
 *      Author: sun
 */
#include <iostream>

#include "SimpleHttp.h"
#include "strhelper.h"

using namespace std;
using namespace sunjwbase;

int main()
{
	SimpleHttp http;

	string strUrl("http://www.baidu.com");
	string strRet;
	http.DoGet(strUrl, strRet, false);
	cout << strRet << endl;

	strRet = strutf8tougbk(strRet);
	cout << strRet << endl;

	strRet = strgbktoutf8(strRet);
	cout << strRet << endl;

	return 0;
}
