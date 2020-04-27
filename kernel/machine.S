
    .global outb;
    .type outb, %function;

outb: #uint16_t port, uint8_t value
    movq    %rdi, %rdx
    movq    %rsi, %rax
    outb    %al, %dx
    ret
    .size outb, .-outb


    .global inb;
    .type inb, %function;
inb:
    movq    $0, %rax
    movq    %rdi, %rdx
    inb     %dx, %al
    ret
    .size inb, .-inb

