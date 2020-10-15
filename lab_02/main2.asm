.386p

descr struc
	limit 	dw 0
	base_l 	dw 0
	base_m 	db 0
	attr_1 	db 0
	arrt_2 	db 0
	base_h 	db 0
descr ends

int_descr struc
	offs_l 	dw 0
	sel		dw 0
	counter db 0
	attr	db 0
	offs_h 	dw 0
int_descr ends

pm_seg segment para public 'code' use32
	assume cs:pm_seg

gdt label byte
	gdt_null descr <>
	gdt_flatDS  descr <0ffffh, 0, 0, 92h, 11001111b, 0>
	gdt_16bitCS descr <rm_seg_size-1, 0, 0, 98h, 0, 0>
	gdt_32bitCS descr <pm_seg_size-1, 0, 0, 98h, 11001111b, 0>
	gdt_32bitDS descr <pm_seg_size-1, 0, 0, 92h, 11001111b, 0>
	gdt_32bitSS descr <stack_size-1, 0, 0, 92h, 11001111b, 0>
	gdt_size = $-gdt
	gdtr dw gdt_size-1
	dd ?

	sel_flatDS  equ 001000b
	sel_16bitCS equ 010000b
	sel_32bitCS equ 011000b
	sel_32bitDS equ 100000b
	sel_32bitSS equ 101000b

idt label byte
	int_descr_1 int_descr 13 dup (<0, SEL_32bitCS, 0, 8Fh, 0>)
	int13 int_descr <0, SEL_32bitCS, 8Fh, 0>
	int_descr_2 int_descr 18 dup (<0, SEL_32bitCS, 8Fh, 0>)
	irq0 int_descr <0, SEL_32bitCS,0, 8Eh, 0>
	irq1 int_descr	<0, SEL_32bitCS,0, 8Eh, 0>
	idt_size = $-idt

	idtr dw idt_size-1
	dd ?

	idtr_real dw 3ffh, 0, 0

	v86_msg db "Процессор в режиме V86 - нельзя переключится в PM$"
	win_msg db "Программа запущена под Windows перейти в кольцо 0$"

	scan2ascii	db 0, 1bh, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 8
				db 0, 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '{', '}'
				db 0, 0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ':', "'", 0, 0
				db 'a', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0
				db 0, 0, 0, 0 ,' '
	time_08 dd 0
	screen_addr dd 320
	master db 0
	slave db 0

pm_entry:
	mov ax, sel_flatDS
	mov ds, ax
	mov es, ax
	mov ax, sel_32bitSS
	mov ebx, stack_size
	mov ss, ax
	mov esp, ebx

	sti

	call print_memory
	jmp short $

dummy proc
	mov ax, 1h
	jmp esc_pressed
dummy endp

dummy13 proc
	mov ax, 0dh
	jmp esc_pressed
dummy13 endp

; Таймер
irq0_handler:
	push eax
	push ebp
	push ecx
	push dx
	mov eax,time_08

	mov ebp,0B8010h
	mov ecx,8
	prcyc:
	mov dl,al
	and dl,0Fh
	cmp dl,10
	jl number
	add dl, 'A' - '0' - 10
	number:
	add dl,'0'
	mov es:[ebp],dl
	sub ebp,2
	ror eax,4
	loop prcyc

	inc eax
	mov time_08,eax

	pop dx
	pop ecx
	pop ebp

	mov	al,20h
	out	20h,al
	pop eax

	iretd

; Прерывания от клавиатуры
irq1_handler:
	push eax
	push ebx
	push es
	push ds
	in al, 60h
	cmp al, 39h
	ja skip_translate
	cmp al, 1
	je esc_pressed
	cmp al, 1
	je esc_pressed
	mov bx, sel_32bitDS
	mov ds, bx
	mov ebx, offset scan2ascii
	xlatb
	mov bx, sel_flatDS
	mov es, bx
	mov ebx, screen_addr
	cmp al, 8
	je bs_pressed

	mov es:[ebx+0b8000h], al
	add dword ptr screen_addr, 2
	jmp short skip_translate

bs_pressed:
	mov al, ' '
	sub ebx, 2
	mov es:[ebx+0b8000h],al
	mov screen_addr, ebx

skip_translate:
	in al, 61h
	or al, 80h
	out 61h, al

	mov al, 20h
	out 20h, al

	pop ds
	pop es
	pop ebx
	pop eax
	iretd

esc_pressed:
	in al, 61h
	or al, 80h
	out 61h, al
	mov al, 20h
	out 20h, al
	pop ds
	pop es
	pop ebx
	pop eax
	cli
		db 0eah
		dd offset rm_return
		dw sel_16bitCS

