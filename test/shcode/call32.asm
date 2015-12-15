

.686p
.model flat
.code
IntAdd Proc
	push ebp
	mov ebp,esp
	xor eax,eax
	mov eax,edx
	add eax,ecx	
	mov esp,ebp
	pop ebp
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
	push esi
	push edi
	push ebx
	push ecx
	push edx
	xor esi,esi
	xor edi,edi
	xor ebx,ebx	
	xor eax,eax
	mov esi,ecx
	;mov edi,r8  ; this will modify
	mov ecx,edx
	xor edx,edx
decode_again:
	mov al,byte ptr[esi]
	cmp al,01h
	jne normal_char
	inc esi
	dec ecx
	mov al,byte ptr[esi]
	cmp al,01h
	jnz set_0
	mov byte ptr[edi],0ffh
	jmp inc_byte
set_0:
	mov byte ptr[edi],bl
	jmp inc_byte
normal_char:
	dec al
	mov byte ptr[edi],al
inc_byte:
	inc edi
	inc esi
	inc edx
	dec ecx
	cmp ecx,ebx
	jnz decode_again
	mov eax,edx
	pop edx
	pop ecx
	pop ebx
	pop edi
	pop esi
	ret
DecodeBuffer endp

GetBaseAddr Proc
	push edi
	lea eax,[$]
	xor ebx,ebx
	xor bl,bl
	mov bl,01h
	shl ebx,08h
	mov bl,02h
	shl ebx,08h
	add eax,ebx
	;mov eax,ebx
	pop edi
	ret
GetBaseAddr endp

MovRax Proc
	mov eax,1
	ret
MovRax endp

End
