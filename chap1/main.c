#include"maxargs.h"
#include"interp.h"
#include"prog1.h"

int main(){

    maxargs(prog());
    maxargs(prog_test1());
    maxargs(prog_test2());
    maxargs(prog_test3());
    printf(">>>>>>>>>>>>>>>>>\n");
    interpStm(prog(), NULL);
    interpStm(prog_test1(), NULL);
    interpStm(prog_test2(), NULL);
    interpStm(prog_test3(), NULL);


    return 0;
}