#include "klib.h"

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)
/* dhrystone 原来一直测试过不去，一直死循环，通过一些替换，发现是strcpy的锅
 */
size_t strlen(const char *s) {
  size_t i = 0;
  while(s[i] && s[i] != '\0') i++;
  return i;
}

char *strcpy(char* dst,const char* src) {
  size_t i;
  for(i=0;src[i]!='\0';i++) 
    dst[i]=src[i];
  
  dst[i]='\0';
  return dst;
}

char *strncpy(char* dst, const char* src, size_t n) {
  size_t i;
  for (i = 0; i < n && src[i]!= '\0'; i++) dst[i] = src[i];
  for (; i < n; i++) dst[i] = '\0';
  return dst;
}

char* strcat(char* dst, const char* src) {
  char *str=dst;
  assert(dst!=NULL && src!=NULL);
  while(*dst!='\0') {
    dst++;
  }
  while((*dst++ = *src++) != '\0');
  return str;
}

int strcmp(const char* s1, const char* s2) {
  // size_t s1_size = strlen(s1);
  // size_t s2_size = strlen(s2);
  // size_t n = s1_size < s2_size ? s1_size : s2_size;
  // return strncmp(s1, s2, n);

  uint32_t i = 0;
  for (i = 0; s1[i] == s2[i]; i++) if(s1[i] == '\0') return 0;

  return ((signed char *)s1)[i] - ((signed char *)s2)[i];
}

int strncmp(const char* s1, const char* s2, size_t n) {
  // size_t s1_size = strlen(s1);
  // size_t s2_size = strlen(s2);
  
  // if (s1_size < n) n = s1_size;
  // if (s2_size < n) n = s2_size;
  
  // uint32_t i = 0;
  // for (i = 0; i < n; i++) {
  // 	int D_value = s1[i] - s2[i];
  // 	if (D_value) return D_value;
  // }
  // return 0;

  int D_value = 0;
  uint32_t  i = 0;
  
  for (i = 0; *s1 && *s2 && i < n && D_value != 0; i++) {
    s1++; s2++;
  	D_value = *s1 - *s2;
  }

  return D_value;
}

void* memset(void* v,int c,size_t n) {
  const unsigned char uc = c; /* only cat the lowest c */
  unsigned char *vu;
  for (vu = v; 0 < n; ++vu, --n)
	  *vu = uc;
  return v;
}

void* memcpy(void* out, const void* in, size_t n) {
  const char *src = in;
  char *dst = out;

  while (n--) *dst++ = *src++;

  return out;
}

int memcmp(const void* s1, const void* s2, size_t n){
  if(!n) return 0;
  while(--n && *(char *)s1 == *(char *)s2) {
	s1 = (char *)s1 + 1;
	s2 = (char *)s2 + 1;
  }
  return (*((unsigned char *)s1) - *((unsigned char *)s2));
}

#endif