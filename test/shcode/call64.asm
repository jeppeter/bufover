
.code
IntAdd Proc
	push rbp
	mov rbp,rsp
	mov rbx ,[rbp+8]
	mov rcx ,[rbp+16]
	mov rax,rbx
	add rax,rcx	
	mov rsp,rbp
	pop rbp
IntAdd endp
End


