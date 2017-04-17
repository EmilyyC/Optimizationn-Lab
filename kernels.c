/********************************************************
 * Kernels to be optimized for the CS:APP Performance Lab
 ********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "defs.h"
#include <pthread.h>
#include <semaphore.h>

/* 
 * Please fill in the following team struct 
 */
team_t team = {
    "304682182",            /* UID */

    "QinYu (Emily) Chen",          /* Full name */
    "emilyqychen@ucla.edu",  /* Email */

    "",                   /* Leave Blank */
    ""                    /* Leave Blank */
};

//  You should modify only this file -- feel free to modify however you like!

/*
 * setup - This function runs one time, and will not be timed.
 *         You can do whatever initialization you need here, but
 *         it is not required -- don't use if you don't want to.
 */
int NUM_THREADS = 8;
sem_t mutexArry[16]; //////////////////////////////////////////////
sem_t ifselectedArr[16];


struct originParams {
    int dim;
    pixel *src;
    pixel * dst;
};

struct threadParams {
    struct originParams* m_smoothPara;
    int m_t;
};
void *blurr_middle (void *arguments);
struct threadParams threadArray[16];


void setup() {

    struct originParams smoothParams = {1,NULL,NULL};
    pthread_t threads[16];
    int t =0;

    for (t=0; t< 16; t++ )
    {   
        //printf("hiiiiiiii\n");
         sem_init (&(ifselectedArr[t]),0,2);
      
        int k; 
        sem_getvalue(&(ifselectedArr[t]), &k);

         printf("im k%d", k);
        //mutexArry[t] = 0;
        threadArray[t].m_smoothPara = &smoothParams;
        threadArray[t].m_t = t;
        //printf("creating thread %d", t);
        int ret = pthread_create(&threads[t],NULL, &blurr_middle, (void *) &threadArray[t] );

        if (ret)
        {
            printf("thread %d failed \n", t);
            exit(1);
        }

    }
  
}



