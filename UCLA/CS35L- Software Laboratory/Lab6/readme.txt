The focus of the multithreading performance laid in the main.c file where the original code only supported for only one thread. Therefore I had to change the code in a way that would be able to allow for greater than one thread, and to do so I realized that the change had to be made in the nested loops that went through the width and height of the image. Knowing that pthread_create took in only one function to create and allows only one argument to be passed into the
function I realized that I had to create one function in which the entire chunk of code (nested for loops) had to go.

I made the scene variable as a global variable as it  had to be used before the multithreading began. pthread_create takes in a void* argument to the function we wish to multithread and so I decided to create an array of threadID's an pass in a reference to these id's and then cast them to integer pointers and dereference them in the function.

The main issue of the lab was how to multithread the image rendering, and I came to the conclusion that  if I start each thread's for loop at the thread number and increment by the total number of threads then I could multithread by breaking the image into sections for each thread to render. 

To solve this I created a 3-D array of the width, height and color of the image and store each value for each thread in its respective indices and then print in the main function to ensure the printing and rendering is the same as the case with one thread.

They were a few other issues regarding to silly compilation errors such as having to return NULL because I had to create my function as a void* function as that is the only type of function pthread_create takes in.

Also, in main.c I used thread_join to join all the threads after the threading was complete

Here we can see the results of the timings of rendering each image below.

time ./srt 1-test.ppm >1-test.ppm.tmp
real	0m21.500s
user	0m21.485s
sys	0m0.007s

mv 1-test.ppm.tmp 1-test.ppm
time ./srt 2-test.ppm >2-test.ppm.tmp

real	0m10.954s
user	0m21.786s
sys	0m0.008s
mv 2-test.ppm.tmp 2-test.ppm
time ./srt 4-test.ppm >4-test.ppm.tmp

real	0m5.791s
user	0m22.995s
sys	0m0.008s
mv 4-test.ppm.tmp 4-test.ppm
time ./srt 8-test.ppm >8-test.ppm.tmp

real	0m4.266s
user	0m32.984s
sys	0m0.017s

I noticed a significant improvement in the real timing of the program because as the number of threads increase, the ray tracing of the image is split into concurrent tasks amongst the threads and so instead of being serial, we parallelize the task.