#include"maxargs.h"

void maxargs(A_stm stm){
    switch (stm->kind)
    {
    case A_compoundStm:
        maxargs(stm->u.compound.stm1);
        maxargs(stm->u.compound.stm2);
        break;
    
    case A_assignStm:
        maxargs_exp(stm->u.assign.exp);
        break;
    
    case A_printStm:
        count_exp(stm->u.print.exps);
        maxargs_expList(stm->u.print.exps);
        break;

    default:
        assert("kind value error for A_stm!");
        break;
    }
}

void maxargs_exp(A_exp exp){
    switch (exp->kind)
    {
    case A_idExp:
        break;
    
    case A_numExp:  
        break;
    
    case A_opExp:
        maxargs_exp(exp->u.op.left);
        maxargs_exp(exp->u.op.right);  
        break;
    
    case A_eseqExp:
        maxargs(exp->u.eseq.stm);
        maxargs_exp(exp->u.eseq.exp);  
        break;
    
    default:
        assert("kind value error for A_exp!");
        break;
    }
}

void maxargs_expList(A_expList expList){
    switch (expList->kind)
    {
    case A_pairExpList:
        maxargs_exp(expList->u.pair.head);
        maxargs_expList(expList->u.pair.tail);
        break;
    
    case A_lastExpList:
        maxargs_exp(expList->u.last);
        break;
    
    default:
        assert("kind value error for A_expList!");
        break;
    }
}

void count_exp(A_expList expList){
    static int NO = 0;
    int count = 1;
    NO++;
    while(expList->kind != A_lastExpList){
        count++;
        expList = expList->u.pair.tail;
    }
    printf("The %d print-exp has %d parameters\n", NO, count);
}