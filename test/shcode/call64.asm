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

End
