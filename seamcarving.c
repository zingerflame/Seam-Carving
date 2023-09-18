#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "c_img.h"
#include "c_img.c"
#include "seamcarving.h"
#include <math.h>

void calc_energy(struct rgb_img* im, struct rgb_img** grad){

    create_img(grad, im->height, im->width);
    
    for(int y = 0; y < im->height; y++){
        for(int x = 0; x < im->width; x++){ //loop every coord

            // REMEMBER COORDS ARE (y,x)
            int dx2;
            int dy2;
            double energy;
            int rx; int gx; int bx;
            int ry; int gy; int by;

            // if y = 0 or x = 0 then wrap around code:
            if (x==0 && y == 0){ // top left corner
                rx = get_pixel(im, y, x+1, 0) - get_pixel(im, y, im->width-1, 0);
                gx = get_pixel(im, y, x+1, 1) - get_pixel(im, y, im->width-1, 1);
                bx = get_pixel(im, y, x+1, 2) - get_pixel(im, y, im->width-1, 2);
                ry = get_pixel(im, y+1, x, 0) - get_pixel(im, im->height-1, x, 0);
                gy = get_pixel(im, y+1, x, 1) - get_pixel(im, im->height-1, x, 1);
                by = get_pixel(im, y+1, x, 2) - get_pixel(im, im->height-1, x, 2);
            }

            else if (x==0 && y == im->height-1){ // bottom left corner
                rx = get_pixel(im, y, x+1, 0) - get_pixel(im, y, im->width-1, 0);
                gx = get_pixel(im, y, x+1, 1) - get_pixel(im, y, im->width-1, 1);
                bx = get_pixel(im, y, x+1, 2) - get_pixel(im, y, im->width-1, 2);
                ry = get_pixel(im, 0, x, 0) - get_pixel(im, y-1, x, 0);
                gy = get_pixel(im, 0, x, 1) - get_pixel(im, y-1, x, 1);
                by = get_pixel(im, 0, x, 2) - get_pixel(im, y-1, x, 2);
            }

            else if (x==im->width-1 && y == 0){ // top right corner
                rx = get_pixel(im, y, 0, 0) - get_pixel(im, y, x-1, 0);
                gx = get_pixel(im, y, 0, 1) - get_pixel(im, y, x-1, 1);
                bx = get_pixel(im, y, 0, 2) - get_pixel(im, y, x-1, 2);
                ry = get_pixel(im, y+1, x, 0) - get_pixel(im, im->height-1, x, 0);
                gy = get_pixel(im, y+1, x, 1) - get_pixel(im, im->height-1, x, 1);
                by = get_pixel(im, y+1, x, 2) - get_pixel(im, im->height-1, x, 2);
            }

            else if (x==im->width-1 && y == im->height-1){ // bottom right corner
                rx = get_pixel(im, y, 0, 0) - get_pixel(im, y, x-1, 0);
                gx = get_pixel(im, y, 0, 1) - get_pixel(im, y, x-1, 1);
                bx = get_pixel(im, y, 0, 2) - get_pixel(im, y, x-1, 2);
                ry = get_pixel(im, 0, x, 0) - get_pixel(im, y-1, x, 0);
                gy = get_pixel(im, 0, x, 1) - get_pixel(im, y-1, x, 1);
                by = get_pixel(im, 0, x, 2) - get_pixel(im, y-1, x, 2);
            }

            else if (x == 0){ // left edge
                rx = get_pixel(im, y, x+1, 0) - get_pixel(im, y, im->width-1, 0);
                gx = get_pixel(im, y, x+1, 1) - get_pixel(im, y, im->width-1, 1);
                bx = get_pixel(im, y, x+1, 2) - get_pixel(im, y, im->width-1, 2);
                ry = get_pixel(im, y+1, x, 0) - get_pixel(im, y-1, x, 0);
                gy = get_pixel(im, y+1, x, 1) - get_pixel(im, y-1, x, 1);
                by = get_pixel(im, y+1, x, 2) - get_pixel(im, y-1, x, 2);
            }

            else if (x == im->width-1){ // right edge
                rx = get_pixel(im, y, 0, 0) - get_pixel(im, y, x-1, 0);
                gx = get_pixel(im, y, 0, 1) - get_pixel(im, y, x-1, 1);
                bx = get_pixel(im, y, 0, 2) - get_pixel(im, y, x-1, 2);
                ry = get_pixel(im, y+1, x, 0) - get_pixel(im, y-1, x, 0);
                gy = get_pixel(im, y+1, x, 1) - get_pixel(im, y-1, x, 1);
                by = get_pixel(im, y+1, x, 2) - get_pixel(im, y-1, x, 2);
            }

            else if (y == 0){ //top edge
                rx = get_pixel(im, y, x+1, 0) - get_pixel(im, y, x-1, 0);
                gx = get_pixel(im, y, x+1, 1) - get_pixel(im, y, x-1, 1);
                bx = get_pixel(im, y, x+1, 2) - get_pixel(im, y, x-1, 2);
                ry = get_pixel(im, y+1, x, 0) - get_pixel(im, im->height-1, x, 0);
                gy = get_pixel(im, y+1, x, 1) - get_pixel(im, im->height-1, x, 1);
                by = get_pixel(im, y+1, x, 2) - get_pixel(im, im->height-1, x, 2);
            }

            else if (y == im->height-1){ // bottom edge
                rx = get_pixel(im, y, x+1, 0) - get_pixel(im, y, x-1, 0);
                gx = get_pixel(im, y, x+1, 1) - get_pixel(im, y, x-1, 1);
                bx = get_pixel(im, y, x+1, 2) - get_pixel(im, y, x-1, 2);
                ry = get_pixel(im, 0, x, 0) - get_pixel(im, y-1, x, 0);
                gy = get_pixel(im, 0, x, 1) - get_pixel(im, y-1, x, 1);
                by = get_pixel(im, 0, x, 2) - get_pixel(im, y-1, x, 2);
            }

            else{ // anywhere in middle
                rx = get_pixel(im, y, x+1, 0) - get_pixel(im, y, x-1, 0);
                gx = get_pixel(im, y, x+1, 1) - get_pixel(im, y, x-1, 1);
                bx = get_pixel(im, y, x+1, 2) - get_pixel(im, y, x-1, 2);
                ry = get_pixel(im, y+1, x, 0) - get_pixel(im, y-1, x, 0);
                gy = get_pixel(im, y+1, x, 1) - get_pixel(im, y-1, x, 1);
                by = get_pixel(im, y+1, x, 2) - get_pixel(im, y-1, x, 2);
            }

            // do something with coords
            dx2 = rx*rx + gx*gx + bx*bx;
            dy2 = ry*ry + gy*gy + by*by;
            energy = sqrt(dx2 + dy2);

            //casting
            uint8_t nrg = (uint8_t)(energy / 10);

            //fill in
            set_pixel(*grad, y, x, nrg, nrg, nrg);
        }
    }
}

