%include "../lib.inc"
MAX_SIZE equ 256

section .data
data_buffer db MAX_SIZE dup (0)
;r15 = strlen + null pointer
%macro IntToStr 2

    push   rax             ; Preserve the value of the integer register
    push   rcx             ; Preserve the value of the loop counter register
    push   rdx             ; Preserve the value of the remainder register

    mov    r15 , 1 ;for null byte 
    mov    rdi , data_buffer         ; Store the pointer to the destination string
    add    rdi , MAX_SIZE   ; Move the pointer to the end of the reserved bytes
    dec    rdi             ; move to one byte from the end
    mov    byte [rdi], 0   ; add null byte
    mov    rcx , 10         ; Set divisor for dividing by 10
    mov    rax , %1         ; Load the integer value to be converted

    cmp    rax, 0          ; Check if the number is zero
    jnz    .start_loop     ; If it's not zero, continue with the loop

    ; If it's zero, directly store '0' and null-terminate the string
    mov    byte [rdi], '0'
    jmp    .end

.start_loop:
    xor    rdx , rdx        ; Clear the remainder register -->  avoid remainder and result concatenation
    div    rcx             ; Divide the number by 10 (quotient in RAX, remainder in RDX)
    add    dl , '0'         ; Convert the remainder (single digit) to ASCII
    dec    rdi             ; Move the pointer one position back
    inc    r15
    mov    [rdi] , dl       ; Store the digit in the string
    test   rax , rax        ; Check if quotient is zero
    jnz    .start_loop     ; If not, continue the loop

.end:

    mov    rdx , %2
    xor    rbx , rbx ; counter

.storeLoop:
    mov al , byte [rdi]
    mov byte [rdx] , al
    inc rdi
    inc rdx 
    inc rbx 
    cmp rbx , r15
    jne .storeLoop

    mov rdi , data_buffer
    add rdi , MAX_SIZE
    sub rdi , r15
    xor rbx , rbx ; counter

.clearLoop:
    mov byte [rdi] , 0
    inc rbx 
    cmp rbx , r15 
    jne .clearLoop

    pop    rdx             ; Restore the value of the remainder register
    pop    rcx             ; Restore the value of the loop counter register
    pop    rax             ; Restore the value of the integer register


%endmacro


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
    jmp .check_if_invalide_loop_d


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
    inc rsi 
    cmp al, 0; to keep new line character 
    je .return
    cmp al , 32
    jne .no_space
    jmp _remove_space_loop

    .no_space:
    inc rdi 
    mov byte [rdi], al
    jmp _remove_space_loop

.return:
    ret




resolve_mul_sub:
    xor r8 , r8 ; 0 = no error / 1 = error
    xor r9 , r9 ; 0 = div / 1 = mul 
    xor r10 , r10 ; 0 = writing first number / 1 = writing second number / ending first number
    mov rbx , first_number 
    mov rcx , second_number 
    _the_loop:
    cmp r10 , 2
    je .processing_math
    mov al , byte [rsi]
    inc rsi 
    cmp al, 10 ; check if end of string
    je .return
    cmp al , 42
    je .multiplication
    cmp al , 47
    je .division
    cmp al , 48
    jl _the_loop
    cmp al , 57
    jle .number
    
    jmp _the_loop


    .multiplication:
    mov r9 , 1
    inc r10
    jmp _the_loop

    .division:
    mov r9 , 0
    inc r10
    jmp _the_loop


    .number:
    cmp r10 , 1
    je .second_number
    ;.first_number:
    mov r15b , [rsi - 1]
    mov byte [rbx] , r15b
    inc rbx
    jmp _the_loop
    .second_number:
    mov r15b , [rsi - 1]
    mov byte [rcx] , r15b
    inc rcx 

    jmp _the_loop


    .processing_math:
    dec rsi; to make rsi at last char
    mov rbx , first_number 
    mov rcx , second_number 
    StrToInt rbx 
    StrToInt rcx
    cmp r9 , 0
    je .prc_div
    call _perform_multiplication
    cmp byte [rsi] , 10 ; 10 = new line
    je .print_result
    ;next step
    ; other string = rsi
    dec r15
    add rdi , r15;goto string end

    jmp resolve_mul_sub

    .prc_div:
    call _perform_division
    cmp byte [rsi] , 10 ; 10 = new line
    je .print_result
    ; next step
    jmp resolve_mul_sub

    .error_while_processing:
    mov rax , 1
    mov rdi , 1
    mov rdx , 29
    mov rsi , error_message_pf
    syscall
    mov r8 , 1
    jmp .return


.return:
    cmp r10 , 1
    je .processing_math
.print_result:
    ;AppendNewLine buffer; segfault here
    mov rax , 1
    mov rdi , 1
    mov rsi , buffer
    mov rdx , r15
    inc rdx 
    syscall
    endl
ret

_perform_division:
mov rdx , 0
mov rax , rbx
idiv rcx 
IntToStr rax , buffer
dec r15
ret 

_perform_multiplication:
imul rbx , rcx 
IntToStr rbx , buffer
dec r15
ret 

; get first number into rbx
; get second number into rcx 

;10 /5 * 3
;rbx = 10
;rcx = 5
;perform calc = rax
;mov rax , into "buffer"
;append rsi until null byte into "buffer"
;"buffer" = 2 * 3
;clear "user_input_no_space"
;cleat first_number and second_number

;rbx = 2
;rcx = 3
;perform calc = rax
;mov rax , into "user_input_no_space"
;append rsi until null byte into "user_input_no_space"
;"user_input_no_space" = 6
;clear "buffer"

;--start over
