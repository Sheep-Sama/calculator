section .text 

_prompt:    
    mov rax , 1
    mov rdi , 1
    mov rdx , 2
    mov rsi , closing_angle_braquet
    syscall
    ret

_read_user_input:
    mov rax, 0 
    mov rdi, 0 
    mov rsi, user_input 
    mov rdx, 256
    syscall
    ret

_check_exit:
    xor rcx , rcx
    mov rsi , user_input ; has null terminator
    mov rdi , exit_cmd
    .check_if_exit_loop:
    mov al , byte [rsi]
    mov bl , byte [rdi]
    inc rsi 
    inc rdi 
    inc rcx 
    cmp rcx , 5; needs 5 iteration to get to the string end 
    je .exit_check 
    cmp al , bl
    je .check_if_exit_loop

    xor rcx , rcx
    mov rsi , user_input ; has null terminator
    mov rdi , exit_cmd2
    .check_if_exit_loop2:
    mov al , byte [rsi]
    mov bl , byte [rdi]
    inc rsi 
    inc rdi 
    inc rcx 
    cmp rcx , 5
    je .exit_check 
    cmp al , bl
    je .check_if_exit_loop2

    ret 

.exit_check:
    cmp al , 0xa ; check if the last char is the return char
    jne .return
    exit
    .return:
    ret 


_check_invalide_char:
    mov rsi , user_input ; has null terminator
    .check_if_invalide_loop:
    mov al , byte [rsi]
    inc rsi 
    cmp al, 10 ; check if end of char
    je .no_invalide_char
    cmp al , 32
    je .check_if_invalide_loop
    cmp al , 42
    je .check_if_invalide_loop 
    cmp al , 43
    je .check_if_invalide_loop
    cmp al , 45 
    je .check_if_invalide_loop
    cmp al , 47
    jl .invalide_char
    cmp al , 57
    jg .invalide_char
    
    jmp .check_if_invalide_loop ; if value between 47 and 57

    .invalide_char:
    mov rax , 1
    mov rdi , 1
    mov rdx , 30
    mov rsi , error_message_ic
    syscall
    mov r8 , 1
    ret 

    .no_invalide_char:
    xor r8 , r8
    ret

_check_invalide_division:    
    xor rcx , rcx ; 1 = found division
    mov rsi , user_input ; has null terminator
    .check_if_invalide_loop_m:
    mov al , byte [rsi]
    inc rsi 
    cmp al, 10 ; check if end of char
    je .no_invalide_math
    cmp al , 47
    je .division
    cmp al , 32 
    je .check_if_invalide_loop_m
    cmp al , 48
    je .zero
    
    xor rcx , rcx
    jmp .check_if_invalide_loop_m


    .zero:
    cmp rcx , 1
    je .invalide_math_cdz

    .division:
    mov rcx , 1
    jmp .check_if_invalide_loop_m

    .invalide_math_cdz:
    mov rax , 1
    mov rdi , 1
    mov rdx , 32
    mov rsi , error_message_cdz
    syscall
    mov r8 , 1
    ret 

    .invalide_math:
    mov rax , 1
    mov rdi , 1
    mov rdx , 36
    mov rsi , error_message_m
    syscall
    mov r8 , 1
    ret 

    .no_invalide_math:
    xor r8 , r8
    ret

_multiplication:
ret     

_addition:
ret

_substraction:
ret 

_division:;can't divide by 0 !
ret