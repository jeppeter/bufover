
.code
;Add Proc
;	push rbp
;	mov rbp,rsp
;	mov rbx ,[rbp+8]
;	mov rcx ,[rbp+16]
;	mov rax,rbx
;	add rax,rcx	
;	mov rsp,rbp
;	pop rbp
;Add endp

; procedure cpu_rdtsc
; Signature: void cpu_rdtsc(uint64_t *result)
Proc
	push	rdx
	rdtsc
	mov	[rcx],	eax
	mov	[rcx+4],	edx
	pop	rdx
	ret
endp

End