#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SAMPLE 5000
#define MAXLEN 20
#define MAXDEF 1000
#define SOURCE "sample_account.csv"

struct account {
  char first_kanji[MAXLEN];
  char first_kana[MAXLEN];
  char first_name[MAXLEN];
  char last_kanji[MAXLEN];
  char last_kana[MAXLEN];
  char last_name[MAXLEN];
  char prefecture[MAXLEN];
};

int main(int argc, char *argv[]) {
  int max = MAXDEF;
  int i = 0, ret;

  if (argc > 1) {
    max = atoi(argv[argc - 1]);
  }

  FILE *fp;
  struct account sample[SAMPLE];

  if ((fp = fopen(SOURCE, "r")) != NULL) {
    while((ret = fscanf(fp, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%s\n",
    sample[i].first_kanji, sample[i].first_kana, sample[i].first_name,
    sample[i].last_kanji,  sample[i].last_kana,  sample[i].last_name,
    sample[i].prefecture)) != EOF)
    ++i;
  }
  fclose(fp);

  int first, last, pref;
  srand((unsigned)time(NULL));
  for(i = 0; i < max; i++) {
    first = rand()%SAMPLE;
    last  = rand()%SAMPLE;
    pref  = rand()%SAMPLE;

    printf("%d,%s,%s,%s,%s,%s_%s@sfdc.demo,090-%04d-%04d,%s\n", i+1,
    sample[last].last_kanji, sample[first].first_kanji, sample[last].last_kana, sample[first].first_kana,
    sample[first].first_name, sample[last].last_name, rand()%10000, rand()%10000, sample[pref].prefecture);
  }
}
