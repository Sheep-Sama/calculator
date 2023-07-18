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
    .check_if_invalide_loop_d:
    mov al , byte [rsi]
    inc rsi 
    cmp al, 10 ; check if end of char
    je .no_invalide_division
    cmp al , 47
    je .division
    cmp al , 32 
    je .check_if_invalide_loop_d
    cmp al , 48
    je .zero

    xor rcx , rcx
    jmp .check_if_invalide_loop_d


    .zero:
    cmp rcx , 1
    je .invalide_math_cdz

    .division:
    mov rcx , 1
    jmp .check_if_invalide_loop_d

    .invalide_math_cdz:
    mov rax , 1
    mov rdi , 1
    mov rdx , 32
    mov rsi , error_message_cdz
    syscall
    mov r8 , 1
    ret 

    .no_invalide_division:
    xor r8 , r8
    ret

_check_invalide_math:    
    xor rcx , rcx ; 1 = found division
    mov rsi , user_input ; has null terminator
    .check_if_invalide_loop_m:
    mov al , byte [rsi]
    inc rsi 
    cmp al, 10 ; check if end of char
    je .no_invalide_math
    cmp al , 32 
    je .check_if_invalide_loop_m
    cmp al , 42
    je .math_operation
    cmp al , 43
    je .math_operation
    cmp al , 45
    je .math_operation
    cmp al , 47
    je .math_operation


    xor rcx , rcx
    jmp .check_if_invalide_loop_m


    .math_operation:
    cmp rcx , 1
    je .invalide_math_imo
    mov rcx , 1
    jmp .check_if_invalide_loop_m

    .invalide_math_imo:
    mov rax , 1
    mov rdi , 1
    mov rdx , 36
    mov rsi , error_message_imo
    syscall
    mov r8 , 1
    ret 

    .no_invalide_math:
    xor r8 , r8
    ret 



_clear_data_user_input_no_space:
    mov rsi , user_input_no_space
    clear_data_loop:
    mov al , byte [rsi]
    inc rsi 
    cmp al, 1
    je .return
    mov byte [rsi - 1] , 0 ; al = rsi - 1
    jmp clear_data_loop

.return:
    ret




_remove_space:
    mov rsi , user_input 
    mov rdi , user_input_no_space
    dec rdi
_remove_space_loop:
    mov al , byte [rsi]
    inc rdi 
    inc rsi 
    cmp al, 10 
    je .return
    cmp al , 32
    jne .no_space
    jmp _remove_space_loop

    .no_space:
    mov byte [rdi], al
    jmp _remove_space_loop



.return:
    mov rax , 1
    mov rdi , 1
    mov rdx , 57
    mov rsi , user_input_no_space 
    syscall
    ret

resolve_mul_sub:
    xor r8 , r8
    xor rbx , rbx ; 1st number
    xor rcx , rcx ; 2nd number
    mov rsi , user_input ; has null terminator
    _the_loop:
    mov al , byte [rsi]
    inc rsi 
    cmp al, 10 ; check if end of char
    je .return
    cmp al , 32
    je .space
    cmp al , 42
    je .multiplication
    cmp al , 47
    je .division
    cmp al , 48
    jl _the_loop
    cmp al , 57
    jle .number
    
    jmp _the_loop

    .space:
    mov rcx , 1
    jmp _the_loop

    ;rbx = number
    .multiplication:
    xor rcx , rcx
    cmp rbx , 0
    je .error_while_processing

    jmp _the_loop 

    .division:
    xor rcx , rcx
    cmp rbx , 0
    je .error_while_processing
    mov rax , 1
    mov rdi , 1
    mov rdx , 29
    mov rsi , error_message_pf
    syscall
    jmp _the_loop

    .number:
    cmp rcx ,  1
    je _the_loop
    mov rbx , rsi

    jmp _the_loop

    .error_while_processing:
    mov rax , 1
    mov rdi , 1
    mov rdx , 29
    mov rsi , error_message_pf
    syscall
    mov r8 , 1
    jmp .return


.return:
    ret