print_memory proc
	push ds
	mov	ax, sel_flatds
	mov	ds, ax
	mov	ebx, 100001h
	mov	dl,	10101010b

	mov	ecx, 0ffeffffeh

check:
	mov	dh, ds:[ebx]
	mov	ds:[ebx], dl
	cmp	ds:[ebx], dl
	jnz	end_of_memory
	mov	ds:[ebx], dh
	inc	ebx
	loop	check

end_of_memory:
	pop	ds
	xor	edx, edx
	mov	eax, ebx
	mov	ebx, 100000h
	div	ebx

	push ebp
	mov ebp,20

	push ecx
	push dx

	mov ecx,8
	add ebp,0b8010h
	prcyc1:
		mov dl,al
		and dl,0fh

		cmp dl,10
		jl number1
		add dl,'A' - '0' - 10
		number1:
		add dl,'0'

		mov es:[ebp],dl
		ror eax,4
		sub ebp,2
	loop prcyc1

	sub ebp,0b8010h
	pop dx
	pop ecx

	pop ebp

	ret
print_memory endp

	pm_seg_size = $-gdt
pm_seg ends

rm_seg segment para public 'code' use16
	assume cs:rm_seg, ds:pm_seg, ss:stack_seg
start:
	; Очистка экрана
	mov ax, 3
	int 10h

	push pm_seg
	pop ds

	xor eax, eax
	mov ax, rm_seg
	shl eax, 4
	mov word ptr gdt_16bitCS+2, ax ; Базой 16bitCS будет rm_seg
	shr eax, 16
	mov byte ptr gdt_16bitCS+4, al
	mov ax, pm_seg
	shl eax, 4
	push eax
	push eax
	mov word ptr gdt_32bitCS+2, ax ; Базой всех 32bit будет pm_seg
	mov word ptr gdt_32bitSS+2, ax
	mov word ptr gdt_32bitDS+2, ax
	shr eax, 16
	mov byte ptr gdt_32bitCS+4, al
	mov byte ptr gdt_32bitSS+4, al
	mov byte ptr gdt_32bitDS+4, al

	pop eax
	add eax, offset gdt
	mov dword ptr gdtr+2, eax
	mov word ptr gdtr, gdt_size-1

	lgdt fword ptr gdtr

	pop eax
	add eax, offset idt
	mov dword ptr idtr+2, eax
	mov word ptr idtr, idt_size-1

	; Перепрограммировать контроллер прерываний
	mov eax, offset irq0_handler
	mov irq0.offs_l, ax
	shr eax, 16
	mov irq0.offs_h, ax
	mov eax, offset irq1_handler
	mov irq1.offs_l, ax
	shr eax, 16
	mov irq1.offs_h, ax

	mov eax, offset dummy
	mov int_descr_1.offs_l, ax
	mov int_descr_2.offs_l, ax
	shr eax, 16
	mov int_descr_1.offs_h, ax
	mov int_descr_2.offs_h, ax

	mov eax, offset dummy13
	mov int13.offs_l, ax
	shr eax, 16
	mov int13.offs_h, ax

	in al, 21h
	mov master, al
	in al, 0A1h
	mov slave, al

	mov al, 11h
	out 20h, al
	mov AL, 20h
	out 21h, al
	mov al, 4
	out 21h, al
	mov al, 1
	out 21h, al
	mov al, 0fch ; Игнорируем все прерывания, кроме первых двух
	out 21h, al

	mov al, 0ffh
	out 0a1h, al

	lidt fword ptr idtr

	; Открытие А20
	in al, 92h
	or al, 2
	out 92h, al

	cli

	in al, 70h
	or al, 80h
	out 70h, al

	; Переход в PM
	mov eax, cr0
	or al, 1
	mov cr0, eax

	db 66h
	db 0eah
	dd offset pm_entry
	dw sel_32bitCS

rm_return:
	; Переход в RM
	mov eax, cr0
	and al, 0feh
	mov cr0, eax

	db 0eah
	dw $+4
	dw rm_seg

	mov ax, pm_seg
	mov ds, ax
	mov es, ax
	mov ax, stack_seg
	mov bx, stack_size
	mov ss, ax
	mov sp, bx

	mov ax, pm_seg
	mov ds, ax

	mov al, master
	out 21h, al

	mov al, slave
	out 0a1h, al

	lidt fword ptr idtr_real

	in al, 70h
	and al, 07fh
	out 70h, al

	sti
	mov ah, 4ch
	int 21h

	rm_seg_size = $-start
rm_seg ends

stack_seg segment para stack 'stack'
stack_start db 100h dup(?)
stack_size = $-stack_start
stack_seg ends

end start
