
        global  set_bit_elem
        global  get_bit_elem
        section .text

set_bit_elem:
        push rbp            ; save the base pointer on the stack (at rsp+8)
        mov rbp, rsp        ; set up the rbp for the bottom of this frame

        ; rdi contains array pointer
        ; rsi contains row width
        ; rdx contains row
        ; rcx contains col

        imul rdx, rsi       ; multiply row*row_width
        add  rdx, rcx       ; add (row*row_width) + col
        mov  rax, rdx       ; move into rax register, index is still in rdx

        sar rax, 3          ; divides rax by 8 (2^3 = 8), this is the byte_offset
        mov rbx, rax        ; move byte_offset into rbx
        sal rax, 3          ; multiplies rax by 8 (2^3 = 8)
        sub rdx, rax        ; bit_offset = index - (byte_offset*8)

        mov rcx, 8          ; move 8 into rcx
        sub rcx, rdx        ; from 8 subtract bit_offset
        mov r8, 1           ; move 1 into rbx
        sub rcx, r8         ; from 8 - bit_offset, subtract 1

        cmp rcx, 0          ; rcx is 8 - byte_offset - 1
        jng .done
    .loop:
        sal r8, 1           ; rbx is 1 to start. mask = 1 << 8 - byte_offset - 1
        dec rcx             ; decrement mask counter
        cmp rcx, 0          ; compare mask counter after decrement
        jg .loop
    .done:                  ; mask is now in rbx

        or  rdi, r8         ; set bit using array pointer?
        mov rax, rdi

        mov rsp, rbp        ; restore stack pointer to before we pushed parameters onto the stack
        pop rbp             ; remove rbp from the stack to restore rsp to initial value
        ret                 ; return value in rax




get_bit_elem:
        push rbp            ; save the base pointer on the stack (at rsp+8)
        mov rbp, rsp        ; set up the rbp for the bottom of this frame

        ; rdi contains array pointer
        ; rsi contains row width
        ; rdx contains row
        ; rcx contains col

        imul rdx, rsi       ; multiply row*row_width
        add  rdx, rcx       ; add (row*row_width) + col
        mov  rax, rdx       ; move into rax register, index is still in rdx

        sar rax, 3          ; divides rax by 8 (2^3 = 8), this is the byte_offset
        mov rbx, rax        ; move byte_offset into rbx
        sal rax, 3          ; multiplies rax by 8 (2^3 = 8)
        sub rdx, rax        ; bit_offset = index - (byte_offset*8)

        mov rcx, 8          ; move 8 into rcx
        sub rcx, rdx        ; from 8 subtract bit_offset
        mov rbx, 1          ; move 1 into rbx
        sub rcx, rbx        ; from 8 - bit_offset, subtract 1

        cmp rcx, 0          ; rcx is 8 - byte_offset - 1
        jng .done
    .loop:
        sal rbx, 1          ; rbx is 1 to start. mask = 1 << 8 - byte_offset - 1
        dec rcx             ; decrement mask counter
        cmp rcx, 0          ; compare mask counter after decrement
        jg .loop
    .done:                  ; mask is now in rbx

        and rdi, rbx        ; get bit
        cmp rdi, 0
        setg al
        movsx rax, al

        mov rsp, rbp        ; restore stack pointer to before we pushed parameters onto the stack
        pop rbp             ; remove rbp from the stack to restore rsp to initial value
        ret                 ; return value in rax
