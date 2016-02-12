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
	jmp rax
	jmp rcx
	jmp rdx
	jmp rbx
	jmp rsp
	jmp rbp
	jmp rsi
	jmp rdi
	jmp r8
	jmp r9
	jmp r10
	jmp r11
	jmp r12
	jmp r13
	jmp r14
	jmp r15	
	mov rdx,01234567812345678h
	mov r8,01234567812345678h
	mov r8,12345678h
	mov r9,12345678h
	mov rcx,01234567812345678h
	jmp qword ptr [rax+100h]
	jmp qword ptr [rax+1234567712345678h]
	jmp qword ptr [rax+rbx]
	jmp qword ptr [rax+rbx+12h]
	jmp qword ptr [rax+4*rbx+12h]
	jmp qword ptr [rax+8*rbx+12h]
	jmp qword ptr [rax+r8]
	jmp qword ptr [rbx+r9]
	jmp qword ptr [rbx+r9+12345678h]
	jmp qword ptr [rax+rcx]
	jmp qword ptr [rbx+rcx]
	jmp qword ptr [rcx+100h]
	jmp qword ptr [rdx+100h]
	jmp qword ptr [rbx+100h]
	jmp qword ptr [$+6]
	mov rax,qword ptr [$+7]
	mov rax,rbx
	mov r8,[r9+r10]
	mov r8,[rcx+rdx]
	mov r8,[rcx+r10]
	mov r8,[r9+rdx]
	mov [rcx+rdx],r8
	mov rax,[rcx+rdx]
	mov [r9+r10],r8
	mov [rcx+rdx],rax
	mov [r13+r14*4],r11
	mov r11,[r13+r14*4]
	mov [r13+r14*4+16h],r11
	mov [r13],r11
	mov r11,011h
	ret
JMPRAX ENDP


End
