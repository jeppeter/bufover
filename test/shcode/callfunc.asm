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
End