void *blurr_middle (void *arguments){


    struct threadParams * params = arguments;
    int i,j;
    int t = params -> m_t;

    //printf("%d\n", ifselectedArr[t]);
    sem_wait (&(ifselectedArr[t]));


    int dim = params -> m_smoothPara -> dim;
    int newDim = dim -1;
    int newDimMinus1 = newDim-1;
    pixel *src = params -> m_smoothPara ->  src;
    pixel * dst = params -> m_smoothPara -> dst;



    int imin = 0;
    int imax =0;
    int jminus2 = 0, jminus1 = 0, jpulse1 =0 ;
    int redMiddle =0, greenMiddle =0, blueMiddle=0;
    int redTemp = 0, greenTemp = 0, blueTemp = 0;

    for (i = (t+2); i < newDim; i=i+NUM_THREADS) {
 
        imin = i-1;
        imax = i+1;

        // THIS IS THE CENTER CENTER SECTION 
       for ( j = 2; j < newDim; j += 2) {  
             jminus2 = j-2;
             jminus1 = j-1;
             jpulse1 = j+1;

            //dst[RIDX(i, jminus1, dim)] = avg2(dim, imin, imax, jminus2, j, src,9);
             redMiddle = (int)src[RIDX( imin, jminus1, dim)].red + (int)src[RIDX(imin, j, dim)].   red   +  (int)src[RIDX(i, j, dim)].red+ (int)src[RIDX(i, jminus1, dim)].red+ (int)src[RIDX(imax, jminus1, dim)].red+ (int)src[RIDX(imax, j, dim)].red;
             greenMiddle = (int)src[RIDX( imin, jminus1, dim)].green + (int)src[RIDX(imin, j, dim)].   green   +  (int)src[RIDX(i, j, dim)].green+ (int)src[RIDX(i, jminus1, dim)].green+ (int)src[RIDX(imax, jminus1, dim)].green+ (int)src[RIDX(imax, j, dim)].green;
             blueMiddle = (int)src[RIDX( imin, jminus1, dim)].blue + (int)src[RIDX(imin, j, dim)].   blue   +  (int)src[RIDX(i, j, dim)].blue+ (int)src[RIDX(i, jminus1, dim)].blue+ (int)src[RIDX(imax, jminus1, dim)].blue+ (int)src[RIDX(imax, j, dim)].blue;

             dst[RIDX(i, jminus1, dim)].red   = ( (int)src[RIDX(imin, jminus2, dim)].red   +  (int)src[RIDX(i, jminus2, dim)].red   +  (int)src[RIDX(imax, jminus2, dim)].red + redMiddle)/9 ;
             dst[RIDX(i, jminus1, dim)].green = ( (int)src[RIDX(imin, jminus2, dim)].green +  (int)src[RIDX(i, jminus2, dim)].green +  (int)src[RIDX(imax, jminus2, dim)].green + greenMiddle)/9 ;
             dst[RIDX(i, jminus1, dim)].blue  = ( (int)src[RIDX(imin, jminus2, dim)].blue  + (int)src[RIDX(i, jminus2, dim)].blue + (int)src[RIDX(imax, jminus2, dim)].blue + blueMiddle)/9 ;

            // dst[RIDX(i, j, dim)] = avg2(dim, imin, imax, jminus1, j+1, src,9);
             dst[RIDX(i, j, dim)].red   = ( (int)src[RIDX(imin, jpulse1, dim)].red   +  (int)src[RIDX(i, jpulse1, dim)].red   +  (int)src[RIDX(imax, jpulse1, dim)].red + redMiddle)/9 ;
             dst[RIDX(i, j, dim)].green = ( (int)src[RIDX(imin, jpulse1, dim)].green +  (int)src[RIDX(i, jpulse1, dim)].green +  (int)src[RIDX(imax, jpulse1, dim)].green + greenMiddle)/9 ;
             dst[RIDX(i, j, dim)].blue  = ( (int)src[RIDX(imin, jpulse1, dim)].blue  + (int)src[RIDX(i, jpulse1, dim)].blue + (int)src[RIDX(imax, jpulse1, dim)].blue + blueMiddle)/9 ;       
       }
       //THE RIGHT EDGE SIDE
        // dst[RIDX(i, newDim, dim)] = avg2(dim, imin, imax, newDimMinus1, newDim, src,6);
        dst[RIDX(i, newDim, dim)].red = ((int)src[RIDX(imin, newDimMinus1, dim)].red + (int)src[RIDX(imin, newDim, dim)].red +  (int)src[RIDX(i, newDimMinus1, dim)].red+ (int)src[RIDX(i, newDim, dim)].red + (int)src[RIDX(imax, newDimMinus1, dim)].red+ (int)src[RIDX(imax, newDim, dim)].red)/6;
        dst[RIDX(i, newDim, dim)].green = ((int)src[RIDX(imin, newDimMinus1, dim)].green + (int)src[RIDX(imin, newDim, dim)].green + (int)src[RIDX(i, newDimMinus1, dim)].green+ (int)src[RIDX(i, newDim, dim)].green + (int)src[RIDX(imax, newDimMinus1, dim)].green+ (int)src[RIDX(imax, newDim, dim)].green)/6 ;
        dst[RIDX(i, newDim, dim)].blue = ((int)src[RIDX(imin, newDimMinus1, dim)].blue + (int)src[RIDX(imin, newDim, dim)].blue +  (int)src[RIDX(i, newDimMinus1, dim)].blue+ (int)src[RIDX(i, newDim, dim)].blue + (int)src[RIDX(imax, newDimMinus1, dim)].blue+ (int)src[RIDX(imax, newDim, dim)].blue)/6 ;
        
        // THE LEFT EDGE SIDE
        //dst[RIDX(i, 0, dim)] = avg2(dim, imin, imax, 0, 1, src,6);
        dst[RIDX(i, 0, dim)].red = ((int)src[RIDX(imin, 1, dim)].red + (int)src[RIDX(imin, 0, dim)].red +  (int)src[RIDX(i, 1, dim)].red+ (int)src[RIDX(i, 0, dim)].red + (int)src[RIDX(imax, 1, dim)].red+ (int)src[RIDX(imax, 0, dim)].red)/6;
        dst[RIDX(i, 0, dim)].green = ((int)src[RIDX(imin, 1, dim)].green + (int)src[RIDX(imin, 0, dim)].green + (int)src[RIDX(i, 1, dim)].green+ (int)src[RIDX(i, 0, dim)].green + (int)src[RIDX(imax, 1, dim)].green+ (int)src[RIDX(imax, 0, dim)].green)/6 ;
        dst[RIDX(i, 0, dim)].blue = ((int)src[RIDX(imin, 1, dim)].blue + (int)src[RIDX(imin, 0, dim)].blue +  (int)src[RIDX(i, 1, dim)].blue+ (int)src[RIDX(i, 0, dim)].blue + (int)src[RIDX(imax, 1, dim)].blue+ (int)src[RIDX(imax, 0, dim)].blue)/6 ;
     
        // if Dim is not even or sth 
       if (j < (newDimMinus1)){
        //dst[RIDX(i, j+1, dim)] = avg2(dim, imin, imax, j, j+2, src,9);
        dst[RIDX(i, j+1, dim)].red = ((int)src[RIDX(imin, j+2, dim)].red + (int)src[RIDX(imin, j, dim)].red + (int)src[RIDX(imin, j+1, dim)].red+ (int)src[RIDX(i, j+2, dim)].red+ (int)src[RIDX(i, j, dim)].red+ (int)src[RIDX(i, j+1, dim)].red+ (int)src[RIDX(imax, j+2, dim)].red+ (int)src[RIDX(imax, j, dim)].red+ (int)src[RIDX(imax, j+1, dim)].red )/9;
        dst[RIDX(i, j+1, dim)].green = ((int)src[RIDX(imin, j+2, dim)].green + (int)src[RIDX(imin, j, dim)].green + (int)src[RIDX(imin, j+1, dim)].green+ (int)src[RIDX(i, j+2, dim)].green+ (int)src[RIDX(i, j, dim)].green+ (int)src[RIDX(i, j+1, dim)].green+ (int)src[RIDX(imax, j+2, dim)].green+ (int)src[RIDX(imax, j, dim)].green+ (int)src[RIDX(imax, j+1, dim)].green)/9 ;
        dst[RIDX(i, j+1, dim)].blue = ((int)src[RIDX(imin, j+2, dim)].blue + (int)src[RIDX(imin, j, dim)].blue + (int)src[RIDX(imin, j+1, dim)].blue+ (int)src[RIDX(i, j+2, dim)].blue+ (int)src[RIDX(i, j, dim)].blue+ (int)src[RIDX(i, j+1, dim)].blue+ (int)src[RIDX(imax, j+2, dim)].blue+ (int)src[RIDX(imax, j, dim)].blue+ (int)src[RIDX(imax, j+1, dim)].blue)/9 ;
        }
    }



    for (j = (t+1); j < newDim; j+= NUM_THREADS) {
     jminus1 = j-1;

    //dst[RIDX(newDim, j, dim)] = avg2(dim, newDimMinus1, newDim, jminus1, j+1, src,6);
    dst[RIDX(newDim, j, dim)].red = ((int)src[RIDX(newDimMinus1, jminus1, dim)].red + (int)src[RIDX(newDimMinus1, j, dim)].red + (int)src[RIDX(newDimMinus1, j+1, dim)].red+  (int)src[RIDX(newDim, jminus1, dim)].red+ (int)src[RIDX(newDim, j, dim)].red+ (int)src[RIDX(newDim, j+1, dim)].red )/6;
    dst[RIDX(newDim, j, dim)].green = ((int)src[RIDX(newDimMinus1, jminus1, dim)].green + (int)src[RIDX(newDimMinus1, j, dim)].green + (int)src[RIDX(newDimMinus1, j+1, dim)].green+  (int)src[RIDX(newDim, jminus1, dim)].green+ (int)src[RIDX(newDim, j, dim)].green+ (int)src[RIDX(newDim, j+1, dim)].green)/6 ;
    dst[RIDX(newDim, j, dim)].blue = ((int)src[RIDX(newDimMinus1, jminus1, dim)].blue + (int)src[RIDX(newDimMinus1, j, dim)].blue + (int)src[RIDX(newDimMinus1, j+1, dim)].blue+  (int)src[RIDX(newDim, jminus1, dim)].blue+ (int)src[RIDX(newDim, j, dim)].blue+ (int)src[RIDX(newDim, j+1, dim)].blue)/6 ;

    ////////////////////////////////////////////
    /////////THE TOP EDGE SIDE ////////////////////////////////////////////
    ////////////////////////////////////////////
    //dst[RIDX(0, j, dim)] = avg2(dim, 0, 1, jminus1, j+1, src,6);
    redTemp = (int)src[RIDX( 0, jminus1, dim)].red + (int)src[RIDX( 0, j, dim)].red + (int)src[RIDX( 0, j+1, dim)].red+  (int)src[RIDX(1, jminus1, dim)].red+ (int)src[RIDX(1, j, dim)].red+ (int)src[RIDX(1, j+1, dim)].red ;
    greenTemp = (int)src[RIDX( 0, jminus1, dim)].green + (int)src[RIDX( 0, j, dim)].green + (int)src[RIDX( 0, j+1, dim)].green+  (int)src[RIDX(1, jminus1, dim)].green+ (int)src[RIDX(1, j, dim)].green+ (int)src[RIDX(1, j+1, dim)].green;
    blueTemp = (int)src[RIDX( 0, jminus1, dim)].blue + (int)src[RIDX( 0, j, dim)].blue + (int)src[RIDX( 0, j+1, dim)].blue+  (int)src[RIDX(1, jminus1, dim)].blue+ (int)src[RIDX(1, j, dim)].blue+ (int)src[RIDX(1, j+1, dim)].blue;

    dst[RIDX(0, j, dim)].red = redTemp/6;
    dst[RIDX(0, j, dim)].green = greenTemp /6;
    dst[RIDX(0, j, dim)].blue = blueTemp /6;

    dst[RIDX(1, j, dim)].red = (redTemp + (int)src[RIDX( 2, jminus1, dim)].red + (int)src[RIDX( 2, j, dim)].red + (int)src[RIDX( 2, j+1, dim)].red) /9;
    dst[RIDX(1, j, dim)].green = (greenTemp + (int)src[RIDX( 2, jminus1, dim)].green + (int)src[RIDX( 2, j, dim)].green + (int)src[RIDX( 2, j+1, dim)].green)/9;
    dst[RIDX(1, j, dim)].blue = (blueTemp + (int)src[RIDX( 2, jminus1, dim)].blue + (int)src[RIDX( 2, j, dim)].blue + (int)src[RIDX( 2, j+1, dim)].blue) /9;

    }
    sem_post(&(mutexArry[t]));  //////////////////////////////////////////////
    return NULL;
}



