#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define VERSION 0.10
#define SAMPLE 5000
#define ADDNUM 122240
#define ADDLEN 36
#define MAXLEN 16
#define MAXDEF 1000
#define SOURCE "sample_account.csv"
#define ADDLST "address.csv"
#define DOMAIN "example.com"

struct account {
  char first_kanji[MAXLEN];
  char first_kana[MAXLEN];
  char first_name[MAXLEN];
  char last_kanji[MAXLEN];
  char last_kana[MAXLEN];
  char last_name[MAXLEN];
  char prefecture[MAXLEN];
};

struct address {
  char zip[MAXLEN];
  char address[ADDLEN];
};

int main(int argc, char *argv[]) {
  int max = MAXDEF;
  int i = 0, ret;

  if (argc > 1) {
    max = atoi(argv[argc - 1]);
  }

  FILE *fp;
  struct account sample[SAMPLE];

  /* Input name informations from sample_account.csv */
  if ((fp = fopen(SOURCE, "r")) != NULL) {
    while((ret = fscanf(fp, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%s\n",
    sample[i].last_kanji, sample[i].last_kana, sample[i].last_name,
    sample[i].first_kanji,  sample[i].first_kana,  sample[i].first_name,
    sample[i].prefecture)) != EOF)
    ++i;
  }
  fclose(fp);

  struct address addresses[ADDNUM];
  i = 0;
  if ((fp = fopen(ADDLST, "r")) != NULL) {
    while((ret = fscanf(fp, "%[^,],%s\n", addresses[i].zip, addresses[i].address)) != EOF)
    ++i;
  }
  fclose(fp);

  int  first, last, region;
  char domain[] = DOMAIN;
  srand((unsigned)time(NULL));

  for(i = 0; i < max; i++) {
    first  = rand()%SAMPLE;
    last   = rand()%SAMPLE;
    region = rand()%ADDNUM;

    printf("%d,%s,%s,%s,%s,%s_%s@%s,090-%04d-%04d,%s,%s,%d-%d,%s\n", i+1,
    sample[last].last_kanji, sample[first].first_kanji, sample[last].last_kana, sample[first].first_kana,
    sample[first].first_name, sample[last].last_name, domain, rand()%10000, rand()%10000,
    addresses[region].zip, addresses[region].address, rand()%10, rand()%20, sample[rand()%SAMPLE].prefecture);
  }
}
