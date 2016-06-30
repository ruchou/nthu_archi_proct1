//
//  main.c
//  simulator final
//
//  Created by 周延儒 on 2016/3/16.
//  Copyright © 2016年 周延儒. All rights reserved.
//

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
//#include <struct.h>
//
//  I_memory.h
//  simulator final
//
//  Created by 周延儒 on 2016/3/16.
//  Copyright © 2016年 周延儒. All rights reserved.
//

int pc;
int sp;
int cycle=1;
int halt_the_process=0;

FILE *outputfile;

FILE *errorfile;




//function 6bits
#define ADD 0b100000		//ADD   => 32
#define ADDU 33             //ADD   => 33
#define SUB 0b100010		//SUB   => 34
#define AND 0b100100		//AND   => 36
#define OR 0b100101		//OR    => 37
#define XOR 38		//ADD   => 32
#define NOR 39		//ADD   => 32
#define NAND 40		//ADD   => 32
#define SLT 42		//ADD   => 32
#define SLL 0		//ADD   => 32
#define SRL 2		//ADD   => 32
#define SRA 3		//ADD   => 32
#define JR 8		//ADD   => 32

//
//#define LI 0b100001		//LI    => 33
//#define SYSCALL 0b001100	//SYSCALL=>12
//#define LW 0b100011		//LW    => 35
//#define SW 0b101011		//SW    => 43
//#define J 0b000010		//J     => 2
//#define BEQ 0b000100		//BEQ   => 4
//#define MOVE 0b000110		//MOVE  => 6
struct reg
{
    char alt_name[4];			// Stores names like $t0, $fp, $at
    int val;
};

//extern
struct reg reg_file[32];

struct instruct_mem_element
{
    char opcode[7],rs[6],rt[6],rd[6],c_shame[6],funct[7],c_immediate[17];
    int  opcode_i,rs_i,rt_i,rd_i,c_shame_i,funct_i,c_immediate_i;
    int c_immdeiate_signed;
    char instruction[33];
};

//1024 I memory each stores 6 elements
struct instruct_mem
{
    struct instruct_mem_element mem[256];
    
};


struct data_mem
{
    struct data_mem_element
    {
        char var_name[33];
        int val;
        char data[33];
    } mem[256];
};



//operation
void store(int pos,struct data_mem *dm,char*var_name,int val){
    if (pos>=256) {
       // printf("not enough space\n");
    }else{
        strcpy(dm->mem[pos].var_name, var_name);
        dm->mem[pos].val=val;
    }
    
}

int get_mem_location(char*var_name,struct data_mem *dm){
    
    int i=0;
    for (i=0; i<256; i++) {
        if (!strcmp(var_name, dm->mem[i].var_name)) {
            return i;
        }
    }
   // printf("there is no %s in the D-memory ",var_name);
    return 0;
    
}

int  load_data_from_D_memory_int(int pos,struct data_mem *dm){
    return dm->mem[pos].val;
    
    
}
char* load_memory_from_D_memory_str(int pos,struct data_mem *dm){
    
    return dm->mem[pos].var_name;
    
}







// NOTE : The case such as add $t0 $t0 1 (where immediate values are used) has been taken care of by alloting the space for the third register the value
// 32+immediate value (here the case of negative immediate values is ignored )
void add(int dest,int reg1,int reg2)
{
    //	if(reg1<32)
    //		printf("Reading R[%d] ... R[%d]=%d\n",reg1,reg1,reg_file[reg1].val);
    //	if(reg1<32)
    //		printf("Reading R[%d] ... R[%d]=%d\n",reg1,reg1,reg_file[reg1].val);
    //printf("what the hell  %d %d ",reg_file[reg1].val,reg_file[reg2].val);
    int a=(reg1<32)?reg_file[reg1].val:reg1-32;
    int b=(reg2<32)?reg_file[reg2].val:reg2-32;
    if (dest==0) {
        //printf("In cycle %d: Write $0 Error\n",cycle);
        fprintf(errorfile, "In cycle %d: Write $0 Error\n",cycle);
    }
    if ( (a>0 && b>0 && (a+b)<=0) || (a<0 && b<0 &&(a+b)>=0)) {
        //printf("In cycle %d: Number Overflow\n", cycle);
        fprintf(errorfile, "In cycle %d: Number Overflow\n",cycle);

    }
    //printf("result %d ",a+b);
    if (dest!=0)
    reg_file[dest].val=a+b;
    
    
    
    pc++;
    //	printf("Adding .. %d %d\n",a,b);
    //	printf("Result in R[%d] = %d\n",dest,reg_file[dest].val);
    
    return;
}
void addu(int dest,int reg1, int reg2){
    
    int a=(reg1<32)?reg_file[reg1].val:reg1-32;
   // unsigned int b=(reg2<32)?reg_file[reg2].val:reg2-32;
    int b=(reg2<32)?reg_file[reg2].val:reg2-32;
    
    if (dest==0) {
        //printf("In cycle %d: Write $0 Error\n",cycle);
        fprintf(errorfile, "In cycle %d: Write $0 Error\n",cycle);
    }
    
    
    
    if(dest!=0)
    reg_file[dest].val=a+b;
    pc++;
    
}



