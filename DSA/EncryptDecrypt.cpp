#include <iostream>
#include <ctime>
#include "EncryptDecrypt.h"

/**
 * ��������:�˵���ʾ
 */
void EncryptDecrypt::menu() {
	/* ����ǰ���ȳ�ʼ�� */
	
    std::cout << "**********Welcome to use DSA encoder**********" << std::endl;
    std::cout << "               s: signature ǩ��             " << std::endl;
    std::cout << "               v: verification ��֤          " << std::endl;
    std::cout << "               p: print   ��ʾ               " << std::endl;
    std::cout << "               r: reset   ����               " << std::endl;
    std::cout << "               q: quit    �˳�               " << std::endl;
    std::cout << "input your choice:" << std::endl;
}

/**
 * ��������:ǩ������
 */
bool EncryptDecrypt::encrypt() {
    std::string str;
    std::cout << "����16��������:" << std::endl;
    std::cout << ">";
    std::cin >> str;// ��������
    if (!std::cin || !islegal(str))
        return false;
    
	
    m = BigInteger(str);
    clock_t start = clock();
    
    dsa.signature(m, r, s);
    clock_t finish = clock();

    std::cout << std::fixed;
    std::cout.precision(3);
    std::cout << "��ʱ: " << (double)(finish-start)/CLOCKS_PER_SEC << "s." << std::endl;
    std::cout << "\n============ǩ������===========\n"
              << "����: " << m << std::endl;
    std::cout << "ǩ��: " << std::endl
              << "r = " << r << std::endl
              << "s = " << s << std::endl;

    return true;
}

/**
 * ��������:��֤����
 */
bool EncryptDecrypt::decrypt() {
	clock_t start = clock();
	
    int ret = dsa.verification(m, r, s, w, u1, u2);
    clock_t finish = clock();

    std::string str = "success��";
    if (ret = 0) 
        str="failure!";

    std::cout << std::fixed;
    std::cout.precision(3);
    std::cout << "��ʱ: " << (double)(finish-start)/CLOCKS_PER_SEC << "s." << std::endl;
    std::cout << "w: " << w << std::endl;
    std::cout << "u1: " << u1 << std::endl;
    std::cout << "u2: " << u2 << std::endl;
    std::cout << "��֤���: " << str << std::endl;
    return true;
}

/**
 * ��������:���RSA�����Ϣ
 */
void EncryptDecrypt::print() {
    std::cout << dsa << std::endl;
}

/**
 * ��������:����RSA�����Ϣ
 */
void EncryptDecrypt::reset() {
    std::cout << "������Կ����: (512-1024)";
    int len;
    std::cin >> len;
    load(len>>1);
}

/**
 * ��������:���ݸ���λ��len����rsa
 */
void EncryptDecrypt::load(int len) {
    std::cout << "��ʼ��..." << std::endl;
    clock_t start = clock();
    dsa.init(len);    // ��ʼ��
    clock_t finish = clock();
    std::cout << "��ʼ�����." << std::endl;
    std::cout << std::fixed;
    std::cout.precision(3);
    std::cout << "��ʱ: " << (double)(finish-start)/CLOCKS_PER_SEC << "s." << std::endl;
}

/**
 * ��������:�ж������ַ���str�Ƿ�Ϸ�
 * ��������:str����������ַ���
 */
bool EncryptDecrypt::islegal(const std::string & str) {
    for (std::string::const_iterator it=str.begin(); it!=str.end(); ++it) {
        if (!isalnum(*it))    // ������ĸ��������
            return false;
        if (isalpha(*it)) {
            char ch = tolower(*it);
            if (ch > 'f')    // ����ʮ�������ַ�'f'
                return false;
        }
    }
    return true;
}
