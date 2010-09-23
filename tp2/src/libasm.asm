GLOBAL  _read_msw,_lidt
GLOBAL  _int_20_hand, _int_21_hand, _int_80_hand,
GLOBAL	_int_00_hand, _int_01_hand, _int_02_hand, _int_03_hand
GLOBAL	_int_04_hand, _int_05_hand, _int_06_hand, _int_07_hand
GLOBAL	_int_08_hand, _int_09_hand, _int_0A_hand, _int_0B_hand
GLOBAL	_int_0C_hand, _int_0D_hand, _int_0E_hand, _int_0F_hand
GLOBAL	_int_10_hand, _int_11_hand, _int_12_hand, _int_13_hand
GLOBAL	_int_14_hand, _int_15_hand, _int_16_hand, _int_17_hand
GLOBAL	_int_18_hand, _int_19_hand, _int_1A_hand, _int_1B_hand
GLOBAL	_int_1C_hand, _int_1D_hand, _int_1E_hand, _int_1F_hand
GLOBAL  _mascaraPIC1,_mascaraPIC2,_Cli,_Sti
GLOBAL	_read_cr0, _setPG
GLOBAL  _debug

EXTERN  int_20, int_21, int_80, int_00, fault_handler

SECTION .text


_Cli:
	cli			; limpia flag de interrupciones
	ret

_Sti:
	sti			; habilita interrupciones por flag
	ret

_mascaraPIC1:			; Escribe mascara del PIC 1
	push    ebp
        mov     ebp, esp
        mov     ax, [ss:ebp+8]  ; ax = mascara de 16 bits
        out	21h,al
        pop     ebp
        retn

_mascaraPIC2:			; Escribe mascara del PIC 2
	push    ebp
        mov     ebp, esp
        mov     ax, [ss:ebp+8]  ; ax = mascara de 16 bits
        out	0A1h,al
        pop     ebp
        retn

_read_msw:
        smsw    ax		; Obtiene la Machine Status Word
        retn


_lidt:				; Carga el IDTR
        push    ebp
        mov     ebp, esp
        push    ebx
        mov     ebx, [ss: ebp + 6] ; ds:bx = puntero a IDTR 
	rol	ebx,16		    	
	lidt    [ds: ebx]          ; carga IDTR
        pop     ebx
        pop     ebp
        retn


_int_00_hand:
    push byte 0
    push byte 0
    jmp isr_common_stub
    
_int_01_hand:
    push byte 0
    push byte 1
    jmp isr_common_stub
    
_int_02_hand:
    push byte 0
    push byte 2
    jmp isr_common_stub
    
_int_03_hand:
    push byte 0
    push byte 3
    jmp isr_common_stub
    
_int_04_hand:
    push byte 0
    push byte 4
    jmp isr_common_stub
    
_int_05_hand:
    push byte 0
    push byte 5
    jmp isr_common_stub  
    
_int_06_hand:
    push byte 0
    push byte 6
    jmp isr_common_stub   
    
_int_07_hand:
    push byte 0
    push byte 7
    jmp isr_common_stub
    
_int_08_hand:
    push byte 0
    push byte 8
    jmp isr_common_stub  
    
_int_09_hand:
    push byte 0
    push byte 9
    jmp isr_common_stub  
    
_int_0A_hand:
    push byte 0
    push byte 10
    jmp isr_common_stub  
    
_int_0B_hand:
    push byte 0
    push byte 11
    jmp isr_common_stub  
    
_int_0C_hand:
    push byte 0
    push byte 12
    jmp isr_common_stub   
    
_int_0D_hand:
    push byte 0
    push byte 13
    jmp isr_common_stub
    
_int_0E_hand:
    push byte 0
    push byte 14
    jmp isr_common_stub 
    
_int_0F_hand:
    push byte 0
    push byte 15
    jmp isr_common_stub 
    