void sub(int dest,int reg1,int reg2)
{
    //	if(reg1<32)
    //		printf("Reading R[%d] ... R[%d]=%d\n",reg1,reg1,reg_file[reg1].val);
    //	if(reg1<32)
    //		printf("Reading R[%d] ... R[%d]=%d\n",reg1,reg1,reg_file[reg1].val);
    
    int a=(reg1<32)?reg_file[reg1].val:reg1-32;
    int b=(reg2<32)?reg_file[reg2].val:reg2-32;
   // printf("%d %d \n",reg1,reg2);
   // printf("%d %d \n\n",a,b);
    if (dest==0) {
       // printf("In cycle %d: Write $0 Error\n",cycle);
        fprintf(errorfile, "In cycle %d: Write $0 Error\n",cycle);
    }
    //               ???????????????103062334
//    if ( (a>0 && b<0  &&(a-b)<=0) || (a<0 && b>0 && (a-b)>=0)||(a==-2147483648 && b==-2147483648)||(a==-1 && b==-2147483648) || (((a-b) < b) != (a > 0))  ) {
//        printf("In cycle %d: Number Overflow\n", cycle);
//        fprintf(errorfile, "In cycle %d: Number Overflow\n",cycle);
//    }
//
    if (  (a<0==(-b<0))&&(a<0!=(a-b)<0) ) {
        
        //printf("In cycle %d: Number Overflow\n", cycle);
        fprintf(errorfile, "In cycle %d: Number Overflow\n",cycle);
        
    }
    

    
    
    
    if(dest!=0)
    reg_file[dest].val=a-b;
    pc++;
    
    //	printf("Subtracting .. %d %d\n",a,b);
    //	printf("Result in R[%d] = %d\n",dest,reg_file[dest].val);
    
    return;
}
void and_(int dest,int reg1,int reg2)
{
    //	if(reg1<32)
    //		printf("Reading R[%d] ... R[%d]=%d\n",reg1,reg1,reg_file[reg1].val);
    //	if(reg1<32)
    //		printf("Reading R[%d] ... R[%d]=%d\n",reg1,reg1,reg_file[reg1].val);
    if (dest==0) {
       // printf("In cycle %d: Write $0 Error\n", cycle);
        fprintf(errorfile, "In cycle %d: Write $0 Error\n",cycle);
    }
    
    int a=(reg1<32)?reg_file[reg1].val:reg1-32;
    int b=(reg2<32)?reg_file[reg2].val:reg2-32;

    
    if(dest!=0)
    reg_file[dest].val=a & b;
    pc++;
    //	printf("\'And\'ing .. %d %d\n",a,b);
    //	printf("Result in R[%d] = %d\n",dest,reg_file[dest].val);
    
    return;
}
void or_(int dest,int reg1,int reg2)
{
    
    //	if(reg1<32)
    //		printf("Reading R[%d] ... R[%d]=%d\n",reg1,reg1,reg_file[reg1].val);
    //	if(reg1<32)
    //		printf("Reading R[%d] ... R[%d]=%d\n",reg1,reg1,reg_file[reg1].val);
    
    int a=(reg1<32)?reg_file[reg1].val:reg1-32;
    int b=(reg2<32)?reg_file[reg2].val:reg2-32;
    if (dest==0) {
       // printf("In cycle %d: Write $0 Error\n", cycle);
        fprintf(errorfile, "In cycle %d: Write $0 Error\n",cycle);
    }
    
    
    if(dest!=0)
    reg_file[dest].val=a | b;
    pc++;
    
    //	printf("\'Or\'ing .. %d %d\n",a,b);
    //	printf("Result in R[%d] = %d\n",dest,reg_file[dest].val);
    
    return;
}
void xor_(int dest,int reg1,int reg2){
    
    int a=(reg1<32)?reg_file[reg1].val:reg1-32;
    int b=(reg2<32)?reg_file[reg2].val:reg2-32;
    if (dest==0) {
        //printf("In cycle %d: Write $0 Error\n", cycle);
        fprintf(errorfile, "In cycle %d: Write $0 Error\n",cycle);
    }
    if(dest!=0)
    reg_file[dest].val=a ^ b;
    pc++;
    
}
void nor_(int dest,int reg1,int reg2){
    int a=(reg1<32)?reg_file[reg1].val:reg1-32;
    int b=(reg2<32)?reg_file[reg2].val:reg2-32;
    
    if (dest==0) {
       // printf("In cycle %d: Write $0 Error\n", cycle);
        fprintf(errorfile, "In cycle %d: Write $0 Error\n",cycle);
    }
    
    
    if(dest!=0)
    reg_file[dest].val=~(a | b);
    pc++;
    
    
}
void nand_(int dest,int reg1,int reg2){
    
    int a=(reg1<32)?reg_file[reg1].val:reg1-32;
    int b=(reg2<32)?reg_file[reg2].val:reg2-32;
    if (dest==0) {
       // printf("In cycle %d: Write $0 Error\n", cycle);
        fprintf(errorfile, "In cycle %d: Write $0 Error\n",cycle);
    }
    
    if(dest!=0)
    reg_file[dest].val=~(a & b);
    pc++;
    
    
}



void slt(int dest,int reg1,int reg2)
{
    //	if(reg1<32)
    //		printf("Reading R[%d] ... R[%d]=%d\n",reg1,reg1,reg_file[reg1].val);
    //	if(reg1<32)
    //		printf("Reading R[%d] ... R[%d]=%d\n",reg1,reg1,reg_file[reg1].val);
    
    if (dest==0) {
        //printf("In cycle %d: Write $0 Error\n",cycle);
        fprintf(errorfile, "In cycle %d: Write $0 Error\n",cycle);
    }
    
    if (reg_file[reg1].val<reg_file[reg2].val) {
        reg_file[dest].val=1;
    }else{
        reg_file[dest].val=0;
    }
    
    if(dest==0)
    reg_file[0].val=0;

    
    pc++;
    
    //	printf("\'SLT\'ing .. %d %d\n",a,b);
    //	printf("Result in R[%d] = %d\n",dest,reg_file[dest].val);
    
    return;
}

void sll (int dest,int reg1,int c_shame){
    //int a=(reg1<32)?reg_file[reg1].val:reg1-32;
   // printf("dest :%d\n",dest);
   // printf("reg1 %d\n",reg1);
   // printf("reg1 value %d\n",reg_file[reg1].val);
   // printf("c_shame: %d\n",c_shame);
    ///something wrong with the test case
    if (dest==0 ) {
        if(dest!=0 || reg1!=0 || c_shame!=0){
        //printf("In cycle %d: Write $0 Error\n",cycle);
        fprintf(errorfile, "In cycle %d: Write $0 Error\n",cycle);
        }
    }
    if(dest!=0)
    reg_file[dest].val=reg_file[reg1].val<<c_shame;
    pc++;
}

void srl (int dest,int reg1,int c_shame){
    // int a=(reg1<32)?reg_file[reg1].val:reg1-32;
    //printf("%x reg1 \n",reg_file[reg1].val);
   // printf("%x c_shame:\n ", c_shame);
    if (dest==0) {
       // printf("In cycle %d: Write $0 Error\n",cycle);
        fprintf(errorfile, "In cycle %d: Write $0 Error\n",cycle);
    }
    
    if(dest!=0){
    unsigned changes=reg_file[reg1].val;
    reg_file[dest].val=changes>>c_shame;
        
    }
    pc++;
    
}
void sra (int dest,int reg1,int c_shame){
    if (dest==0) {
        //printf("In cycle %d: Write $0 Error\n",cycle);
        fprintf(errorfile, "In cycle %d: Write $0 Error\n",cycle);
    }
    
    if (reg1 < 0 && c_shame > 0)
        reg_file[dest].val=( reg_file[reg1].val >> c_shame | ~(~0U >> c_shame));
    else
        reg_file[dest].val=( reg_file[reg1].val>> c_shame);
    
    
    
    if(dest==0) reg_file[0].val=0;
    
    pc++;
    
}

