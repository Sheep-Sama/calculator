
section .bss
    IntBuffer resb 100
    IntBufferPos resb 8

section .data 
    newline db 0xa
    doubleQuote db 0x22


%macro printInt 1
    mov rax, %1
    mov rcx, IntBuffer
    mov [IntBufferPos], rcx
    %%storeLoop:
        mov rdx, 0
        mov rbx, 10
        div rbx
        push rax
        add rdx, 48
        mov [rcx] ,dl
        mov rcx, [IntBufferPos]
        inc rcx 
        mov [IntBufferPos], rcx

        pop rax
        cmp rax,0
        jne %%storeLoop
    %%printLoop:
        mov rcx, [IntBufferPos]
        mov rax, 1
        mov rdi, 1
        mov rsi, rcx
        mov rdx ,1 
        syscall

        mov rcx, [IntBufferPos]
        dec rcx 
        mov [IntBufferPos], rcx

        cmp rcx, IntBuffer
        jge %%printLoop 
%endmacro

%macro printString 1
   mov rax , %1
   mov rbx,0
   %%cloop:
        inc rax
        inc rbx
        mov cl, [rax]
        cmp cl, 0
        jne %%cloop
    mov rax, 1
    mov rdi, 1
    pop rsi
    mov rdx, rbx
    ret
%endmacro

%macro endl 0
    mov rax, 1
    mov rdi, 1
    mov rsi, newline
    mov rdx ,1 
    syscall
%endmacro

%macro exit 0
    mov rax,60
    mov rdi,0
    syscall
%endmacro


%macro copyStack 1
    pop %1
    push %1
%endmacro

%macro printDoubleQuote 0
    push rcx 
    push rdx 
    push rsi
    
    mov rdi , 1
    mov rax , 1
    mov rdx , 1
    mov rsi , doubleQuote
    syscall

    pop rsi 
    pop rdx 
    pop rcx
%endmacro

%macro Cnot 1
    cmp %1 , 0
    je %%w_one
    mov rcx , 0

    jmp %%end 
    %%w_one:
    mov rcx , 1

    %%end:

%endmacro

%macro StrToInt 1
    mov r15 , %1
    xor %1, %1 

%%next_digit:
    movzx r14, byte [r15] 

    test r14, r14
    jz %%done 

    imul %1, %1, 10 
    sub r14, '0'; '0' = 48
    add %1, r14 

    inc r15
    jmp %%next_digit
%%done: ; escape the loop !
%endmacro

; 1 = pointer / 2 = number of bytes
%macro ClearData 2 
    push rsi 
    push rbx 
    push rax 

    xor rbx , rbx
    mov rsi , %1 

%%ClearLoop:
    mov byte [rsi] , 0
    inc rsi 
    inc rbx 
    cmp rbx , %2
    jne %%ClearLoop


    pop rax 
    pop rbx 
    pop rsi
%endmacro

; 1 = pointer to string to write / 2 = pointer for reading string / 3 = number of char to write
%macro CopyChar 3
    push rsi 
    push rdi 
    push rbx
    push rax 
    push rdx 

    mov rdi, %1
    mov rsi, %2
    mov rdx ,%3
    xor rbx, rbx
    
%%Copy_loop:
    mov al , byte [rsi]
    mov byte [rdi] , al
    inc rbx 
    inc rsi 
    inc rdi 
    cmp rbx , rdx 
    jne %%Copy_loop
    
    pop rdx 
    pop rax 
    pop rbx 
    pop rdi 
    pop rsi 
%endmacro

%macro AppendNewLine 1
    push rsi 
    push rax 

    mov rsi , %1
.loop:
    mov al ,byte [rsi]
    inc rsi 
    cmp al , 0
    jne .loop

    mov byte [rsi - 1] , 10;add return char
    mov byte [rsi] , 0;add null pointer back


    push rax 
    pop rsi
%endmacro

;result = rax = 0 = don't find ; 1 = find
%macro FindChar 2
    push rsi
    push rbx 

    mov rsi , %1 
    mov bl , %2

    %%searching_loop:
    mov al , byte [rsi]
    cmp al , %2 
    inc rsi
    cmp al , 0
    je %%not_found
    cmp al , bl
    je %%found
    
    jmp %%searching_loop

    %%found:
    mov rax , 1
    jmp %%end
    %%not_found:
    mov rax , 0
    
    %%end:

    pop rbx 
    pop rsi 
%endmacro

;output = rax
%macro StrLen 1
    push rsi 
    push rbx 

    xor rax , rax
    mov rsi , %1
%%loop:
    mov bl , byte [rsi]
    inc rsi 
    inc rax 
    cmp bl , 0 
    jne %%loop
    
    dec rax ;could make bug

    pop rbx
    pop rsi
%endmacro


;output = rax
%macro StrLenLB 1
    push rsi 
    push rbx 

    xor rax , rax
    mov rsi , %1
%%loop:
    mov bl , byte [rsi]
    inc rsi 
    inc rax 
    cmp bl , 10
    jne %%loop
    
    ;dec rax ;count also the new line char

    pop rbx
    pop rsi
%endmacro