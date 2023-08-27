%include "calc.inc"
MAX_SIZE equ 256

section .bss
    user_input resb 256

section .data
    closing_angle_braquet db "> "
    error_message_ic db "Error (1): invalide character",10
    error_message_cdz db "Error (2): can't divide by zero",10
    error_message_imo db "Error (3): invalide math opperation",10
    error_message_pf db "Error (4): math processing failed",10
    exit_cmd db "exit"
    exit_cmd2 db "quit"
    user_input_no_space db MAX_SIZE dup (0)
    buffer db MAX_SIZE dup (0) , 1
    first_number db MAX_SIZE dup (0)
    second_number db MAX_SIZE dup (0)


section .text
global _start
_start:
    ClearData user_input , MAX_SIZE

    call _prompt
    call _read_user_input
    call _check_exit
    call _check_invalide_char
    cmp r8 , 1
    je .restart

    call _check_invalide_division
    cmp r8 , 1
    je .restart

    call opperations_simplification
    cmp r8 , 1
    je .restart
    
    call _check_invalide_math
    cmp r8 , 1
    je .restart

    ClearData user_input_no_space , MAX_SIZE
    call _remove_space
    
    ClearData user_input , MAX_SIZE; no need for user_input

    call square_handling
    cmp r8 , 1
    je .restart

    mov rsi , user_input_no_space
    call resolve_mul_div
    cmp r8 , 1
    je .restart

    mov rsi , user_input_no_space
    call resolve_add_sub
    cmp r8 , 1
    je .restart

.restart:
    jmp _start