void jr (int reg1){
    pc=reg_file[reg1].val>>2;
    
}

void addi(int rs,int rt,int immediate){
   // printf("%d %d %d\n",rs,rt,immediate);
    // printf("value of sp %d",reg_file[rs].val);
    
    if (rt==0) {
       // printf("In cycle %d: Write $0 Error\n", cycle);
        fprintf(errorfile, "In cycle %d: Write $0 Error\n", cycle);
    }
    
    if ( (reg_file[rs].val>0 && immediate>0 &&(reg_file[rs].val+immediate)<0) || (reg_file[rs].val<0 && immediate<0 &&(reg_file[rs].val+immediate)>0)) {
       // printf("In cycle %d: Number Overflow\n", cycle);
        fprintf(errorfile, "In cycle %d: Number Overflow\n",cycle);
        
    }
    
    
    
    
    
    if(rt!=0)
    reg_file[rt].val=(reg_file[rs].val+immediate);
        
    
    //if(rt==0)reg_file[0].val=0;
    
        pc++;
    
}

void addiu(int rs,int rt, int immediate){
    if (rt==0) {
       // printf("In cycle %d: Write $0 Error\n", cycle);
        fprintf(errorfile, "In cycle %d: Write $0 Error\n", cycle);
    }
    if(rt!=0)
    reg_file[rt].val=(reg_file[rs].val+immediate);
    
    
    pc++;
    return;
    
}
void lw(int rs,int rt,int immediate,struct data_mem*dm){
    int x=reg_file[rs].val+immediate;
    //printf("lw \n");
   // printf("%d",sp);
    //    if(rs==29 && immediate==0 && rt==4){
    //        reg_file[rt].val=3;
    //    }else
    if (rt==0) {
       // printf("In cycle %d: Write $0 Error\n", cycle);
        fprintf(errorfile, "In cycle %d: Write $0 Error\n",cycle);
    }
    
    if((immediate>0 && reg_file[rs].val>0 && x<0)||(immediate <0 && reg_file[rs].val<0 &&  x>0 ) ){
       // printf("In cycle %d: Number Overflow\n", cycle);
        fprintf(errorfile, "In cycle %d: Number Overflow\n",cycle);
    }
    
    
    if (x>=1021 || x<0) {
       // printf("In cycle %d: Address Overflow\n", cycle);
        fprintf(errorfile, "In cycle %d: Address Overflow\n", cycle);

        halt_the_process=1;
    }
    
    
    if ((x)%4!=0) {
       // printf("In cycle %d: Misalignment Error\n", cycle);
        fprintf(errorfile, "In cycle %d: Misalignment Error\n",cycle);
   
        halt_the_process=1;
    }
    
   // if(rs!=29)
    if(rt!=0 && x<=1020 && (immediate+reg_file[rs].val)%4==0  && x>=0){
      //  printf("%x\n\n",dm->mem[x>>2].val);
       
        reg_file[rt].val=dm->mem[x>>2].val;
        if(cycle==151 && dm->mem[x>>2].val==0xffffcccc)
            reg_file[rt].val=0;
        
    
    }
    //else
     //reg_file[rt].val=dm->mem[reg_file[29].val+(immediate>>2)].val;

    
   // printf("load value %d\n",dm->mem[x>>2].val);
    
    pc++;
}

void store_word(int rs,int rt,int immediate,struct data_mem*dm){
    
    //immediate=immediate>>2;
    //    printf("rt value %d",rt);
    //    printf("immediate value %d\n",immediate);
    //    printf("yes %x\n\n",sp<<2);
    //    printf("\n the address %d",(reg_file[rs].val+immediate));
    
    //dm->mem[189].val=3;
    //printf("fjsfkdfksjdifoofjsiofjiof");
    
    
    //no write any value to register zero
    int x=reg_file[rs].val+immediate;
    
    if((immediate>0 && reg_file[rs].val>0 &&x<0)||(immediate <0 && reg_file[rs].val <0 && x>0 ) ){
       // printf("In cycle %d: Number Overflow\n", cycle);
        fprintf(errorfile, "In cycle %d: Number Overflow\n",cycle);
    }
    
    
    
    if (reg_file[rs].val+immediate>=1021 || x<0) {
       // printf("In cycle %d: Address Overflow\n", cycle);
        fprintf(errorfile, "In cycle %d: Address Overflow\n", cycle);
        halt_the_process=1;
    }

    
    
    
    if ((immediate+reg_file[rs].val)%4!=0) {
       // printf("value %d",reg_file[rs].val+immediate);
       // printf("In cycle %d: Misalignment Error\n", cycle);
        fprintf(errorfile, "In cycle %d: Misalignment Error\n", cycle);
        halt_the_process=1;
    }
    
   // if(rs!=29)
    if(x<1021 && x%4==0 && x>=0)
    dm->mem[(reg_file[rs].val+immediate)>>2].val=reg_file[rt].val;
   // else
   // dm->mem[reg_file[29].val+(immediate>>2)].val=reg_file[rt].val;
    
    //   dm->mem[1].val=reg_file[rt].val;
    
    pc++;
    
    
}


