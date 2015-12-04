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

InputParam Proc
	; this is for the input pinbuffer
	xor rax,rax
	xor al,al
	mov al,011h
	shl rax,08h
	mov al,022h
	shl rax,08h
	mov al,033h
	shl rax,08h
	mov al,011h
	shl rax,08h
	mov al,022h
	shl rax,08h
	mov al,033h
	shl rax,08h
	mov al,044h
	shl rax,08h
	mov al,044h
	mov rcx,rax


	; this is for inlen
	xor rax,rax
	xor al,al
	mov al,011h
	shl rax,08h
	mov al,022h
	shl rax,08h
	mov al,033h
	shl rax,08h
	mov al,011h
	shl rax,08h
	mov al,022h
	shl rax,08h
	mov al,033h
	shl rax,08h
	mov al,044h
	shl rax,08h
	mov al,044h
	mov rdx,rax

	; this is for poutbuf
	xor rax,rax
	xor al,al
	mov al,011h
	shl rax,08h
	mov al,022h
	shl rax,08h
	mov al,033h
	shl rax,08h
	mov al,011h
	shl rax,08h
	mov al,022h
	shl rax,08h
	mov al,033h
	shl rax,08h
	mov al,044h
	shl rax,08h
	mov al,044h
	mov r8,rax

	; this is for outlen
	xor rax,rax
	xor al,al
	mov al,011h
	shl rax,08h
	mov al,022h
	shl rax,08h
	mov al,033h
	shl rax,08h
	mov al,011h
	shl rax,08h
	mov al,022h
	shl rax,08h
	mov al,033h
	shl rax,08h
	mov al,044h
	shl rax,08h
	mov al,044h
	mov r9,rax




InputParam endp

End
