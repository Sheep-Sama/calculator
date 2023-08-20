%include "../lib.inc"
MAX_SIZE equ 256

section .data
data_buffer db MAX_SIZE dup (0)
;r15 = strlen + null pointer
%macro IntToStr 2

    push   rax             ; Preserve the value of the integer register
    push   rcx             ; Preserve the value of the loop counter register
    push   rdx             ; Preserve the value of the remainder register
    push   rdi

    mov    r15 , 1 ;for null byte 
    mov    rdi , data_buffer         ; Store the pointer to the destination string
    add    rdi , MAX_SIZE   ; Move the pointer to the end of the reserved bytes
    dec    rdi             ; move to one byte from the end
    mov    byte [rdi], 0   ; add null byte
    mov    rcx , 10         ; Set divisor for dividing by 10
    mov    rax , %1         ; Load the integer value to be converted

    cmp    rax, 0          ; Check if the number is zero
    jnz    %%start_loop     ; If it's not zero, continue with the loop

    ; If it's zero, directly store '0' and null-terminate the string
    inc    r15
    mov    rdi, %2 
    mov    byte [rdi], '0'
    mov    byte [rdi + 1] , 0
    jmp    %%exit

%%start_loop:
    xor    rdx , rdx        ; Clear the remainder register -->  avoid remainder and result concatenation
    div    rcx             ; Divide the number by 10 (quotient in RAX, remainder in RDX)
    add    dl , '0'         ; Convert the remainder (single digit) to ASCII
    dec    rdi             ; Move the pointer one position back
    inc    r15
    mov    [rdi] , dl       ; Store the digit in the string
    test   rax , rax        ; Check if quotient is zero
    jnz    %%start_loop     ; If not, continue the loop

%%end:

    mov    rdx , %2
    xor    rbx , rbx ; counter

%%storeLoop:
    mov al , byte [rdi]
    mov byte [rdx] , al
    inc rdi
    inc rdx
    inc rbx 
    cmp rbx , r15
    jne %%storeLoop

    mov rdi , data_buffer
    add rdi , MAX_SIZE
    sub rdi , r15
    xor rbx , rbx ; counter

%%clearLoop:
    mov byte [rdi] , 0
    inc rbx 
    inc rdi
    cmp rbx , r15 
    jne %%clearLoop

%%exit:

    pop    rdi
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
    cmp al , '^'
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

square_handling:
    FindChar user_input_no_space , '^'
    cmp rax , 1
    jne .return_no_print
    
    mov rdi , buffer
    mov rsi , user_input_no_space
    mov rbx , first_number 
    mov rcx , second_number
    xor rdx , rdx ;last operation found pointer
    xor r10 , r10 
    xor r9 , r9
    mov r11 , user_input_no_space ;last copied char
    ClearData buffer , MAX_SIZE
    ClearData first_number , MAX_SIZE
    ClearData second_number , MAX_SIZE

.loop:
    mov al , byte [rsi]
    inc rsi
    cmp al , 10
    je .return
    cmp al , '^'
    je .sqr
    cmp al , 48
    jge .number
    jmp .other_opperation


    .sqr:
    mov r9 , 0
    mov r10 , 1
    mov rdx , rsi - 1
    jmp .loop

    .other_opperation:
    mov r9 , 1
    mov r10 , 1
    mov rdx , rsi - 1
    jmp .loop

    .number:
    cmp r10 , 1
    je .second_number
    ;.first_number:
    mov r15b , [rsi - 1]
    mov byte [rbx] , r15b
    inc rbx

    cmp byte [rsi] , 10
    je .copy_end

    jmp .loop
    .second_number:
    mov r15b , [rsi - 1]
    mov byte [rcx] , r15b
    inc rcx 

    cmp byte [rsi] , '^'
    je .processing_math
    cmp byte [rsi] , 48
    jl .processing_math ; not a number !

    jmp .loop

    .processing_math:
    xor r10 , r10;to start again at first number
    cmp r9 , 1
    mov rbx , first_number 
    je .other_opperation_copy
    mov rcx , second_number 
    StrToInt rbx 
    StrToInt rcx
    call _process_sqr
    cmp byte [rsi] , 0
    je .return
    jmp .end

    .other_opperation_copy:
    mov rsi , rdx; move back pointer for next opperation

    .copy_loop:
    mov al , byte [rbx]
    mov byte [rdi] , al
    inc rbx 
    inc rdi 
    cmp byte [rbx] , 0
    jne .copy_loop
    ;copy opperator
    dec rdx 
    mov al , byte [rdx]
    mov byte [rdi] , al
    inc rdi

    .end:
    ClearData first_number , MAX_SIZE
    ClearData second_number , MAX_SIZE
    mov rbx , first_number 
    mov rcx , second_number
    jmp .loop

