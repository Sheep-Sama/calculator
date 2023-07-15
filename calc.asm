%include "../lib.inc"
%include "calc.inc"

section .bss
    user_input resb 256

section .data
    closing_angle_braquet db "> "
    error_message_cro db "Error (1): couldn't resolve opperation"
    exit_cmd db "exit"
    exit_cmd2 db "quit"


section .text
global _start
_start:
    call _prompt
    call _read_user_input
    call _check_exit

    ; calculation and output

    jmp _start
    .exit:
    exit
