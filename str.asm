section .data
	fmt db "%d",10
extern printf

section .text
	global main

main:
	push rbp
	xor rax,rax
	sub rsp,0x128
	xor rdi,rdi
	mov rsi, rsp
	mov rdx,0x128
	syscall
	mov BYTE [rsp+rax],0x0
	mov rdi,rsp
	call strlen
        mov rdi,fmt
	mov rsi,rax
	xor rax,rax
	call printf
	add rsp,0x128
	pop rbp
	mov rax,0
	ret



strlen:
	push rbp
	mov rbp,rsp
	push rbx
	xor rcx,rcx
	_loop:
	   movzx rbx, BYTE [rdi+rcx]
	   test rbx,rbx
	   jz _end
	   add rcx,1
	   jmp _loop
	_end:
      mov rax,rcx
	    pop rbx
      pop rbp
      ret