_int_10_hand:
    push byte 0
    push byte 16
    jmp isr_common_stub   
    
_int_11_hand:
    push byte 0
    push byte 17
    jmp isr_common_stub  
    
_int_12_hand:
    push byte 0
    push byte 18
    jmp isr_common_stub
    
_int_13_hand:
    push byte 0
    push byte 19
    jmp isr_common_stub 
    
_int_14_hand:
    push byte 0
    push byte 20
    jmp isr_common_stub 
    
_int_15_hand:
    push byte 0
    push byte 21
    jmp isr_common_stub
    
_int_16_hand:
    push byte 0
    push byte 22
    jmp isr_common_stub 
    
_int_17_hand:
    push byte 0
    push byte 23
    jmp isr_common_stub
    
_int_18_hand:
    push byte 0
    push byte 24
    jmp isr_common_stub 
    
_int_19_hand:
    push byte 0
    push byte 25
    jmp isr_common_stub
    
_int_1A_hand:
    push byte 0
    push byte 26
    jmp isr_common_stub
    
_int_1B_hand:
    push byte 0
    push byte 27
    jmp isr_common_stub 
    
_int_1C_hand:
    push byte 0
    push byte 28
    jmp isr_common_stub
    
_int_1D_hand:
    push byte 0
    push byte 29
    jmp isr_common_stub
    
_int_1E_hand:
    push byte 0
    push byte 30
    jmp isr_common_stub
    
_int_1F_hand:
    push byte 0
    push byte 31
    jmp isr_common_stub     
    
_int_20_hand:				; Handler de INT 20 ( Timer tick)
        push    ds
        push    es                      ; Se salvan los registros
        pusha                           ; Carga de DS y ES con el valor del selector
        mov     ax, 10h			; a utilizar.
        mov     ds, ax
        mov     es, ax                  
        call    int_20                 
        mov	al,20h			; Envio de EOI generico al PIC
	out	20h,al
	popa                            
        pop     es
        pop     ds
        iret

_int_21_hand:
	cli
	push	ds
	push	es
	pusha
	
	mov	ax, 10h
	mov	ds, ax
	mov	es, ax

	in al, 60h
	call	int_21
	
	mov	al,20h
	out	20h,al
	
	popa
	pop	es
	pop	ds
	sti
	iret
	
_int_80_hand:
        cli
        push    ds
        push    es
        push    sp
        push    bp
        push    si
        push    di
        
        push    ax      ; DS and ES need to be loaded without losing AX
        mov     ax, 10h	
        mov     ds, ax
        mov     es, ax
        pop     ax
        
        call    int_80
        
        pop     di
	pop     si
	pop     bp
        pop     sp                    
        pop     es
        pop     ds
        sti
        iret
	
isr_common_stub:
	pusha
	push ds
	push es
	push fs
	push gs
	mov ax, 0x10   ; Load the Kernel Data Segment descriptor!
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov eax, esp   ; Push us the stack
	push eax
	mov eax, fault_handler
	call eax       ; A special call, preserves the 'eip' register
	pop eax
	pop gs
	pop fs
	pop es
	pop ds
	popa
	add esp, 8     ; Cleans up the pushed error code and pushed ISR number
	iret           ; pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP!
	
;-------------------------------------------------------------------------------
;	Functions for enabling paging

_read_cr0:
	mov 	eax, cr0
	retn

_setPG:
	push	ebp
	mov		ebp, esp
	mov		eax, [ebp+8]
	mov 	cr0, eax
	mov	esp,ebp
	pop	ebp
	retn

;-------------------------------------------------------------------------------


; Debug para el BOCHS, detiene la ejecució; Para continuar colocar en el BOCHSDBG: set $eax=0
;


_debug:
        push    bp
        mov     bp, sp
        push	ax
vuelve:	mov     ax, 1
        cmp	ax, 0
	jne	vuelve
	pop	ax
	pop     bp
        retn