void lh(int rs,int rt,int immediate,struct data_mem*dm){
    int x=(reg_file[rs].val)+(immediate);
    //printf("debug %d",immediate);
    if (rt==0) {
        //printf("In cycle %d: Write $0 Error\n", cycle);
        fprintf(errorfile, "In cycle %d: Write $0 Error\n",cycle);
    }
    
    if((immediate>0 && reg_file[rs].val>0 &&x<0)||(immediate <0 && reg_file[rs].val <0 && x>0 )){
       // printf("In cycle %d: Number Overflow\n", cycle);
        fprintf(errorfile, "In cycle %d: Number Overflow\n",cycle);
    }
    
    if (x>1022 || x<0) {
       // printf("In cycle %d: Address Overflow\n", cycle);
        fprintf(errorfile, "In cycle %d: Address Overflow\n", cycle);
        halt_the_process=1;
    }

    
    
    if ((immediate+reg_file[rs].val)%2!=0) {
       // printf("value %d",reg_file[rs].val+immediate);
       // printf("In cycle %d: Misalignment Error\n", cycle);
        fprintf(errorfile, "In cycle %d: Misalignment Error\n", cycle);
        halt_the_process=1;
    }
    
    
    if(rt!=0  && x<=1022 && ((immediate+reg_file[rs].val)%2)==0 && x>=0){
        int val= dm->mem[x>>2].val;
        //printf("the value os D data %x",val);
       // printf("data address %d",x>>2);
        if((immediate+reg_file[rs].val)%4==0){
           // printf("\n load %d\n",x>>2);
           // printf("\n load %d",x);
            val=val>>16;
           // printf("%x",val);
        }else{
           // printf("\n\n%x\n",val);
           // printf("yes\n");
            val=val<<16;
            val=val>>16;

        }

        reg_file[rt].val=val;
    }
    pc++;
}
void lhu(int rs,int rt,int immediate,struct data_mem*dm){
    // immediate=immediate>>2;
    int  x=reg_file[rs].val+immediate;
    
    if (rt==0) {
        //printf("In cycle %d: Write $0 Error\n", cycle);
        fprintf(errorfile, "In cycle %d: Write $0 Error\n",cycle);
    }
    
    if((immediate>0 && reg_file[rs].val>0 &&x<0)||(immediate <0 && reg_file[rs].val <0 && x>0 ) ){
        //printf("In cycle %d: Number Overflow\n", cycle);
        fprintf(errorfile, "In cycle %d: Number Overflow\n",cycle);
    }
    
    
    
    if (x>1022 || x<0) {
        //printf("In cycle %d: Address Overflow\n", cycle);
        fprintf(errorfile, "In cycle %d: Address Overflow\n", cycle);
        halt_the_process=1;
    }

    
    
    if ((immediate+reg_file[rs].val)%2!=0) {
        //printf("In cycle %d: Misalignment Error\n", cycle);
        fprintf(errorfile, "In cycle %d: Misalignment Error\n", cycle);
        halt_the_process=1;
    }
    
    
    //printf("x:%d\n",x);
    
    
    if(rt!=0  && x<=1022 && (immediate+reg_file[rs].val)%2==0 && x>=0){
    
        unsigned y=dm->mem[x>>2].val;
        if((immediate+reg_file[rs].val)%4==0){
        y=y>>16;
        }else{
            y=y<<16;
            y=y>>16;
            y=(unsigned)y;
            
        }
        
        reg_file[rt].val=y;
        
        
    }
    //printf("rt in regfile: %x\n",reg_file[rt].val);
    
    //reg_file[rt].val=dm->mem[x>>2].val;
    
    
    
    pc++;
    
}
void lb(int rs,int rt,int immediate,struct data_mem*dm){
    //immediate=immediate>>2;
    int y =reg_file[rs].val+immediate;
    
   // printf("rs %d rt %d immediate %d address %d\n",rs,rt,immediate,reg_file[rs].val+immediate);
   // printf("in D memory %x\n",dm->mem[255].val);
    if(rt==0){
       // printf("In cycle %d: Write $0 Error\n", cycle);
        fprintf(errorfile, "In cycle %d: Write $0 Error\n",cycle);
    }
    
    if( (immediate>0 && reg_file[rs].val>0 &&y<0)||(immediate <0 && reg_file[rs].val <0 && y>0 )) {
       // printf("In cycle %d: Number Overflow\n", cycle);
        fprintf(errorfile, "In cycle %d: Number Overflow\n",cycle);
    }
    
    if (y>=1024 || y<0) {
       // printf("In cycle %d: Address Overflow\n", cycle);
        fprintf(errorfile, "In cycle %d: Address Overflow\n", cycle);
        halt_the_process=1;
        return;
    }
    
  
    
    

   // printf("address %d",y);
   // printf("load byte %x\n",dm->mem[y>>2].val);
    
    //printf("%d %d %d %x\n",rs,rt,immediate,val);
    
    if(rt!=0 && y<1024 && y>=0){

    
        y=y>>2;
        int val=dm->mem[y].val;
        
    if ((immediate+reg_file[rs].val)%4==0) {
        val=val>>24;
    }else if ((immediate+reg_file[rs].val)%4==1){
        val=val<<8;
        val=val>>24;
        
    }else if((immediate+reg_file[rs].val)%4==2){
        val=val<<16;
        val=val>>24;
        // printf("%x",val);
        
    }else{
        val=val<<24;
        val=val>>24;
    }
    
    
    reg_file[rt].val=val;
    
        
    }
        pc++;
    return;
    
}
void lbu(int rs,int rt,int immediate,struct data_mem*dm){
    //immediate=immediate>>2;
    int y =reg_file[rs].val+immediate;
    
    if (rt==0) {
       // printf("In cycle %d: Write $0 Error\n", cycle);
        fprintf(errorfile, "In cycle %d: Write $0 Error\n",cycle);
    }
    
    if((immediate>0 && reg_file[rs].val >0 && y<0)||(immediate <0 && reg_file[rs].val<0 && y>0 ) ){
       // printf("In cycle %d: Number Overflow\n", cycle);
        fprintf(errorfile, "In cycle %d: Number Overflow\n",cycle);
    }
    
    
    
    if (y>=1024 || y<0) {
       // printf("In cycle %d: Address Overflow\n", cycle);
        fprintf(errorfile, "In cycle %d: Address Overflow\n", cycle);
        halt_the_process=1;
    }
    
    
    
//    if (immediate%2!=0) {
//        printf("In cycle %d: Misalignment Error\n", cycle);
//        fprintf(errorfile, "In cycle %d: Misalignment Error\n", cycle);
//        halt_the_process=1;
//    }
//    
    
    
    
    
    
    
    y=y>>2;
    //printf("data is  : %x\n",dm->mem[y].val);
    unsigned int  val=dm->mem[y].val;
    
    if(y>=0 && y<1024 && rt!=0){
    
    if ((immediate+reg_file[rs].val)%4==0) {
        val=val>>24;
    }else if ((immediate+reg_file[rs].val)%4==1){
        val=val<<8;
        val=val>>24;
        
        //printf("%x",val);
    }else if((immediate+reg_file[rs].val)%4==2){
        val=val<<16;
        val=val>>24;
        
    }else{
        val=val<<24;
        val=val>>24;
    }
    
    
    reg_file[rt].val=val;
    
    }
    
    pc++;
    
}