.copy_end:
    mov rbx , first_number 

    .copy_last_int_Loop:
    mov al , byte [rbx]
    mov byte [rdi] , al
    inc rbx 
    inc rdi 
    cmp byte [rbx] , 0
    jne .copy_last_int_Loop
    
    mov byte [rdi] , 10
    inc rdi
    
    jmp .return


.error_while_processing:
    mov rax , 1
    mov rdi , 1
    mov rdx , 34
    mov rsi , error_message_pf
    syscall
    mov r8 , 1
    ret

.return:

    StrLen user_input_no_space
    ClearData user_input_no_space , rax
    StrLen buffer;output = rax
    CopyChar user_input_no_space , buffer , rax
    
    FindChar user_input_no_space , '^'
    cmp rax , 1
    je square_handling

    mov rax , 1
    mov rdi , 1
    mov rsi , user_input_no_space
    mov rdx , 256
    syscall
.return_no_print:
ret
_process_sqr:

cmp rcx , 1
je .after_process
cmp rcx , 0
je .square_zero

mov r11 , 1
mov r12 , rbx 
.calc_loop:
imul rbx , r12
inc r11 
cmp r11 , rcx
jne .calc_loop
jmp .after_process

.square_zero:
mov rbx , 0

.after_process:
IntToStr rbx , first_number
mov rbx , first_number
.store_loop:
mov al , byte [rbx]
mov byte [rdi] , al
inc rbx 
inc rdi
cmp byte [rbx] , 0
jne .store_loop

;move opperation:
mov al , byte [rsi]
mov byte [rdi] , al
inc rsi
inc rdi
ret 


resolve_mul_div:
    FindChar rsi , '*'
    cmp rax , 1
    je .next
    
    FindChar rsi , '/'
    cmp rax , 1
    je .next
    ret

    .next:
    xor r8 , r8 ; 0 = no error / 1 = error
    xor r9 , r9 ; 1 = div / 2 = mul 
    xor r10 , r10 ; 0 = writing first number / 1 = writing second number / ending first number
    mov rbx , first_number 
    mov rcx , second_number 
    ClearData first_number , MAX_SIZE
    ClearData second_number , MAX_SIZE
    ClearData buffer , MAX_SIZE
    _the_loop:
    cmp r10 , 2
    je .processing_math
    mov al , byte [rsi]
    inc rsi 
    cmp al , 10 ; check if end of string
    je .return
    cmp al , 43
    je .opperation
    cmp al , 45
    je .opperation
    cmp al , 42
    je .multiplication
    cmp al , 47
    je .division
    cmp al , 48
    jl _the_loop
    cmp al , 57
    jle .number
    
    jmp _the_loop

    .opperation:
    jmp resolve_mul_div

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

    cmp byte [rsi] , 48
    jl .processing_math ; not a number !

    jmp _the_loop


    .processing_math:
    mov rbx , first_number 
    mov rcx , second_number 

    cmp byte [rbx] , 0
    je .error_while_processing
    cmp byte [rcx] , 0
    je .error_while_processing

    StrToInt rbx 
    StrToInt rcx
    cmp r9 , 0
    je .prc_div
    call _perform_multiplication
    jmp .after_process

    .prc_div:
    call _perform_division


    .after_process:
    mov rdi , user_input_no_space
    FindChar rdi , '+'
    cmp rax , 1
    je .concat
    FindChar rdi , '-'
    cmp rax , 1
    je .concat

    cmp byte [rsi] , 10 ; 10 = new line
    je .print_result

    .concat:
    call _concat_buffer

    jmp resolve_mul_div

    .error_while_processing:
    mov rax , 1
    mov rdi , 1
    mov rdx , 34
    mov rsi , error_message_pf
    syscall
    mov r8 , 1
    ret


