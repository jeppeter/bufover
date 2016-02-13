.code
IntAdd Proc
	push rbp
	mov rbp,rsp
	xor rax,rax
	mov eax,edx
	add eax,ecx	
	mov rsp,rbp
	pop rbp
	ret
IntAdd endp
	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;  parameter for decode buffer 
;  ecx for pinbuf
;  edx for inlen
;  r8  for poutbuf
;  r9  for outlen
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
DecodeBuffer Proc
	push rsi
	push rdi
	push rbx
	push rcx
	push rdx
	xor rsi,rsi
	xor rdi,rdi
	xor rbx,rbx	
	xor rax,rax
	mov esi,ecx
	mov rdi,r8
	mov ecx,edx
	xor rdx,rdx
decode_again:
	mov al,byte ptr[rsi]
	cmp al,01h
	jne normal_char
	inc rsi
	dec rcx
	mov al,byte ptr[rsi]
	cmp al,01h
	jnz set_0
	mov byte ptr[rdi],0ffh
	jmp inc_byte
set_0:
	mov byte ptr[rdi],bl
	jmp inc_byte
normal_char:
	dec al
	mov byte ptr[rdi],al
inc_byte:
	inc rdi
	inc rsi
	inc rdx
	dec rcx
	cmp ecx,ebx
	jnz decode_again
	mov rax,rdx
	pop rdx
	pop rcx
	pop rbx
	pop rdi
	pop rsi
	ret
DecodeBuffer endp

GetBaseAddr Proc
	push rdi
	lea rax,[$]
	xor rbx,rbx
	xor bl,bl
	mov bl,01h
	shl rbx,08h
	mov bl,02h
	shl rbx,08h
	add rax,rbx
	;mov rax,rbx
	pop rdi
	ret
GetBaseAddr endp

MovRax Proc
	mov rax,1
	ret
MovRax endp

public CallJmpoline
CallJmpoline PROC
	push rax
	mov rax,rcx
	push rax
	mov rax,qword ptr[rsp+8]
	ret 8h
CallJmpoline ENDP

ShellCodeForCall PROC
	push rax
	mov rax,1234567812345678h
	push rax
	mov rax,qword ptr[rsp+8]
	ret 8h
ShellCodeForCall ENDP

JMPRAX PROC
	lea rax,[$+7]
	lea rcx,[$+7]
	lea rdx,[$+7]
	lea rbx,[$+7]
	lea rsp,[$+7]
	lea rbp,[$+7]
	lea rsi,[$+7]
	lea rdi,[$+7]
	mov rax,01234567812345678h
	mov rcx,01234567812345678h
	mov rdx,01234567812345678h
	mov rbx,01234567812345678h
	mov rsp,01234567812345678h
	mov rbp,01234567812345678h
	mov rsi,01234567812345678h
	mov rdi,01234567812345678h
	lea r8,[$+7]
	lea r9,[$+7]
	lea r10,[$+7]
	lea r11,[$+7]
	lea r12,[$+7]
	lea r13,[$+7]
	lea r14,[$+7]
	lea r15,[$+7]

	mov r8,01234567812345678h
	mov r9,01234567812345678h
	mov r10,01234567812345678h
	mov r11,01234567812345678h
	mov r12,01234567812345678h
	mov r13,01234567812345678h
	mov r14,01234567812345678h
	mov r15,01234567812345678h



	lea rax,[rcx+rdx+8]
	mov [rcx+rdx+8],rax
	mov rax,[rcx+rdx+8]
	mov rax,[r9+rdx+8]
	mov rax,[r9+r10+8]
	mov r8,[r9+r10+8]
	mov rax,[rcx+r10+8]
	mov rax,[rcx*8+r10+8]
	mov rax,[rcx+r10*4+8]
	xor rax,[rcx+rdx+8]
	xor [rcx+rdx+8],rax
	xor r8,r9
	xor rax,r9
	xor rax,rcx
	xor eax,ecx
	xor ax,cx
	xor al,cl
	xor ah,ch
	xor eax,edx
	ret
JMPRAX ENDP


End