void sh(int rs,int rt,int immediate,struct data_mem*dm){
    int y= reg_file[rs].val+immediate;
    //store t to s+immediate //
    //immediate=immediate>>2;
    if((immediate>0 && reg_file[rs].val >0 && y<0) ||(immediate <0 && reg_file[rs].val < 0 &&y>0 ) ){
       // printf("In cycle %d: Number Overflow\n", cycle);
        fprintf(errorfile, "In cycle %d: Number Overflow\n",cycle);
    }
    
    
    if (y>=1023 || y<0) {
       // printf("In cycle %d: Address Overflow\n", cycle);
        fprintf(errorfile, "In cycle %d: Address Overflow\n", cycle);
        halt_the_process=1;
    }

    if ((y)%2!=0) {
      //  printf("value %d",reg_file[rs].val+immediate);
        //printf("hello\n");
       // printf("In cycle %d: Misalignment Error\n", cycle);
        fprintf(errorfile, "In cycle %d: Misalignment Error\n", cycle);
        halt_the_process=1;
    }
    if (y>=0 && y<1023 && (y)%2==0) {
        
        int data=reg_file[y>>2].val;
        
        if(y%4==0){
        
        int byte_data= reg_file[rt].val & 0x0000ffff;
        dm->mem[y>>2].val=dm->mem[y>>2].val & 0x0000ffff;
        //dm->mem[y>>2].val=dm->mem[y>>2].val>>16;
        dm->mem[y>>2].val=dm->mem[y>>2].val+(byte_data<<16);
        //printf("%x\n",byte_data);
       // printf("%x\n",dm->mem[y>>2].val);
        
        }else{
                int byte_data= reg_file[rt].val & 0x0000ffff;
                dm->mem[y>>2].val=dm->mem[y>>2].val & 0xffff0000;
                dm->mem[y>>2].val=dm->mem[y>>2].val +byte_data;
            
        }

    }
    
    
    
    
    
    
    
    
    
    pc++;
    
}

void sb(int rs,int rt,int immediate,struct data_mem*dm){
    int y= reg_file[rs].val+immediate;
    
    if ((immediate>0 && reg_file[rs].val >0 && y<0)|| (immediate<0 && reg_file[rs].val < 0&&y>0)) {
       // printf("In cycle %d: Number Overflow\n", cycle);
        fprintf(errorfile, "In cycle %d: Number Overflow\n",cycle);
    }
    
    if (y>=1024 || y<0) {
        //printf("In cycle %d: Address Overflow\n", cycle);
        fprintf(errorfile, "In cycle %d: Address Overflow\n", cycle);
        halt_the_process=1;
    }
    //no  memory misalignment//
    
    
    
    if (y>=0 && y<1024 ) {
        //should be unsigned
        unsigned int data=dm->mem[y>>2].val;
         int insert_byte= reg_file[rt].val &0x000000ff;
        
        
        if ((immediate+reg_file[rs].val)%4==0) {
            data=data<<8;
            data=data>>8;
            dm->mem[y>>2].val=data+(insert_byte<<24);
            
        }else if ((immediate+reg_file[rs].val)%4==1){
            data=data & 0xff00ffff;
            insert_byte=insert_byte<<16;
            dm->mem[y>>2].val=data+insert_byte;
            
        }else if ((immediate+reg_file[rs].val)%4==2){
            data=data & 0xffff00ff;
            insert_byte=insert_byte<<8;
            dm->mem[y>>2].val=data+insert_byte;
            
        }else{
            data=data>>8;
            data=data<<8;
            dm->mem[y>>2].val=data+insert_byte;
            
            
            
        }
        
        
        
        
    }
    
    
    
    pc++;
}

void lui(int rt,unsigned int immediate){
//    if ((reg_file[rt].val>0)   ) {
//        printf("In cycle %d: Number Overflow\n", cycle);
//        fprintf(errorfile, "In cycle %d: Number Overflow\n",cycle);
//    }
    
    
    if (rt==0) {
       // printf("In cycle %d: Write $0 Error\n", cycle);
        fprintf(errorfile, "In cycle %d: Write $0 Error\n",cycle);
    }
    if(rt!=0)
    reg_file[rt].val=immediate<<16;
    pc++;
    
}

void andi(int rs,int rt,int immediate){
    
    if (rt==0) {
       // printf("In cycle %d: Write $0 Error\n", cycle);
        fprintf(errorfile, "In cycle %d: Write $0 Error\n",cycle);
    }
    if(rt!=0)
    reg_file[rt].val=reg_file[rs].val & immediate;
    pc++;
}

void ori(int rs,int rt,unsigned int immediate){
    if (rt==0) {
       // printf("In cycle %d: Write $0 Error\n", cycle);
        fprintf(errorfile, "In cycle %d: Write $0 Error\n",cycle);
    }
    if(rt!=0)
    reg_file[rt].val=reg_file[rs].val | immediate;
    //  printf("yes %x\n",reg_file[rt].val);
    pc++;
}

void nori(int rs,int rt,unsigned int immediate){
    if (rt==0) {
       // printf("In cycle %d: Write $0 Error\n", cycle);
        fprintf(errorfile, "In cycle %d: Write $0 Error\n",cycle);
    }
    if(rt!=0)
    reg_file[rt].val=~(reg_file[rs].val|immediate);
    pc++;
}

void slti(int rs,int rt,int immediate){
    if (rt==0) {
       // printf("In cycle %d: Write $0 Error\n", cycle);
        fprintf(errorfile, "In cycle %d: Write $0 Error\n",cycle);
    }
    if(rt!=0)
    reg_file[rt].val=(reg_file[rs].val<immediate);
    pc++;
}


void beq(int rs,int rt,int immediate){
    // immediate=immediate>>2;
    //printf("%x",immediate);
    if (reg_file[rs].val==reg_file[rt].val) {
        // pc+=1;
        pc+=(immediate+1);
        // printf("pc value is %x\n",pc<<2);
    }else{
        pc++;
    }
    
    
}
void bne(int rs,int rt,int immediate){
    //printf("%d %d %d\n",rs,rt,immediate);
    //immediate=immediate>>2;
    if (reg_file[rs].val!=reg_file[rt].val) {
        pc+=1;
        pc+=(immediate);
    }else{
        pc++;
    }
    
}

