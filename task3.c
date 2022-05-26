#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>

int fib(int n){
  int t1 = 0, t2 = 1;
  int nextTerm = t1 + t2;
  fprintf(stderr,"Fibonacci:%d,%d,", t1, t2);
  nextTerm= t1+t2;
  while(nextTerm <= n) {
    printf("%d,", nextTerm);
    t1 = t2;
    t2 = nextTerm;
    nextTerm = t1 + t2;
  }
  return nextTerm;
  }
int main(int argc, char **argv)
{
  if (argc != 2)
  {
    fprintf(stderr, "(wrong usage )input limit");
    return 0;
  }
  int n= atoi(argv[1]);
  fib(n);
  return 0;
}