char smooth_descr[] = "smooth: Current working version";
void smooth(int dim, pixel *src, pixel *dst) 
{
    int red_tmp = 0, green_tmp =0, blue_tmp = 0;
    int t;  //i, j
    int newDim = dim-1;
    int newDimMinus1 = newDim-1;
   
    switch (dim) {
    case 64: NUM_THREADS = 2;
    break;
    case 128: NUM_THREADS = 4;
    break;
    case 256: NUM_THREADS = 6;
    break;
    case 512: NUM_THREADS = 8;
    break;
    case 1024: NUM_THREADS = 16;
    break;
    default: NUM_THREADS = 4; 
    }

   // if (dim < 65)
   //      NUM_THREADS = 2;
   //  else if (dim < 129)
   //      NUM_THREADS = 4;
   //  else if  (dim < 257)
   //      NUM_THREADS = 6;
   //  else if (dim < 1025)
   //      NUM_THREADS = 8;
   //   else   
   //       NUM_THREADS = 16;

    

    struct originParams smoothParams = {dim,src,dst};
    struct threadParams threadArray[NUM_THREADS];

    
    pthread_t threads[NUM_THREADS];
    for (t=0; t< NUM_THREADS; t++ )
    {   
        //mutexArry[t] = 0;
        sem_init (&mutexArry[t],0,0);
        threadArray[t].m_smoothPara = &smoothParams;
        threadArray[t].m_t = t;
        //printf("creating thread %d", t);
        int ret = pthread_create(&threads[t],NULL, &blurr_middle, (void *) &threadArray[t] );

        if (ret)
        {
            printf("thread %d failed \n", t);
            exit(1);
        }

    }

    //Corner cases
     // 4 corners

    // TOP LEFT
     //dst[RIDX(0, 0, dim)] = avg2(dim, 0, 1, 0, 1, src,4); //(0,0)
    // dst[RIDX(0, 0, dim)].red = ((int)src[RIDX( 0, 0, dim)].red + (int)src[RIDX( 0, 1, dim)].red+  (int)src[RIDX(1, 0, dim)].red+ (int)src[RIDX(1, 1, dim)].red )/4;
    // dst[RIDX(0, 0, dim)].green = ((int)src[RIDX( 0, 0, dim)].green  + (int)src[RIDX( 0, 1, dim)].green+  (int)src[RIDX(1, 0, dim)].green+ (int)src[RIDX(1, 1, dim)].green)/4 ;
    // dst[RIDX(0, 0, dim)].blue = ((int)src[RIDX( 0, 0, dim)].blue + (int)src[RIDX( 0, 1, dim)].blue+  (int)src[RIDX(1, 0, dim)].blue+ (int)src[RIDX(1, 1, dim)].blue)/4 ;

    red_tmp = (int)src[RIDX( 0, 0, dim)].red + (int)src[RIDX( 0, 1, dim)].red+  (int)src[RIDX(1, 0, dim)].red+ (int)src[RIDX(1, 1, dim)].red ;
    green_tmp = (int)src[RIDX( 0, 0, dim)].green + (int)src[RIDX( 0, 1, dim)].green+  (int)src[RIDX(1, 0, dim)].green+ (int)src[RIDX(1, 1, dim)].green ;
    blue_tmp = (int)src[RIDX( 0, 0, dim)].blue + (int)src[RIDX( 0, 1, dim)].blue+  (int)src[RIDX(1, 0, dim)].blue+ (int)src[RIDX(1, 1, dim)].blue ;

    dst[RIDX(0, 0, dim)].red = red_tmp/4;
    dst[RIDX(0, 0, dim)].green = green_tmp/4;
    dst[RIDX(0, 0, dim)].blue = blue_tmp/4;

    dst[RIDX(1, 0, dim)].red = (red_tmp + (int)src[RIDX(2, 0, dim)].red + (int)src[RIDX(2, 1, dim)].red)/6;
    dst[RIDX(1, 0, dim)].green = (green_tmp + (int)src[RIDX(2, 0, dim)].green + (int)src[RIDX(2, 1, dim)].green)/6;
    dst[RIDX(1, 0, dim)].blue = (blue_tmp + (int)src[RIDX(2, 0, dim)].blue + (int)src[RIDX(2, 1, dim)].blue)/6;


    // TOP RIGHT
     //dst[RIDX(0, newDim, dim)] = avg2(dim, 0, 1, newDimMinus1, newDim, src,4); //(0,max)
    red_tmp = (int)src[RIDX( 0, newDim, dim)].red + (int)src[RIDX( 0, newDimMinus1, dim)].red+  (int)src[RIDX(1, newDim, dim)].red+ (int)src[RIDX(1, newDimMinus1, dim)].red ;
    green_tmp = (int)src[RIDX( 0, newDim, dim)].green + (int)src[RIDX( 0, newDimMinus1, dim)].green+  (int)src[RIDX(1, newDim, dim)].green+ (int)src[RIDX(1, newDimMinus1, dim)].green ;
    blue_tmp = (int)src[RIDX( 0, newDim, dim)].blue + (int)src[RIDX( 0, newDimMinus1, dim)].blue+  (int)src[RIDX(1, newDim, dim)].blue+ (int)src[RIDX(1, newDimMinus1, dim)].blue ;

    dst[RIDX(0, newDim, dim)].red = red_tmp/4;
    dst[RIDX(0, newDim, dim)].green = green_tmp/4;
    dst[RIDX(0, newDim, dim)].blue = blue_tmp/4;

    dst[RIDX(1, newDim, dim)].red = (red_tmp + (int)src[RIDX(2, newDim, dim)].red + (int)src[RIDX(2, newDimMinus1, dim)].red)/6;
    dst[RIDX(1, newDim, dim)].green = (green_tmp + (int)src[RIDX(2, newDim, dim)].green + (int)src[RIDX(2, newDimMinus1, dim)].green)/6;
    dst[RIDX(1, newDim, dim)].blue = (blue_tmp + (int)src[RIDX(2, newDim, dim)].blue + (int)src[RIDX(2, newDimMinus1, dim)].blue)/6;

    // BOTTOM LEFT
    //dst[RIDX(newDim, 0, dim)] = avg2(dim, newDimMinus1, newDim, 0, 1, src,4); //(max,0)
    dst[RIDX(newDim, 0, dim)].red = ((int)src[RIDX( newDim, 0, dim)].red + (int)src[RIDX( newDim, 1, dim)].red+  (int)src[RIDX(newDimMinus1, 0, dim)].red+ (int)src[RIDX(newDimMinus1, 1, dim)].red )/4;
    dst[RIDX(newDim, 0, dim)].green = ((int)src[RIDX( newDim, 0, dim)].green  + (int)src[RIDX( newDim, 1, dim)].green+  (int)src[RIDX(newDimMinus1, 0, dim)].green+ (int)src[RIDX(newDimMinus1, 1, dim)].green)/4 ;
    dst[RIDX(newDim, 0, dim)].blue = ((int)src[RIDX( newDim, 0, dim)].blue + (int)src[RIDX( newDim, 1, dim)].blue+  (int)src[RIDX(newDimMinus1, 0, dim)].blue+ (int)src[RIDX(newDimMinus1, 1, dim)].blue)/4 ;

    // BOTTOM RIGHT
    //dst[RIDX(newDim, newDim, dim)] = avg2(dim, newDimMinus1, newDim, newDimMinus1, newDim, src,4); //(max,max)
    dst[RIDX(newDim, newDim, dim)].red = ((int)src[RIDX( newDim, newDim, dim)].red + (int)src[RIDX( newDim, newDimMinus1, dim)].red+  (int)src[RIDX(newDimMinus1, newDim, dim)].red+ (int)src[RIDX(newDimMinus1, newDimMinus1, dim)].red )/4;
    dst[RIDX(newDim, newDim, dim)].green = ((int)src[RIDX( newDim, newDim, dim)].green  + (int)src[RIDX( newDim, newDimMinus1, dim)].green+  (int)src[RIDX(newDimMinus1, newDim, dim)].green+ (int)src[RIDX(newDimMinus1, newDimMinus1, dim)].green)/4 ;
    dst[RIDX(newDim, newDim, dim)].blue = ((int)src[RIDX( newDim, newDim, dim)].blue + (int)src[RIDX( newDim, newDimMinus1, dim)].blue+  (int)src[RIDX(newDimMinus1, newDim, dim)].blue+ (int)src[RIDX(newDimMinus1, newDimMinus1, dim)].blue)/4 ;

   // for (t=0; t< NUM_THREADS; t++){
   //      pthread_join(threads[t], NULL);
   // }

    for (t=0; t< NUM_THREADS; t++){
        sem_wait(&(mutexArry[t]));
   }
}