.return:
    cmp r10 , 1
    je .processing_math
.print_result:
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
ret 

_perform_multiplication:
imul rbx , rcx 
IntToStr rbx , buffer
ret 

_concat_buffer:
mov rdi , buffer
dec r15; remove null byte
add rdi , r15;goto string end
;counting rsi length
mov rax , rsi 
xor rbx , rbx
.counting_loop:
mov cl , byte [rax]
inc rbx 
inc rax 
cmp cl , 10
jne .counting_loop

CopyChar rdi , rsi , rbx 
;check if operation
mov rsi , user_input_no_space 
xor rbx , rbx 
xor rcx , rcx 
.checking_loop:
    mov al , byte [rsi]
    inc rsi 
    inc rbx
    cmp al , '+'
    je .has_opperation
    cmp al , '-'
    je .has_opperation
    cmp al , '*' 
    je .loop_end
    cmp al , '/'
    je .loop_end

    jmp .checking_loop

    .has_opperation:
    mov rcx , rbx 
    jmp .checking_loop
.loop_end:
    cmp rcx , 0
    je .print_data
    ; found opperation !
    CopyChar data_buffer , user_input_no_space , rcx 
    mov rsi , data_buffer 
    add rsi , rcx ;rsi = end of data_buffer
    StrLen buffer; output = rax
    CopyChar rsi , buffer , rax
    CopyChar buffer , data_buffer , MAX_SIZE
.print_data:
ClearData user_input_no_space , MAX_SIZE
CopyChar user_input_no_space , buffer , MAX_SIZE
StrLen user_input_no_space ;output = rax
mov rsi , user_input_no_space
mov rdi , 1
mov rdx , rax
mov rax , 1
syscall
ret 


;           _____  _____              _____ _    _ ____ 
;     /\   |  __ \|  __ \    ___     / ____| |  | |  _ \
;    /  \  | |  | | |  | |  ( _ )   | (___ | |  | | |_) |
;   / /\ \ | |  | | |  | |  / _ \/\  \___ \| |  | |  _ <
;  / ____ \| |__| | |__| | | (_>  <  ____) | |__| | |_) |
; /_/    \_\_____/|_____/   \___/\/ |_____/ \____/|____/


