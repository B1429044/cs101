#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE* fp;
    int a[] = {0,1,2};
    char b[] = "ABC";
    float c[] = {1.1,1.2,1.3};
    int a_read[3];
    char b_read[4];
    float c_read[3];
    
    fp = fopen("a.bin", "wb+");
    fwrite(a, sizeof(int), 3, fp);
    fwrite(b, sizeof(char), 4, fp);
    fwrite(c, sizeof(float), 3, fp);
    
    fseek(fp, 0, SEEK_SET);

    fread(a_read, sizeof(int), 3, fp);
    fread(b_read, sizeof(char), 4, fp);
    fread(c_read, sizeof(float), 3, fp);
    
    for (int i = 0; i < 3; i++) {
        printf("%d ", a_read[i]);
    }
    printf("\n");

    printf("%s\n", b_read);

    for (int i = 0; i < 3; i++) {
        printf("%f ", c_read[i]);
    }
    printf("\n");

    return 0;
}
