#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/time.h>

char buffer[2][1000];

typedef struct matrix{
    int rows;
    int cols;
    double arr[20][20];
}matrix;
matrix m[5];

struct dims{
    int row;
    int col;
};

void read_input(int id , char *txt){
    FILE *fp = fopen(txt, "r");
    if(fp == NULL){
        printf("error");
        exit(1);
    }
    int i= 0;
    //read all characters writtin in the file.
    while(!feof(fp)){
        buffer[id][i++] = fgetc(fp);
    }
    buffer[id][i-1] = '\0';
    fclose(fp);
}

void analyze(int id){
    char *ptr = buffer[id];
    int dim[2];
    int i =0;
    while (*ptr) { // While there are more characters to process...
        if ( isdigit(*ptr) ) {
            // Found a number
            dim[i] = (int)strtol(ptr,&ptr, 10); // Read number
            i++; // and print it.
            if(i == 2){
                break;
            }
        } else {
            // Otherwise, move on to the next character.
            ptr++;
        }
    }
    i = 0;
    int j =0;
    m[id].rows = dim[0];
    m[id].cols = dim[1];
    m[id].arr[dim[0]][dim[1]];
    while (*ptr) { // While there are more characters to process...
        if ( isdigit(*ptr) ) {
            // Found a number
            m[id].arr[i][j] = strtod(ptr,&ptr); // Read number
            j++; // and print it.
            if(j == m[id].cols){
                j = 0;
                i++;
            }
        } else {
            // Otherwise, move on to the next character.
            ptr++;
        }
    }
}

void *perMatrix(){
    for(int i = 0; i<m[0].rows ; i++){
        for(int j=0; j<m[1].cols ; j++){
            m[2].arr[i][j] =0;
            for(int k =0; k<m[0].cols; k++){
                m[2].arr[i][j] += m[0].arr[i][k]*m[1].arr[k][j];
            }
        }
    }
}
void execute_perMAtrix(pthread_t th1){
    pthread_create(&th1, NULL, perMatrix,NULL);
    pthread_join(th1, NULL);
}
void *perRow(void * i){
    long h = (long) i;
    for(int j =0; j<m[1].cols; j++){
        m[3].arr[h][j] = 0;
        for(int k=0; k<m[0].cols; k++){
            m[3].arr[h][j] += m[0].arr[h][k]*m[1].arr[k][j];
        }
    }
}
void execute_perRow(pthread_t th2[m[0].rows]){
    for(long i=0; i<m[0].rows; i++){
        pthread_create(&th2[i], NULL, perRow, (void *)i);
    }

    for(int i=0; i<m[0].rows; i++){
        pthread_join(th2[i], NULL);
    }
}
void *perElement(void * data){
    struct dims *myData;
    myData = (struct dims *) data;
    int i = myData->row;
    int j = myData->col;
    m[4].arr[i][j] = 0;
    for(int k=0; k<m[0].cols; k++){
        m[4].arr[i][j] += m[0].arr[i][k]*m[1].arr[k][j];
    }
}
void execute_perElement(pthread_t th3[m[0].rows][m[1].cols]){
    struct dims *data;
    for(int i=0; i< m[0].rows; i++){
        for(int j=0; j< m[1].cols; j++){
            data = malloc(sizeof(struct dims));
            data->row = i;
            data->col = j;
            pthread_create(&th3[i][j], NULL, perElement, (void *)data);
        }
    }
    for(int i=0; i< m[0].rows; i++){
        for(int j=0; j< m[1].cols; j++){
            pthread_join(th3[i][j], NULL);
        }
    }
    free(data);
}
int main(int argc , char *argv[]){
    read_input(0, argv[1]);
    read_input(1, argv[2]);
    analyze(0);
    analyze(1);
    m[2].rows = m[0].rows;
    m[2].cols = m[1].cols;
    pthread_t th1;
    pthread_t th2[m[0].rows];
    pthread_t th3[m[0].rows][m[1].cols];

    struct timeval stop, start;
    gettimeofday(&start, NULL); //start checking time
    execute_perMAtrix(th1);
    gettimeofday(&stop, NULL); //end checking time
    printf("Thread per matrix:\nSeconds taken %lu\n", stop.tv_sec - start.tv_sec);
    printf("Microseconds taken: %lu\n", stop.tv_usec - start.tv_usec);

    gettimeofday(&start, NULL);
    execute_perRow(th2);
    gettimeofday(&stop, NULL);
    printf("Thread per row:\nSeconds taken %lu\n", stop.tv_sec - start.tv_sec);
    printf("Microseconds taken: %lu\n", stop.tv_usec - start.tv_usec);

    gettimeofday(&start, NULL);
    execute_perElement(th3);
    gettimeofday(&stop, NULL);
    printf("Thread per element:\nSeconds taken %lu\n", stop.tv_sec - start.tv_sec);
    printf("Microseconds taken: %lu\n", stop.tv_usec - start.tv_usec);

    char out1[100];
    char name1[] = " _per_matrix";
    int i;
    for(i=0; i< strlen(argv[3]); i++){
        out1[i] = argv[3][i];
    }
    for(i= i; i< strlen(name1); i++){
        out1[i] = name1[i];
    }
    out1[i] = '\0';
    FILE *fp = fopen(out1, "w");
    if (fp == NULL)
    {
        printf("Error opening the file %s", out1);
        return -1;
    }
    fprintf(fp, "Method: A thread per matrix\n");
    fprintf(fp, "row=%d col=%d\n", m[2].rows, m[2].cols);
    for(int i =0; i< m[2].rows; i++){
        for(int j=0; j< m[2].cols; j++){
            fprintf(fp, "%lf ", m[2].arr[i][j]);
        }
        fprintf(fp,"\n");
    }
    fclose(fp);
    char out2[100];
    char name2[] = " _per_row";
    int j;
    for(j=0; j< strlen(argv[3]); j++){
        out2[j] = argv[3][j];
    }
    for(j= j; j< strlen(name2); j++){
        out2[j] = name2[j];
    }
    out2[j] = '\0';
    fp = fopen(out2, "w");
    if (fp == NULL)
    {
        printf("Error opening the file %s", out2);
        return -1;
    }
    fprintf(fp, "Method: A thread per row\n");
    fprintf(fp, "row=%d col=%d\n", m[2].rows, m[2].cols);
    for(int i =0; i< m[2].rows; i++){
        for(int j=0; j< m[2].cols; j++){
            fprintf(fp, "%lf ", m[3].arr[i][j]);
        }
        fprintf(fp,"\n");
    }
    fclose(fp);
    char *out3 = argv[3];
    out3 = strcat(out3,"_per_element");
    fp = fopen(out3, "w");
    if (fp == NULL)
    {
        printf("Error opening the file %s", out3);
        return -1;
    }
    fprintf(fp, "Method: A thread per element\n");
    fprintf(fp, "row=%d col=%d\n", m[2].rows, m[2].cols);
    for(int i =0; i< m[2].rows; i++){
        for(int j=0; j< m[2].cols; j++){
            fprintf(fp, "%lf ", m[4].arr[i][j]);
        }
        fprintf(fp,"\n");
    }
    fclose(fp);
    return 0;
}
