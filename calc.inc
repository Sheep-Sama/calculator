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
