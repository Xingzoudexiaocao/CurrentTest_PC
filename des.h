/*
*文件:Des.h
*功能:Des相关算法

*时间:2008-09-23
*/
// #include <stdlib.h>
#ifndef __DES_H__
#define __DES_H__
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//加解密模式
// enum CryptModel{DES,DES3,OTHER};
// //MAC模式
// enum MACModel{ANSI_DATAXNOR_DES,ANSI_DATAXNOR_3DES,ANSI_XNOR_DES,ANSI_XNOR_3DES,
// 			PBOC_DATAXNOR_DES,PBOC_DATAXNOR_3DES,PBOC_XNOR_DES,PBOC_XNOR_3DES};
/*des加密算法*/
/*参数：m:8字节明文 k八字节密钥 e八字节密文*/
extern void endes(unsigned char m[8],unsigned char k[8],unsigned char e[8]);

/*des解密算法*/
/*参数：e:8字节密文 k八字节密钥 m八字节明文*/
extern void undes(unsigned char e[8],unsigned char k[8],unsigned char m[8]);

///*3des加密算法*/
///*参数：m:8字节明文 k八字节密钥 e八字节密文*/
//extern void en3des(unsigned char m[8],unsigned char k[16],unsigned char e[8]);
//
///*3des加密算法*/
///*参数：m:8字节明文 k八字节密钥 e八字节密文*/
//extern void un3des(unsigned char e[8],unsigned char k[16],unsigned char m[8]);

#endif
