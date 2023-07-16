%include "../lib.inc"
%include "calc.inc"

section .bss
    user_input resb 256

section .data
    closing_angle_braquet db "> "
    error_message_ic db "Error (1): invalide character",10
    error_message_cdz db "Error (2): can't divide by zero",10
    error_message_m db "Error (3): invalide math opperation",10
    exit_cmd db "exit"
    exit_cmd2 db "quit"
    for_testing db "opperation succeed !",10

section .text
global _start
_start:
    call _prompt
    call _read_user_input
    call _check_exit
    call _check_invalide_char
    cmp r8 , 1
    je .restart
    call _check_invalide_division
    cmp r8 , 1
    je .restart


    mov rax , 1
    mov rdi , 1
    mov rsi , for_testing
    mov rdx , 21
    syscall
    ; calculation and output goes here

.restart:
    jmp _start