double min(double a, double b){
    if (a <= b){
        return a;
    }
    else{
        return b;
    }
}

void dynamic_seam(struct rgb_img* grad, double** best_arr){
    (*best_arr) = (double*)malloc((grad->height*grad->width)*sizeof(double));

    int width = grad->width; int height = grad->height;
    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){

            //top row base case
            if (y==0){
                (*best_arr)[y*width+x] = (double)get_pixel(grad, y, x, 0);
            }
            
            // left n right edge case
            else if (x == 0){
                (*best_arr)[y*width+x] = (double)get_pixel(grad, y, x, 0) + min((*best_arr)[(y-1)*width+x], (*best_arr)[(y-1)*width+x+1]);
            }
            else if (x == width-1){
                (*best_arr)[y*width+x] = (double)get_pixel(grad, y, x, 0) + min((*best_arr)[(y-1)*width+x], (*best_arr)[(y-1)*width+x-1]);

            }
            else{
                (*best_arr)[y*width+x] = (double)get_pixel(grad, y, x, 0) + min((*best_arr)[(y-1)*width+x-1], min((*best_arr)[(y-1)*width+x], (*best_arr)[(y-1)*width+x+1]));
            } 

        }
    }
}

int min_loc(double* list, int width, int y, int x1, int x2){ //compares two items in a list and returns the minimum coordinate of x
    if (list[y*width + x1] <= list[y*width + x2]){
        return x1;
    }
    else{
        return x2;
    }
}

