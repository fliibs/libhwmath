#include "tester.h"
bool Tester::run(){
////////////////
// start test
////////////////
    bool fail=0;
    std::array<int,5>status1;
    std::array<int,5>status2;

    for(int i=0;i<test_times;i++){
        if(c_debug){
            printf("into c_debug\n");
            in_a.sign=1;
            in_a.expo=11;
            in_a.mant=369;
            in_b.sign=0;
            in_b.expo=4;
            in_b.mant=237;
            in_c.sign=0;
            in_c.expo=0;
            in_c.mant=0;
        }
        else{
            stimu1.stimus_func(&in_a);
            stimu2.stimus_func(&in_b);
            stimu3.stimus_func(&in_c);
        }
        status1={0,0,0,0,0};
        status2={0,0,0,0,0};
        status1=checker_1_func(in_a,in_b,in_c,rnd_mode,&out_1);
        status2=checker_2_func(in_a,in_b,in_c,rnd_mode,&out_2);
        fail   =checker1.checker_func(out_1,out_2,status1,status2);
        if(fail){
            if(vari_debug){
                checker1.compareVariables(my_rtl1.pipe1.VariablesTable,my_cmodel1.c_vari_table);
            }
            printf("in_a:\n");
            in_a.print();
            printf("in_b:\n");
            in_b.print();
            if(arg1.op=="fma"){
                printf("in_c:\n");
                in_c.print();
            }
            printf("output one is ---------------\n");
            out_1.print();
            printf("----------------------------\n");
            printf("output two is ---------------\n");  
            out_2.print();
            printf("----------------------------\n");
            break;
        }
    }
    return fail;
}

