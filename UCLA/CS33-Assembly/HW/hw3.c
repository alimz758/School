void psumla(float a[], float p[], long n)
{

 long i;
 float last_val, val;
 last_val = p[O] = a[O];
 for (i = 1; i < n; i+=6) {
 	//unrolling five times
 	//using local variables for reassociation
 	//since I implemented i+=6 rather than i++;  therefore, the number of iterations has reduced, instead
 	//I used 5 addition to get the same result as question 5.12
 	//I used local variables to make it faster as well as the book has reccommended
 	val = last_val + a [i] ;
 	p[i] = val;
 	last_val = val;

 	val = last_val + a [i+1] ;
 	p[i+1] = val;
 	last_val = val;

 	val = last_val + a [i+2] ;
 	p[i+2] = val;
 	last_val = val;

 	val = last_val + a [i+3] ;
 	p[i+3] = val;
 	last_val = val;

 	val = last_val + a [i+4] ;
 	p[i+4] = val;
 	last_val = val;

 	val = last_val + a [i+5] ;
 	p[i+5] = val;
 	last_val = val;
 }
//for the last five
for (; i < n; i++)
{
	p[i]= a[i]+p[i-1];
}

}