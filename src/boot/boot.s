    .set ALIGN,    1<<0
    .set MEMINFO,  1<<1
    .set FLAGS,    ALIGN | MEMINFO
    .set MAGIC,    0x1BADB002
    .set CHECKSUM, -(MAGIC + FLAGS)

    .section .multiboot
    .align 4
    .long MAGIC
    .long FLAGS
    .long CHECKSUM

    .section .bss
    .align 16
    stack_bottom:
    .skip 16384 # 16 KiB
    stack_top:

    .section .text


    .global idt_load
    .type idt_load, @function
    idt_load:
        mov 4(%esp), %eax
        lidt (%eax)
        ret

    .macro ISR_NO_ERR index
        .global _isr\index
        _isr\index:
            cli
            push $0
            push $\index
            jmp isr_common
    .endm

    .macro ISR_ERR index
        .global _isr\index
        _isr\index:
            cli
            push $\index
            jmp isr_common
    .endm

    ISR_NO_ERR 0
    ISR_NO_ERR 1
    ISR_NO_ERR 2
    ISR_NO_ERR 3
    ISR_NO_ERR 4
    ISR_NO_ERR 5
    ISR_NO_ERR 6
    ISR_NO_ERR 7
    ISR_ERR 8
    ISR_NO_ERR 9
    ISR_ERR 10
    ISR_ERR 11
    ISR_ERR 12
    ISR_ERR 13
    ISR_ERR 14
    ISR_NO_ERR 15
    ISR_NO_ERR 16
    ISR_NO_ERR 17
    ISR_NO_ERR 18
    ISR_NO_ERR 19
    ISR_NO_ERR 20
    ISR_NO_ERR 21
    ISR_NO_ERR 22
    ISR_NO_ERR 23
    ISR_NO_ERR 24
    ISR_NO_ERR 25
    ISR_NO_ERR 26
    ISR_NO_ERR 27
    ISR_NO_ERR 28
    ISR_NO_ERR 29
    ISR_NO_ERR 30
    ISR_NO_ERR 31
    ISR_NO_ERR 32
    ISR_NO_ERR 33
    ISR_NO_ERR 34
    ISR_NO_ERR 35
    ISR_NO_ERR 36
    ISR_NO_ERR 37
    ISR_NO_ERR 38
    ISR_NO_ERR 39
    ISR_NO_ERR 40
    ISR_NO_ERR 41
    ISR_NO_ERR 42
    ISR_NO_ERR 43
    ISR_NO_ERR 44
    ISR_NO_ERR 45
    ISR_NO_ERR 46
    ISR_NO_ERR 47

    /* defined in isr.c */
    .extern isr_handler
    .type isr_handler, @function

    isr_common:
        pusha
        push %ds
        push %es
        push %fs
        push %gs

        mov $0x10, %ax
        mov %ax, %ds
        mov %ax, %es
        mov %ax, %fs
        mov %ax, %gs
        cld

        push %esp
        call isr_handler
        add $4, %esp

        pop %gs
        pop %fs
        pop %es
        pop %ds

        popa

        add $8, %esp
        iret


    .global _start
    .type _start, @function
    _start:
        mov $stack_top, %esp

        call kernel_main

        cli
    1:	hlt
        jmp 1b

    .size _start, . - _start

        .section .text

.code32
.global _read_from_disk
.type _read_from_disk, @function
_read_from_disk:
    # %ecx: sector
    # %esi: buffer
    # %edx: size (number of bytes)
    push %esi    # Save buffer pointer
    push %edx    # Save size

    mov $0x02, %ah     # Read sector function
    mov %dl, %al       # Number of sectors (size / 512)
    shr $9, %al        # Divide by 512 to get sector count
    mov $0x00, %ch     # Cylinder
    mov $0x01, %cl     # Starting sector
    mov $0x00, %dh     # Head
    mov $0x00, %dl     # Drive (0 = first floppy drive)
    int $0x13          # BIOS disk interrupt
    
    pop %edx           # Restore size
    pop %esi           # Restore buffer pointer
    ret

read_disk_error:
    # Simple error handling - you might want to expand this
    mov $0, %eax       # Return 0 to indicate error
    pop %edx
    pop %esi
    ret

.code32
.global _write_to_disk
.type _write_to_disk, @function
_write_to_disk:
    # Similar implementation for write
    push %esi    # Save buffer pointer
    push %edx    # Save size

    mov $0x03, %ah     # Write sector function
    mov %dl, %al       # Number of sectors (size / 512)
    shr $9, %al        # Divide by 512 to get sector count
    mov $0x00, %ch     # Cylinder
    mov $0x01, %cl     # Starting sector
    mov $0x00, %dh     # Head
    mov $0x00, %dl     # Drive (0 = first floppy drive)
    int $0x13          # BIOS disk interrupt
    
    pop %edx           # Restore size
    pop %esi           # Restore buffer pointer
    ret
    
write_disk_error:
    # Simple error handling - you might want to expand this
    mov $0, %eax       # Return 0 to indicate error
    pop %edx
    pop %esi
    ret
