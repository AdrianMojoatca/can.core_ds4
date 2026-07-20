#ifndef AES128_H

#define AES128_H

#define KEY_SIZE  16

void memxor(UInt8 *dst,const UInt8 *src,UInt8 len);
void aes_encrypt(UInt8 *key,UInt8 *plain, UInt8 *cipher);
void aes_decrypt(UInt8 *key, UInt8 *cipher,UInt8 *plain);

#endif