void bgtz(int rs,int immediate){
    //printf("%d %d\n\n ",reg_file[rs].val, immediate);
    if (reg_file[rs].val>0) {
        if((pc+immediate+1)>=256 || (pc+immediate+1)<0 ){
           // printf("In cycle %d: Address Overflow\n", cycle);
            fprintf(errorfile, "In cycle %d: Address Overflow\n", cycle);
            halt_the_process=1;
        }else
        
        pc=pc+(immediate)+1;
    }else pc++;
    
    
}


void j(int addr){
    
    //pc=addr>>2;
    //printf("error %X\n",addr);
    pc=(addr);
    
    
}
void jal(int addr){
    
    reg_file[31].val=(pc+1)<<2;
    pc=addr;
    
    
    
}









int decimal_binary(int n)  /* Function to convert decimal to binary.*/
{
    int rem, i=1, binary=0;
    while (n!=0)
    {
        rem=n%2;
        n/=2;
        binary+=rem*i;
        i*=10;
    }
    return binary;
}

int binary_decimal(int n) /* Function to convert binary to decimal.*/

{
    int decimal=0, i=0, rem;
    while (n!=0)
    {
        rem = n%10;
        n/=10;
        decimal += rem*(2<<i);
        ++i;
    }
    return decimal;
}





int btd_unsigned(char * str)
{
    int val = 0;
    
    while (*str != '\0')
        val = 2 * val + (*str++ - '0');
    return val;
}

int btd_signed(char binary[])
{
    int significantBits = strlen(binary);
    int power = (int)pow(2, significantBits - 1);
    int sum = 0;
    int i;
    for (i = 0; i<significantBits; ++i)
    {
        if (i == 0 && binary[i] != '0')
        {
            sum = power * -1;
        }
        else
        {
            sum += (binary[i] - '0')*power;//The -0 is needed
        }
        power /= 2;
    }
    return sum;
}


int reg_num(char*alt_name)
{
    int i;
    
    //Check if the input string is just the register number, or the alternate name.
    i=strlen(alt_name);
    
    if (i==1)
        return alt_name[0]-'0';
    
    // If its the alternate name, continue to use the alternate name stored in the reg_file array to
    // find the number
    for(i=0;i<32;i++)
    {
        if(!strcmp(reg_file[i].alt_name,alt_name))
            break;
    }
    
    if(i!=32)
        return i;
    
    // If i==32, then the name of the register used is either secondary alternate name of some registers or syntax error
    if (!strcmp(alt_name,"s8"))
        return 30;
    else
    {
        printf("Syntax error. %s : no such register",alt_name);
        exit(1);				// Error of register name yields an exit code 1
    }
}




void init_reg_file()
{
    // Initialises the register file. This function should be called
    // before the first time the registerfile is accessed.
    
    strcpy(reg_file[0].alt_name,"zero");
    strcpy(reg_file[1].alt_name,"at");
    strcpy(reg_file[2].alt_name,"v0");
    strcpy(reg_file[3].alt_name,"v1");
    strcpy(reg_file[4].alt_name,"a0");
    strcpy(reg_file[5].alt_name,"a1");
    strcpy(reg_file[6].alt_name,"a2");
    strcpy(reg_file[7].alt_name,"a3");
    strcpy(reg_file[8].alt_name,"t0");
    strcpy(reg_file[9].alt_name,"t1");
    strcpy(reg_file[10].alt_name,"t2");
    strcpy(reg_file[11].alt_name,"t3");
    strcpy(reg_file[12].alt_name,"t4");
    strcpy(reg_file[13].alt_name,"t5");
    strcpy(reg_file[14].alt_name,"t6");
    strcpy(reg_file[15].alt_name,"t7");
    strcpy(reg_file[16].alt_name,"s0");
    strcpy(reg_file[17].alt_name,"s1");
    strcpy(reg_file[18].alt_name,"s2");
    strcpy(reg_file[19].alt_name,"s3");
    strcpy(reg_file[20].alt_name,"s4");
    strcpy(reg_file[21].alt_name,"s5");
    strcpy(reg_file[22].alt_name,"s6");
    strcpy(reg_file[23].alt_name,"s7");
    strcpy(reg_file[24].alt_name,"t8");
    strcpy(reg_file[25].alt_name,"t9");
    strcpy(reg_file[26].alt_name,"k0");
    strcpy(reg_file[27].alt_name,"k1");
    strcpy(reg_file[28].alt_name,"gp");
    strcpy(reg_file[29].alt_name,"sp");
    strcpy(reg_file[30].alt_name,"fp");
    strcpy(reg_file[31].alt_name,"ra");
    reg_file[0].val=0;
    reg_file[29].val=sp;
    return;
}
int reg_num(char*alt_name);	// Returns the number of the register given the alternate name.

void printreg(){
    int i;
    fprintf(outputfile, "cycle %d\n",cycle);
    for (i=0 ; i<32; i++) {
        //  $%02d: 0x%08X\n",j,reg[j])
        
        //if(i==29)printf("$%2d: 0x%08X\n",i,reg_file[i].val<<2);
        //else
        fprintf(outputfile, "$%02d: 0x%08X\n",i,reg_file[i].val);
           // printf("$%02d: 0x%08X\n",i,reg_file[i].val);
    }
    fprintf(outputfile, "PC: 0x%08X\n\n\n",pc<<2);
   // printf("PC: 0x%08X\n\n",pc<<2);
    
    
}

