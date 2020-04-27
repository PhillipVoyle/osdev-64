.code64
.extern irq2_handler;
.extern irq3_handler;
.extern irq4_handler;
.extern irq5_handler;
.extern irq6_handler;
.extern irq7_handler;
.extern irq8_handler;
.extern irq9_handler;
.extern irq10_handler;
.extern irq11_handler;
.extern irq12_handler;
.extern irq13_handler;
.extern irq14_handler;
.extern irq15_handler;

.extern int2_handler;
.extern int3_handler;
.extern int4_handler;
.extern int5_handler;
.extern int6_handler;
.extern int7_handler;
.extern int8_handler;
.extern int9_handler;
.extern int10_handler;
.extern int11_handler;
.extern int12_handler;
.extern int13_handler;
.extern int14_handler;
.extern int15_handler;

.macro pushaq
    push %rsp
    push %rax
    push %rcx
    push %rdx
    push %rbx
    push %rbp
    push %rsi
    push %rdi
/*
    push %r8
    push %r9
    push %r10
    push %r11
    push %r12
    push %r13
    push %r14
    push %r15
*/

/*
    mov %ds, %ax
    push %ax
    mov %es, %ax
    push %ax
    mov %fs, %ax
    push %ax
    mov %gs, %ax
    push %ax
    mov %ss, %ax
    push %ax

    mov $0, %ax
*/

.endm # pushaq

.macro popaq
/*
    pop %ax
    mov %ax, %ss
    pop %ax
    mov %ax, %gs
    pop %ax
    mov %ax, %fs
    pop %ax
    mov %ax, %es
    pop %ax
    mov %ax, %ds
    pop %r15
    pop %r14
    pop %r13
    pop %r12
    pop %r11
    pop %r10
    pop %r9
    pop %r8
*/
    pop %rdi    
    pop %rsi    
    pop %rbp    
    pop %rbx    
    pop %rdx    
    pop %rcx
    pop %rax
    pop %rsp
.endm # popaq
 
.global irq0;
irq0:
    pushaq
    call irq0_handler
    popaq
    iretq
 
.global irq1;
irq1:
    pushaq
    call irq1_handler
    popaq
    iretq
 
.global irq2;
irq2:
    pushaq
    call irq2_handler
    popaq
    iretq
 
.global irq3;
irq3:
    pushaq
    call irq3_handler
    popaq
    iretq

.global irq4;
irq4:
    pushaq
    call irq4_handler
    popaq
    iretq

.global irq5;
irq5:
    pushaq
    call irq5_handler
    popaq
    iretq
 
.global irq6;
irq6:
    pushaq
    call irq6_handler
    popaq
    iretq
 
.global irq7;
irq7:
    pushaq
    call irq7_handler
    popaq
    iretq
 
.global irq8;
irq8:
    pushaq
    call irq8_handler
    popaq
    iretq

.global irq9;
irq9:
    pushaq
    call irq9_handler
    popaq
    iretq

.global irq10;
irq10:
    pushaq
    call irq10_handler
    popaq
    iretq

.global irq11;
irq11:
    pushaq
    call irq11_handler
    popaq
    iretq

.global irq12;
irq12:
    pushaq
    call irq12_handler
    popaq
    iretq
 
.global irq13;
irq13:
    pushaq
    call irq13_handler
    popaq
    iretq
 
.global irq14;
irq14:
    pushaq
    call irq14_handler
    popaq
    iretq
 
.global irq15;
irq15:
    pushaq
    call irq15_handler
    popaq
    iretq
 
.global int0
int0:
    pushaq
    call int0_handler
    popaq
    iretq

.global int1
int1:
    pushaq
    call int1_handler
    popaq
    iretq

.global int2
int2:
    pushaq
    call int2_handler
    popaq
    iretq

.global int3
int3:
    pushaq
    call int3_handler
    popaq
    iretq

.global int4
int4:
    pushaq
    call int4_handler
    popaq
    iretq

.global int5
int5:
    pushaq
    call int5_handler
    popaq
    iretq


.global int6
int6:
    pushaq
    call int6_handler
    popaq
    iretq

.global int7
int7:
    pushaq
    call int7_handler
    popaq
    iretq

.global int8
int8:
    pushaq
    call int8_handler
    popaq
    iretq

.global int9
int9:
    pushaq
    call int9_handler
    popaq
    iretq

.global int10
int10:
    pushaq
    call int10_handler
    popaq
    iretq

.global int11
int11:
    pushaq
    call int11_handler
    popaq
    iretq

.global int12
int12:
    pushaq
    call int12_handler
    popaq
    iretq

.global int13
int13:
    pushaq
    call int13_handler
    popaq
    iretq

.global int14
int14:
    pushaq
    call int14_handler
    popaq
    iretq

.global int15
int15:
    pushaq
    call int15_handler
    popaq
    iretq
