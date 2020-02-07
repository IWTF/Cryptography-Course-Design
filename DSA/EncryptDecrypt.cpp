#include <iostream>
#include <ctime>
#include "EncryptDecrypt.h"

/**
 * 函数功能:菜单显示
 */
void EncryptDecrypt::menu() {
	/* 计算前，先初始化 */
	
    std::cout << "**********Welcome to use DSA encoder**********" << std::endl;
    std::cout << "               s: signature 签名             " << std::endl;
    std::cout << "               v: verification 验证          " << std::endl;
    std::cout << "               p: print   显示               " << std::endl;
    std::cout << "               r: reset   重置               " << std::endl;
    std::cout << "               q: quit    退出               " << std::endl;
    std::cout << "input your choice:" << std::endl;
}

/**
 * 函数功能:签名运算
 */
bool EncryptDecrypt::encrypt() {
    std::string str;
    std::cout << "输入16进制数据:" << std::endl;
    std::cout << ">";
    std::cin >> str;// 输入明文
    if (!std::cin || !islegal(str))
        return false;
    
	
    m = BigInteger(str);
    clock_t start = clock();
    
    dsa.signature(m, r, s);
    clock_t finish = clock();

    std::cout << std::fixed;
    std::cout.precision(3);
    std::cout << "用时: " << (double)(finish-start)/CLOCKS_PER_SEC << "s." << std::endl;
    std::cout << "\n============签名过程===========\n"
              << "明文: " << m << std::endl;
    std::cout << "签名: " << std::endl
              << "r = " << r << std::endl
              << "s = " << s << std::endl;

    return true;
}

/**
 * 函数功能:验证运算
 */
bool EncryptDecrypt::decrypt() {
	clock_t start = clock();
	
    int ret = dsa.verification(m, r, s, w, u1, u2);
    clock_t finish = clock();

    std::string str = "success！";
    if (ret = 0) 
        str="failure!";

    std::cout << std::fixed;
    std::cout.precision(3);
    std::cout << "用时: " << (double)(finish-start)/CLOCKS_PER_SEC << "s." << std::endl;
    std::cout << "w: " << w << std::endl;
    std::cout << "u1: " << u1 << std::endl;
    std::cout << "u2: " << u2 << std::endl;
    std::cout << "验证结果: " << str << std::endl;
    return true;
}

/**
 * 函数功能:输出RSA相关信息
 */
void EncryptDecrypt::print() {
    std::cout << dsa << std::endl;
}

/**
 * 函数功能:重置RSA相关信息
 */
void EncryptDecrypt::reset() {
    std::cout << "输入密钥长度: (512-1024)";
    int len;
    std::cin >> len;
    load(len>>1);
}

/**
 * 函数功能:根据给定位数len加载rsa
 */
void EncryptDecrypt::load(int len) {
    std::cout << "初始化..." << std::endl;
    clock_t start = clock();
    dsa.init(len);    // 初始化
    clock_t finish = clock();
    std::cout << "初始化完成." << std::endl;
    std::cout << std::fixed;
    std::cout.precision(3);
    std::cout << "用时: " << (double)(finish-start)/CLOCKS_PER_SEC << "s." << std::endl;
}

/**
 * 函数功能:判断输入字符串str是否合法
 * 参数含义:str代表输入的字符串
 */
bool EncryptDecrypt::islegal(const std::string & str) {
    for (std::string::const_iterator it=str.begin(); it!=str.end(); ++it) {
        if (!isalnum(*it))    // 不是字母或者数字
            return false;
        if (isalpha(*it)) {
            char ch = tolower(*it);
            if (ch > 'f')    // 超过十六进制字符'f'
                return false;
        }
    }
    return true;
}
