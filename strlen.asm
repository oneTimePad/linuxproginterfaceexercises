
section .data
  fmt db "%d"

global _start
extern printf
section .text

_start:
  sub esp,0x128
  xor eax,eax
  mov ebx,1
  mov edx,0x128
  lea ecx, [esp-0x128]
  int 0x80
  push eax
  call _strlen
  push eax
  push fmt
  call printf
  mov eax,60
  int 0x80


_strlen:
  push ebp
  mov ebp,esp
  mov eax, [ebp+0x8]
  xor ecx,ecx
  _loop:
    movzx ebx, BYTE  [eax+ecx]
    test ebx,ebx
    jz _end
    add ecx,1
    jmp _loop
  _end:
    mov eax,ecx
    mov esp,ebp
    pop ebp
    ret
