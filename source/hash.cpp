#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <openssl/sha.h>

#include "hash.h"

using namespace std;

char *GenerateHash::getSHA512(string key) {
  int i;
  unsigned char digest[SHA512_DIGEST_LENGTH];
           char *md = (char *)malloc(SHA512_DIGEST_LENGTH*2+1);
           char *ckey = strdup(key.c_str());

  SHA512((unsigned char*)&ckey, strlen(ckey), (unsigned char*)&digest);

  for (i = 0; i < SHA512_DIGEST_LENGTH; i++) {
    sprintf(&md[i*2], "%02x", (unsigned int)digest[i]);
  }

  return md;
}

char *GenerateHash::getSHA256(string key) {
  int i;
  unsigned char digest[SHA256_DIGEST_LENGTH];
           char *md = (char *)malloc(SHA256_DIGEST_LENGTH*2+1);
           char *ckey = strdup(key.c_str());

  SHA256((unsigned char*)&ckey, strlen(ckey), (unsigned char*)&digest);

  for (i = 0; i < SHA256_DIGEST_LENGTH; i++) {
    sprintf(&md[i*2], "%02x", (unsigned int)digest[i]);
  }

  return md;
}