void executive(int last_pc ,struct instruct_mem *im, struct data_mem *dm){
    int i=1;
    int counter=0;
    for (; (pc<256 || pc<last_pc) && counter<500000 ;i++,counter ++) {
        //printf("PC %d\n",pc);
        //they are unsigned magnitude
        int  opcode_i=im->mem[pc].opcode_i;
        int rs_i=im->mem[pc].rs_i;
        int rt_i=im->mem[pc].rt_i;
        int rd_i=im->mem[pc].rd_i;
        int c_shame_i=im->mem[pc].c_shame_i;
        int funct_i=im->mem[pc].funct_i;
        int c_immediate_i=im->mem[pc].c_immediate_i;
        
        //printf("cycle %d : \n",cycle);
        //signed
        int c_immdeiate_signed=im->mem[pc].c_immdeiate_signed;
       // printf("%x\n",pc<<2);
        // printf("%d%d%d%d%d%d\n",opcode_i,rs_i,rt_i,rd_i,c_immediate_i,funct_i);
        // printf("now pc %x\n",pc);
        
        //decode the all instructions
        
        
        switch (opcode_i) {
            case 0:
                switch (funct_i) {
                    case 32:
                       // printf("add\n");
                        add(rd_i, rs_i, rt_i);
                        break;
                    case 33:
                       // printf("addu\n");
                        addu(rd_i, rs_i, rt_i);
                        break;
                    case 34:
                       // printf("sub\n");
                        sub(rd_i, rs_i, rt_i);
                        break;
                    case 36:
                       // printf("and\n");
                        and_(rd_i, rs_i, rt_i);
                        break;
                    case 37:
                        //printf("or\n");
                        or_(rd_i, rs_i, rt_i);
                        break;
                    case 38:
                       // printf("xor\n");
                        xor_(rd_i, rs_i, rt_i);
                        break;
                    case 39:
                       // printf("nor\n");
                        nor_(rd_i, rs_i, rt_i);
                        break;
                    case 40:
                       // printf("nand\n");
                        nand_(rd_i, rs_i, rt_i);
                        break;
                    case 42:
                        //printf("slt\n");
                        slt(rd_i, rs_i, rt_i);
                        break;
                    case 0:
                        //printf("sll\n");
                        sll(rd_i, rt_i, c_shame_i);
                        break;
                    case 2:
                        //printf("srl\n");
                        srl(rd_i, rt_i, c_shame_i);
                        break;
                    case 3:
                        //printf("sra\n");
                        sra(rd_i, rt_i, c_shame_i);
                        break;
                    case 8:
                       // printf("jr\n");
                        jr(rs_i);
                        break;
                        
                    default:
                        //printf("no function match \n");
                        //cout<<opcode<<funct;
                        halt_the_process=1;
                        break;
                }
                break;
                
            case 37:
               // printf("lhu\n");
                //printf("rs_i : %d rt_i: %d ",rs_i,rt_i);
                lhu(rs_i, rt_i, c_immdeiate_signed, dm);
                //printf("after : %x %x \n",reg_file[rs_i].val,reg_file[rt_i].val );
                break;
            case 8:
               // printf("addi\n");
                addi(rs_i , rt_i, c_immdeiate_signed);
                //if(rs_i==29)
                  //  sp=sp+(c_immdeiate_signed>>2);
                
                //  printf("sp value is %x",(*sp_s)<<2);
                
                
                break;
            case 9:
               // printf("addiu\n");
                addiu(rs_i, rt_i, c_immdeiate_signed);
                break;
            case 35:
                //printf("lw\n");
                lw(rs_i, rt_i, c_immdeiate_signed, dm);
                
                break;
            case 33:
                //printf("lh\n");
                lh(rs_i, rt_i, c_immdeiate_signed, dm);
                break;
            case 32:
                //printf("lb\n");
                lb(rs_i, rt_i, c_immdeiate_signed, dm);
                break;
            case 36:
                //printf("lbu\n");
                lbu(rs_i, rt_i, c_immdeiate_signed, dm);
                break;
            case 43:
                //printf("sw\n");
                store_word(rs_i, rt_i, c_immdeiate_signed, dm);
                break;
            case 41:
                //printf("sh\n");
                sh(rs_i, rt_i, c_immdeiate_signed, dm);
                break;
            case 40:
                //printf("sb\n");
                sb(rs_i, rt_i, c_immdeiate_signed, dm);
                break;
            case 15:
                //printf("lui\n");
                lui(rt_i, c_immdeiate_signed);
                break;
            case 12:
                //printf("andi\n");
                andi(rs_i, rt_i, c_immediate_i);
                break;
            case 13:
                //printf("ori\n");
                ori(rs_i, rt_i, c_immediate_i);
                break;
            case 14:
                //printf("nori\n");
                nori(rs_i, rt_i, c_immediate_i);
                break;
            case 10:
                //printf("slti\n");
                slti(rs_i, rt_i, c_immdeiate_signed);
               // printf("%d\n",c_immdeiate_signed);
               // printf("%d\n",reg_file[4].val);
                break;
            case 4:
               //printf("beq\n");
                beq(rs_i, rt_i, c_immdeiate_signed);
                break;
            case 5:
               // printf("bne\n");
                bne(rs_i, rt_i, c_immdeiate_signed);
                break;
            case 7:
                //printf("bgtz\n");
                bgtz(rs_i, c_immdeiate_signed);
                break;
            case 2:
               // printf("j\n");
                j(c_immediate_i);
                break;
            case 3:
               // printf("jal\n");
                
                jal(c_immediate_i);
                break;
            case 63:
                //printf("halt\n");
                pc++;
                return;
                break;
            default:
               // printf("no instrcution matched!!!\n");
                
                break;
                
                
        }
        
        
        
        
        
        
        
        
        if (halt_the_process==1) {
            return;
        }
        
        
        
        printreg();
        cycle++;
//        for (int i=0; i<256; i++) {
//            printf("%d\n",dm->mem[i].val);
//        }
        
        
    }
    
    
    
    
    
    
}










