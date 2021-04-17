#include"interp.h"

Table_ Table(string id, int value, Table_ tail){
    Table_ t = malloc(sizeof(*t));
    t->id = id;
    t->value = value;
    t->tail = tail;
    return t;
}

IntAndTable_ IntAndTable(int i, Table_ t){
    IntAndTable_ it = checked_malloc(sizeof(*it));
    it->i = i;
    it->t = t;
    return it;
}

Table_ interpStm(A_stm s, Table_ t){
    IntAndTable_ it;
    switch (s->kind)
    {
    case A_compoundStm:
        t = interpStm(s->u.compound.stm1, t);
        t = interpStm(s->u.compound.stm2, t);
        return t;
        break;
    
    case A_assignStm:
        it = interpExp(s->u.assign.exp, t);
        t = update(it->t, s->u.assign.id, it->i);
        return t;
        break;
    
    case A_printStm:
        it = interpExpList(s->u.print.exps, t);
        return it->t;
        break;
    
    default:
        assert("kind value error for A_stm!");
        break;
    }
}

IntAndTable_ interpExp(A_exp e, Table_ t){
    switch (e->kind)
    {
    case A_idExp:
        return IntAndTable(lookup(t, e->u.id), t);
        break;

    case A_numExp:
        return IntAndTable(e->u.num, t);
        break;
    
    case A_opExp:
        {
            int left_val, right_val;
            IntAndTable_ it_tmp = interpExp(e->u.op.left, t);
            left_val = it_tmp->i;
            it_tmp = interpExp(e->u.op.right, it_tmp->t);
            right_val = it_tmp->i;
            
            int value;
            switch (e->u.op.oper)
            {
            case A_plus:
                value = left_val + right_val;
                break;
            
            case A_minus:
                value = left_val - right_val;
                break;
            
            case A_times:
                value = left_val * right_val;
                break;
            
            case A_div:
                value = left_val / right_val;
                break;
            
            default:
                assert("kind value error for oper!");
                break;
            }
            return IntAndTable(value, it_tmp->t);
            break;
        }

    case A_eseqExp:
        t = interpStm(e->u.eseq.stm, t);
        return interpExp(e->u.eseq.exp, t);
        break;

    default:
        assert("kind value error for A_exp!");
        break;
    }
}

IntAndTable_ interpExpList(A_expList expList, Table_ t){
    IntAndTable_ it;
    switch (expList->kind)
    {
    case A_pairExpList:
        it = interpExp(expList->u.pair.head, t);
        printf("%d ", it->i);
        return interpExpList(expList->u.pair.tail, it->t);
        break;
    
    case A_lastExpList:
        it = interpExp(expList->u.last, t);
        printf("%d\n", it->i);
        return it;
        break;

    default:
        assert("kind value error for A_expList!");
        break;
    }
}

int lookup(Table_ t, string key){
    int res = 0;
    while(t != NULL){
        if(t->id == key){
            res = t->value;
            break;
        }
        t = t->tail;
    }
    return res;
}

Table_ update(Table_ t, string id, int value){
    return Table(id, value, t);
}