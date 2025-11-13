#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define COUNTER_FILE "counter.bin"
#define MAX_LOTTO_NUM 7      // 六個號碼 + 一個特別號
#define MAX_LOTTO_NUMSET 5   // 每張樂透最多 5 組

// 函式前置宣告
int num_in_numset(int num, int numset[], int Len);
void init_file();
int get_counter();
void set_counter(int counter);
void do_lotto_main(int counter);
void print_lottofile(int num_set, int counter, char lotto_file[]);
void print_lotto_row(FILE* tmpfp, int n);
void print_file_to_screen(char lotto_file[]);

// 初始化 counter.bin
void init_file() {
    int write_array[1] = {0};
    FILE* fp = fopen(COUNTER_FILE, "rb");
    if (fp == NULL) {
        FILE* tmpfp = fopen(COUNTER_FILE, "wb+");
        fwrite(write_array, sizeof(int), 1, tmpfp);
        fclose(tmpfp);
    } else {
        fclose(fp);
    }
}

// 讀取計數器
int get_counter() {
    int read_array[1];
    FILE* tmpfp = fopen(COUNTER_FILE, "rb");
    fread(read_array, sizeof(int), 1, tmpfp);
    fclose(tmpfp);
    return read_array[0];
}

// 寫入計數器
void set_counter(int counter) {
    FILE* tmpfp = fopen(COUNTER_FILE, "wb");
    fwrite(&counter, sizeof(int), 1, tmpfp);
    fclose(tmpfp);
}

// 判斷號碼是否重複
int num_in_numset(int num, int numset[], int Len) {
    for (int i = 0; i < Len; i++) {
        if (num == numset[i])
            return 1;
    }
    return 0;
}

// 主流程
void do_lotto_main(int counter) {
    char lotto_file[32];
    int num_set = 0;
    snprintf(lotto_file, sizeof(lotto_file), "lotto[%05d].txt", counter);

    printf("歡迎光臨長庚樂透購買機台\n");
    printf("請問您要買幾組樂透彩（最多 %d 組）：", MAX_LOTTO_NUMSET);
    scanf("%d", &num_set);

    if (num_set < 1 || num_set > MAX_LOTTO_NUMSET) {
        printf("組數錯誤，請輸入 1~%d。\n", MAX_LOTTO_NUMSET);
        return;
    }

    print_lottofile(num_set, counter, lotto_file);

    printf("\n已為您購買的 %d 組樂透組合輸出至 %s\n\n", num_set, lotto_file);

    // 顯示檔案內容
    print_file_to_screen(lotto_file);
}

// 產生樂透檔案
void print_lottofile(int num_set, int counter, char lotto_file[]) {
    time_t curtime;
    time(&curtime);

    FILE* tmpfp = fopen(lotto_file, "w+");
    fprintf(tmpfp, "======== lotto649 =========\n");
    fprintf(tmpfp, "========+ No.%05d +========\n", counter);

    // 日期後換行
    fprintf(tmpfp, "= %.*s\n", 24, ctime(&curtime));  

    for (int i = 0; i < MAX_LOTTO_NUMSET; i++) {
        if (i < num_set)
            print_lotto_row(tmpfp, i + 1);
        else
            fprintf(tmpfp, "[%d]: -- -- -- -- -- --\n", i + 1);
    }

    fprintf(tmpfp, "========= csie@CGU =========\n");
    fclose(tmpfp);
}

// 產生單組樂透號碼
void print_lotto_row(FILE* tmpfp, int n) {
    int numset[MAX_LOTTO_NUM] = {0};

    // 產生前 6 個不重複號碼
    for (int i = 0; i < MAX_LOTTO_NUM - 1;) {
        int num = (rand() % 69) + 1;
        if (!num_in_numset(num, numset, i)) {
            numset[i] = num;
            i++;
        }
    }

    // 排序前 6 個號碼
    for (int i = 0; i < MAX_LOTTO_NUM - 2; i++) {
        for (int j = i + 1; j < MAX_LOTTO_NUM - 1; j++) {
            if (numset[i] > numset[j]) {
                int temp = numset[i];
                numset[i] = numset[j];
                numset[j] = temp;
            }
        }
    }

    // 產生特別號（1~10）
    int special;
    do {
        special = (rand() % 10) + 1;
    } while (num_in_numset(special, numset, MAX_LOTTO_NUM - 1));
    numset[MAX_LOTTO_NUM - 1] = special;

    // 輸出
    fprintf(tmpfp, "[%d]: ", n);
    for (int i = 0; i < MAX_LOTTO_NUM; i++) {
        fprintf(tmpfp, "%02d ", numset[i]);
    }
    fprintf(tmpfp, "\n");
}

// 顯示產生的檔案內容
void print_file_to_screen(char lotto_file[]) {
    FILE* fp = fopen(lotto_file, "r");
    if (!fp) {
        printf("無法開啟檔案 %s\n", lotto_file);
        return;
    }

    printf("======= 檔案內容 =======\n");
    char line[128];
    while (fgets(line, sizeof(line), fp)) {
        printf("%s", line);
    }
    printf("=========================\n");
    fclose(fp);
}

// 主程式
int main() {
    srand(time(NULL));  // 設定亂數種子
    init_file();

    int counter = get_counter() + 1;
    set_counter(counter);

    do_lotto_main(counter);

    return 0;
}