// testing different thred
// char smooth_descr_4[] = "smooth: Thread =4";

// void smooth_4(int dim, pixel *src, pixel *dst) 
// {
//     NUM_THREADS=4;
//     smooth(dim, src, dst);

// }
// char naive_smooth_descr[] = "naive_smooth: Naive baseline implementation";
// void naive_smooth(int dim, pixel *src, pixel *dst) 
// {
//     int num_threads =4 ;
//    int t, j;  //i, j
//    int newDim = dim-1;
//     struct originParams smoothParams = {dim,src,dst};
//     struct threadParams threadArray[num_threads];


  
//     pthread_t threads[num_threads];
//     for (t=0; t< num_threads; t++ )
//     {   
//         threadArray[t].m_smoothPara = &smoothParams;
//         threadArray[t].m_t = t;
//         //printf("creating thread %d", t);
//         int ret = pthread_create(&threads[t],NULL, &blurr_middle, (void *) &threadArray[t] );

//         if (ret)
//         {
//             printf("thread %d failed \n", t);
//             exit(1);
//         }

//     }

//      dst[RIDX(0, 0, dim)] = avg2(dim, 0, 1, 0, 1, src,4); //(0,0)
//      dst[RIDX(0, newDim, dim)] = avg2(dim, 0, 1, newDimMinus1, newDim, src,4); //(0,max)
//      dst[RIDX(newDim, 0, dim)] = avg2(dim, newDimMinus1, newDim, 0, 1, src,4); //(max,0)
//      dst[RIDX(newDim, newDim, dim)] = avg2(dim, newDimMinus1, newDim, newDimMinus1, newDim, src,4); //(max,max)
   

