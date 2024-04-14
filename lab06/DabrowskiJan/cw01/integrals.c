#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h> 
#include <wait.h>


double function(double rectangle_center){
    return 4.0/(rectangle_center*rectangle_center + 1);
}

double countIntegral(int rectangle_index, double rectangle_width){
    double rectangle_center;
    if ((rectangle_index + 1)*rectangle_width > 1){
        rectangle_center = (rectangle_index*rectangle_width+1)/2.0;
        return (1-rectangle_index*rectangle_width) * function(rectangle_center);
    }
    rectangle_center = (rectangle_index+0.5)*rectangle_width;

    return rectangle_width * function(rectangle_center);
}


int main(int argc, char *argv[]){
    if (argc != 3){
        printf("Usage: %s <rectangle width> <processes num>\n", argv[0]);
        return 1;
    }
    struct timeval start, end;

    gettimeofday(&start, NULL);

    char *endptr;
    double rectangle_width = strtod(argv[1], &endptr);

    if (rectangle_width == 0){
        printf("Rectangle width needs to be non-zero\n");
        return 1;
    }

    int rectangles_no = 1.0/rectangle_width;
    if (rectangles_no * rectangle_width < 1){
        rectangles_no += 1;
    }

    int processes_no = atoi(argv[2]);

    if(processes_no > rectangles_no){
        printf("More processes than needed, limiting the number...\n");
        processes_no = rectangles_no;
    }

    printf("rectangles number: %d, processes number: %d\n", rectangles_no, processes_no);

    int fd[processes_no][2];

    for (int i=0;i<processes_no;i++){
        if (pipe(fd[i]) == -1){
            perror("pipe");
            return 1;
        }
    }

    double partial;

    double counted;

    double integral = 0;

    pid_t pid;

    int reps;

    for (int i=0;i<processes_no;i++){

        if((pid = fork()) == -1){
            perror("pid");
            return 1;
        }
        
        if (pid != 0){
            continue;
        }

        close(fd[i][0]);
        reps = 0;
        while (i + processes_no*reps < rectangles_no){
            counted += countIntegral(i + processes_no*reps, rectangle_width);
            reps += 1;
        }
        write(fd[i][1], &counted , sizeof(double));

        close(fd[i][1]);
        return 0;

    }

    int status;
    for (int i = 0; i < processes_no; i++) {
        if (wait(&status) == -1) {
            perror("wait");
            exit(EXIT_FAILURE);
        }
    }

    for (int i=0;i<processes_no;i++){
        partial = 0;

        close(fd[i][1]);
        read(fd[i][0], &partial, sizeof(double));
        integral += partial;

        close(fd[i][0]);  
    }

    gettimeofday(&end, NULL);

    double elapsed_time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;

    printf("%f\n", integral);
    printf("Duration: %f\n", elapsed_time);

    int file = open("./times.txt", O_RDONLY | O_WRONLY);
    if (file == -1) {
        perror("open");
        return 1;
    }
    off_t offset = 0;
    if (lseek(file,offset,SEEK_END) == -1){
        perror("lseek");
        return 1;
    }

    char ratio_string[100];
    char total_time_string[100];
    char separator_string[] = "----------------------------------------------------------------------------------\n";

    sprintf(ratio_string, "Rects per process ratio: %f (Number of rectangles: %d, number of processes: %d)\n", (double)rectangles_no/processes_no, rectangles_no, processes_no);
    sprintf(total_time_string, "Total time: %f s\n", elapsed_time);

    char multiline_string[300];
    snprintf(multiline_string, sizeof(multiline_string), "%s%s%s", ratio_string, total_time_string, separator_string);

    write(file, multiline_string, strlen(multiline_string));

    close(file);

    return 0;
}