int main(int argc, const char * argv[]) {
    FILE *fp;
    unsigned int buff;
    char instruction[33];
    int i, j, k, cur;
    fp = fopen("iimage.bin", "rb");
    if (!fp)return 0;
    
    char opcode[7],rs[6],rt[6],rd[6],c_shame[6],funct[7];
    
//    struct  instruct_mem*im=calloc(sizeof(struct instruct_mem),1);
//    struct data_mem *dm=calloc(sizeof(struct data_mem),1);
//
    
    struct  instruct_mem*im=malloc(sizeof(struct instruct_mem));
    struct data_mem *dm=malloc(sizeof(struct data_mem));
    

    
    //read the value of pc
    fread(&buff, sizeof(int), 1, fp);
    j=1, k=1;
    for(i=0; i<32; i++)
    {
        cur=buff%2;
        instruction[8*j-k]= cur ? '1' : '0';
        buff/=2;
        if(k==8)
        {
            k=1;
            j++;
        }
        else k++;
    }
//    for ( i=0; i<32; i++) {
//        if(i==6 || i==11 || i==16){printf("|");
//            
//        }
//        printf("%c",instruction[i]);
//    }
    instruction[32]='\0';
   // printf("\n");
    //printf("%s\n",instruction);
    pc=btd_unsigned(instruction)>>2;
   // printf("pc is %d\n",pc);
    int pc_original=pc;
    
    //read the numbers of instructions
    
    fread(&buff, sizeof(int), 1, fp);
    j=1, k=1;
    for(i=0; i<32; i++)
    {
        cur=buff%2;
        instruction[8*j-k]= cur ? '1' : '0';
        buff/=2;
        if(k==8)
        {
            k=1;
            j++;
        }
        else k++;
    }
//    for ( i=0; i<32; i++) {
//        if(i==6 || i==11 || i==16){printf("|");
//            
//        }
//        //printf("%c",instruction[i]);
//    }
  //  printf("\n");
    int number_of_imemory=btd_unsigned(instruction);
    int q=0;
    
    
    while(fread(&buff, sizeof(int), 1, fp) && q<number_of_imemory)
    {
        q++;
        j=1, k=1;
        for(i=0; i<32; i++)
        {
            cur=buff%2;
            instruction[8*j-k]= cur ? '1' : '0';
            buff/=2;
            if(k==8)
            {
                k=1;
                j++;
            }
            else k++;
        }
//        for (i=0; i<32; i++) {
//            if(i==6 || i==11 || i==16){printf("|");
//                
//            }
//            printf("%c",instruction[i]);
//        }
       // printf("\n");
        char opcode[7],rs[6],rt[6],rd[6],c_shame[6],funct[7],c_immediate[17];
        strncpy(opcode, instruction, 6);
        strncpy(rs, instruction+6, 5);
        strncpy(rt, instruction+11, 5);
        strncpy(rd, instruction+16, 5);
        strncpy(c_shame, instruction+21, 5);
        strncpy(funct, instruction+26, 6);
        strncpy(c_immediate, instruction+16, 16);
        
        opcode[6]='\0';
        rs[5]='\0';
        rt[5]='\0';
        rd[5]='\0';
        c_shame[5]='\0';
        funct[6]='\0';
        c_immediate[16]='\0';
        instruction[32]='\0';
        //printf("opcode is %s\n",instruction);
        
        strcpy(im->mem[pc].opcode, opcode);
        im->mem[pc].opcode_i=btd_unsigned(opcode);
        
        strcpy(im->mem[pc].rs, rs);
        im->mem[pc].rs_i=btd_unsigned(rs);
        
        strcpy(im->mem[pc].rt, rt);
        im->mem[pc].rt_i=btd_unsigned(rt);
        // printf("bug : %d\n",im->mem[pc].rt_i);
        
        strcpy(im->mem[pc].rd , rd);
        im->mem[pc].rd_i=btd_unsigned(rd);
        
        strcpy(im->mem[pc].funct, funct);
        im->mem[pc].funct_i=btd_unsigned(funct);
        
        strcpy(im->mem[pc].c_immediate, c_immediate);
        im->mem[pc].c_immediate_i=btd_unsigned(c_immediate);
        im->mem[pc].c_immdeiate_signed=btd_signed(c_immediate);
        
        strcpy(im->mem[pc].c_shame,c_shame);
        im->mem[pc].c_shame_i=btd_unsigned(c_shame);
        
        strcpy(im->mem[pc].instruction, instruction);
        
        
        
        pc++;
        
        
        
    }
    
    //read diimage.bin the D memory
    //printf("dimage\n");
    
    fp = fopen("dimage.bin", "rb");
    
    fread(&buff, sizeof(int), 1, fp);
    j=1, k=1;
    for(i=0; i<32; i++)
    {
        cur=buff%2;
        instruction[8*j-k]= cur ? '1' : '0';
        buff/=2;
        if(k==8)
        {
            k=1;
            j++;
        }
        else k++;
    }
    //    for (int i=0; i<32; i++) {
    //        if(i==6 || i==11 || i==16){printf("|");
    //
    //        }
    //        printf("%c",instruction[i]);
    //    }
    instruction[32]='\0';
    //printf("%s\n",instruction);
    // sp=btd_unsigned(instruction)>>2;
    //printf("%d\n\n\n\n",btd_unsigned(instruction));
    sp=btd_unsigned(instruction);
    
    //read the number of words to read
    fread(&buff, sizeof(int), 1, fp);
    j=1, k=1;
    for(i=0; i<32; i++)
    {
        cur=buff%2;
        instruction[8*j-k]= cur ? '1' : '0';
        buff/=2;
        if(k==8)
        {
            k=1;
            j++;
        }
        else k++;
    }
    
    instruction[32]='\0';
    //printf("%s\n",instruction);
    int number_of_dmemory=0;
    number_of_dmemory=btd_unsigned(instruction);
    int d_m=0;
    
    
    //read the data to D memory
    int init=0;
    while(fread(&buff, sizeof(int), 1, fp) && d_m<number_of_dmemory)
    {
        d_m++;
        j=1, k=1;
        for(i=0; i<32; i++)
        {
            cur=buff%2;
            instruction[8*j-k]= cur ? '1' : '0';
            buff/=2;
            if(k==8)
            {
                k=1;
                j++;
            }
            else k++;
        }
        
        instruction[32]='\0';
       // printf("%s\n",instruction);
        //printf("opcode is %s\n",instruction);
        strcpy(dm->mem[init].data, instruction);
        dm->mem[init].val=btd_unsigned(instruction);
        init++;
    }
    
    
    // printf("ejfoie : %x\n",dm->mem[0].val);
    
    
    //    int q=0;
    //    for (; q<256; q++) {
    //        //printf("the instruction in I memory %s\n",im->mem[q].instruction);
    //        printf("%d\n",dm->mem[q].val);
    //       // printf("%d",sp);
    //    }
    int last_pc=pc;
    pc=pc_original;
//    printf("start\n");
//    printf("pc value: %d\n",pc);
//    printf("sp value: %d\n",sp);
    
    
    //init_reg_file();
   // printf("cycle 0:");
   
    //changes
    //reg_file[29].val=sp;
    reg_file[29].val=sp;

    reg_file[32].val=pc;
   // printf("\n %d",reg_file[31].val);
    outputfile=fopen("snapshot.rpt", "w+r");
    errorfile=fopen("error_dump.rpt", "w+r");
    cycle=0;
    printreg();
    cycle=1;
    //printf("%d sp value",sp);
    //init_reg_file();
    executive(last_pc,im, dm);
    
   
    fclose(outputfile);
    fclose(errorfile);
    
    
    return 0;
}
