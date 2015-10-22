#include "stdio.h"
// �k�������Ŗ��߂�
void strcls(char *str) {
    while(*str != '\0') *str++ = '\0';
}
 
// ���l��16�i��������ɕϊ�����
void int2hex(char *s, int value) {
    s[0] = '0', s[1] = 'x';
    int i, filter = 0x0000000f;
    s += 2;
    for(i = 0; i < 8; i++) {
        if(((value >> (7-i)*4) & filter) >= 10) {
            *s++ = 'A' + ((value >> (7-i)*4) & filter) - 10;
        } else {
            *s++ = '0' + ((value >> (7-i)*4) & filter);
        }
    }
    *s = '\0';
}
 
// 10�i��value��n���ڂ�Ԃ�
int figure(int value, int n) {
    int i;
    for(i = 0; i < n-1; i++) value /= 10;
    return value % 10;
}
 
// ���l��10�i��������ɕϊ�����
void int2dec(char *s, int value) {
    int i;
    char zero = 1;
    for(i = 0; i < 10; i++) {
        if(zero && figure(value, 10-i) != 0) zero = 0;
        if(!zero) *s++ = '0' + figure(value, 10-i);
    }
}



int sprintf(char *str, const char *fmt, ...) {
    int *arg = (int *)(&str + 2);   // �ό����̔z��
    int cnt, i, argc = 0;
    char buf[20];
    const char *p = fmt;
    for(cnt = 0; *p != '\0'; p++) {
        if(*p == '%') {
            strcls(buf);    // �o�b�t�@�̏�����
            // �t�H�[�}�b�g�w��q�̏ꍇ�͈����̐��l�𕶎���֕ϊ�
            switch(p[1]) {
                case 'd': int2dec(buf, arg[argc++]); break;
                case 'x': int2hex(buf, arg[argc++]); break;
            }
            // �ϊ��������l�𐶐�������ɃR�s�[
            for(i = 0; buf[i] != '\0'; i++,cnt++) *str++ = buf[i];
            p++;
        } else {
            // �t�H�[�}�b�g�w��q�ȊO�͂��̂܂܃R�s�[
            *str++ = *p; cnt++;
        }
    }
    return cnt;
}