//     //middle rows

//     // When i = 0 
//     for (j = 1; j < newDim; j++) {
//     dst[RIDX(newDim, j, dim)] = avg2(dim, newDimMinus1, newDim, jminus1, j+1, src,6);
//     dst[RIDX(0, j, dim)] = avg2(dim, 0, 1, jminus1, j+1, src,6);
//     }
    
//    for (t=0; t< num_threads; t++){
//         pthread_join(threads[t], NULL);
//    }
// }
/********************************************************************* 
 * register_smooth_functions - Register all of your different versions
 *     of the smooth kernel with the driver by calling the
 *     add_smooth_function() for each test function.  When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_smooth_functions() {
    add_smooth_function(&smooth, smooth_descr);
    //NUM_THREADS=4;
    //add_smooth_function(&smooth_4, smooth_descr_4);
    /* ... Register additional test functions here */
}






// IGNORE EVERYTHING AFTER THIS POINT!!!!!!!!!!!!!!!!!!!!!!!!!!!!!111
// You are not implementing rotate


/***************
 * ROTATE KERNEL
 ***************/

/******************************************************
 * Your different versions of the rotate kernel go here
 ******************************************************/

/* 
 * naive_rotate - The naive baseline version of rotate 
 */
char naive_rotate_descr[] = "naive_rotate: Naive baseline implementation";
void naive_rotate(int dim, pixel *src, pixel *dst) 
{
    int i, j;

    for (i = 0; i < dim; i++)
    for (j = 0; j < dim; j++)
        dst[RIDX(dim-1-j, i, dim)] = src[RIDX(i, j, dim)];
}

/* 
 * rotate - Your current working version of rotate
 * IMPORTANT: This is the version you will be graded on
 */
char rotate_descr[] = "rotate: Current working version";
void rotate(int dim, pixel *src, pixel *dst) 
{
    naive_rotate(dim, src, dst);
}

/*********************************************************************
 * register_rotate_functions - Register all of your different versions
 *     of the rotate kernel with the driver by calling the
 *     add_rotate_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_rotate_functions() 
{
    add_rotate_function(&naive_rotate, naive_rotate_descr);   
    //add_rotate_function(&rotate, rotate_descr);   
    /* ... Register additional test functions here */
}