void recover_path(double* best, int height, int width, int** path){
    // consider double* best as (*best_arr)

    (*path) = (int*)malloc(height*sizeof(int));

    double minval = -1;
    int initial_min_coord = -1;
    for(int x = 0; x < width; x++){
        if (minval == -1){ // first number iterated
            minval = best[(height-1)*width+x];
            initial_min_coord = x;
        }

        else if (best[(height-1)*width+x] < minval){
            minval = best[(height-1)*width+x];
            initial_min_coord = x;
        }
    }
    (*path)[height-1] = initial_min_coord;
    //start pt acquired

    for (int y = height-2; y >= 0; y--){
        int min_coord;

        //left n right edge cases
        if (initial_min_coord == 0){
            min_coord = min_loc(best, width, y, initial_min_coord, initial_min_coord+1);
        }
        else if (initial_min_coord == width-1){
            min_coord = min_loc(best, width, y, initial_min_coord, initial_min_coord-1);
        }
        else {
            min_coord = min_loc(best, width, y, initial_min_coord, min_loc(best, width, y, initial_min_coord-1, initial_min_coord+1)); //3 coord comparison
        }

        //update initial for next iter
        initial_min_coord = min_coord;
        //left-append
        (*path)[y] = initial_min_coord;
    }

}

void remove_seam(struct rgb_img* src, struct rgb_img** dest, int* path){
    create_img(dest, src->height, src->width-1);

    for(int y = 0; y < src->height; y++){
        for(int x = 0; x < src->width; x++){
            //copy the raster but skip x = path[y]
            if (x < path[y]){
                int red = get_pixel(src, y, x, 0);
                int green = get_pixel(src, y, x, 1);
                int blue = get_pixel(src, y, x, 2);
                set_pixel(*dest, y, x, red, green, blue);
            }
            else if (x > path[y]){
                int red = get_pixel(src, y, x, 0);
                int green = get_pixel(src, y, x, 1);
                int blue = get_pixel(src, y, x, 2);
                set_pixel(*dest, y, x-1, red, green, blue);
            }
        }
    }
}

int main(){

    //test pt 1

    struct rgb_img* im;
    read_in_img(&im, "YOURIMHERE");
    struct rgb_img *grad;
  
    calc_energy(im,  &grad);
  
    print_grad(grad);

    //test pt 2
    double* best_arr;
    dynamic_seam(grad, &best_arr);

    int height = grad->height;
    int width = grad->width;
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            printf("%f\t", best_arr[i*width + j]);
        }
    printf("\n");    
    }

    // test pt 3
    int* path = 0;
    recover_path(best_arr, height, width, &path);

    for (int i = 0; i < height; i++){
        printf("%i\t", path[i]);
    }

    printf("\n");
    printf("\n");
    //test pt 4 & 5
    struct rgb_img *cur_im;
    double *best;

//     read_in_img(&im, "6x5.bin");
    
//     for(int i = 0; i < 4; i++){
//         //printf("i = %d\n", i);
//         calc_energy(im,  &grad);
//         dynamic_seam(grad, &best);
//         recover_path(best, grad->height, grad->width, &path);
//         remove_seam(im, &cur_im, path);

// /////////////////////
//         int height = grad->height;
//         int width = grad->width;
//         for(int i = 0; i < height; i++){
//             for(int j = 0; j < width; j++){
//                 printf("%f\t", best[i*width + j]);
//             }
//             printf("\n");    
//         }
//         for (int i = 0; i < height; i++){
//             printf("%i\t", path[i]);
//         }
//         printf("\n");
// ////////////////
//         char filename[200];
//         sprintf(filename, "small_img%d.bin", i);
//         write_img(cur_im, filename);

//         destroy_image(im);
//         destroy_image(grad);
//         free(best);
//         free(path);
//         im = cur_im;
//     }
//     destroy_image(im);

    read_in_img(&im, "YOURIMHERE");
    
    for(int i = 0; i < 200; i++){
        printf("i = %d\n", i);
        calc_energy(im,  &grad);
        dynamic_seam(grad, &best);
        recover_path(best, grad->height, grad->width, &path);
        remove_seam(im, &cur_im, path);

        destroy_image(im);
        destroy_image(grad);
        free(best);
        free(path);
        im = cur_im;
    }
    write_img(cur_im, "gru200.bin");
    destroy_image(im);
}