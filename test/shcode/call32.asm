
.686P

.code

Add PROC
	push ebp
	mov ebp,esp
	mov ebx,[ebp+4]
	mov ecx,[ebp+8]
	mov eax,ebx
	add eax,ecx
	mov esp,ebp
	pop ebp
Add ENDP
End