resolve_add_sub:
    ;check if there is addition or substraction
    FindChar rsi , '+'
    cmp rax , 1
    je .next
    
    FindChar rsi , '-'
    cmp rax , 1
    je .next
    ret

    .next:
    ;for '-{int}' or '+{int}' case
    inc rsi 
    FindChar rsi , '+'
    cmp rax , 1
    je .next2
    FindChar rsi , '-'
    cmp rax , 1
    je .next2

    cmp byte [rsi] , 10; for '+' and '-' case
    jne .valid_int
    ret 
    
    .valid_int:

    CopyChar buffer , user_input_no_space , MAX_SIZE
    StrLen buffer 
    mov r15 , rax 
    sub r15 , 2 ; remove the line breaking chars
    jmp _the_loop_as.print_result  

    .next2:
    dec rsi 
    
    xor r8 , r8 ; 0 = no error / 1 = error
    xor r9 , r9 ; 0 = sub / 1 = add 
    xor r10 , r10 ; 0 = writing first number / 1 = writing second number / ending first number
    mov rbx , first_number 
    mov rcx , second_number 
    ClearData first_number , MAX_SIZE
    ClearData second_number , MAX_SIZE
    ClearData buffer , MAX_SIZE
    _the_loop_as:
    cmp r10 , 2
    je .processing_math
    mov al , byte [rsi]
    inc rsi 
    cmp al, 10 ; check if end of string
    je .return
    cmp al , '+'
    je .addition
    cmp al , '-'
    je .substraction
    cmp al , 48
    jl _the_loop_as
    cmp al , 57
    jle .number
    
    jmp _the_loop_as


    .addition:
    mov r9 , 1
    inc r10
    jmp _the_loop_as

    .substraction:
    cmp byte [first_number] , 0
    je .negative_first_number
    mov r9 , 0
    inc r10
    jmp _the_loop_as
    .negative_first_number:
    mov r12 , 1
    jmp _the_loop_as


    .number:
    cmp r10 , 1
    je .second_number
    ;.first_number:
    mov r15b , [rsi - 1]
    mov byte [rbx] , r15b
    inc rbx
    jmp _the_loop_as
    .second_number:
    mov r15b , [rsi - 1]
    mov byte [rcx] , r15b
    inc rcx 

    cmp byte [rsi] , 48
    jl .processing_math ; not a number !

    jmp _the_loop_as


    .processing_math:
    mov rbx , first_number 
    mov rcx , second_number 

    cmp byte [rcx] , 0
    je .error_while_processing

    StrToInt rbx 
    StrToInt rcx
    cmp r9 , 0
    je .prc_div
    call _perform_addition
    jmp .after_process

    .prc_div:
    call _perform_substraction
    
    .after_process:
    xor r12 , r12

    cmp byte [rsi] , 10 ; 10 = new line
    je .print_result
    call _concat_buffer_as
    jmp resolve_add_sub

    .error_while_processing:
    mov rax , 1
    mov rdi , 1
    mov rdx , 34
    mov rsi , error_message_pf
    syscall
    mov r8 , 1
    ret


.return:
    cmp r10 , 1
    je .processing_math
.print_result:
    mov rax , 1
    mov rdi , 1
    mov rsi , buffer
    mov rdx , r15
    inc rdx 
    syscall
    endl
ret

_perform_substraction:
cmp r12 , 0
je .first_number_positive

add rbx , rcx 
IntToStr rbx , buffer + 1

mov rdi , buffer
mov al , '-'
mov byte [rdi], al 
inc r15 ;for the '-' added to the string

jmp .return



.first_number_positive:

cmp rbx , rcx 
jl .negative_res

sub rbx , rcx 
IntToStr rbx , buffer
jmp .return 

.negative_res:
mov r10 , rbx 
mov rbx , rcx 
mov rcx , r10 
sub rbx , rcx 
IntToStr rbx , buffer + 1

mov rdi , buffer
mov al , '-'
mov byte [rdi], al 
inc r15 ;for the '-' added to the string

.return:
ret 

_perform_addition:
;add support for negative int !!!
cmp r12 , 0
je .first_number_positive


cmp rbx , rcx 
jl .negative_res

;-2+3 --> not working =(
sub rbx , rcx 
IntToStr rbx , buffer + 1
mov rdi , buffer
mov al , '-'
mov byte [rdi], al 
inc r15 ;for the '-' added to the string
jmp .return

.negative_res:
;switch rcx and rbx
mov r10 , rbx 
mov rbx , rcx 
mov rcx , r10 

sub rbx , rcx 
IntToStr rbx , buffer
jmp .return



.first_number_positive:
add rbx , rcx 
IntToStr rbx , buffer

.return:
ret 

_concat_buffer_as:
mov rdi , buffer
dec r15; remove null byte
add rdi , r15;goto string end
;counting rsi length
mov rax , rsi 
xor rbx , rbx
.counting_loop:
mov cl , byte [rax]
inc rbx 
inc rax 
cmp cl , 10
jne .counting_loop

CopyChar rdi , rsi , rbx 
mov rsi , buffer
ClearData user_input_no_space , MAX_SIZE
CopyChar user_input_no_space , rsi , MAX_SIZE
mov rsi , user_input_no_space
mov rdi , 1
mov rdx , 256
mov rax , 1
syscall

ret

;to do
;fix bug : 2 + 5^4 + 3
;catch error : 4^ = segfault