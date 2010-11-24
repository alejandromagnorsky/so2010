GLOBAL  _read_msw,_lidt
GLOBAL  _int_20_hand, _int_21_hand, _int_80_hand, _int_96_hand
GLOBAL	_int_00_hand, _int_01_hand, _int_02_hand, _int_03_hand
GLOBAL	_int_04_hand, _int_05_hand, _int_06_hand, _int_07_hand
GLOBAL	_int_08_hand, _int_09_hand, _int_0A_hand, _int_0B_hand
GLOBAL	_int_0C_hand, _int_0D_hand, _int_0E_hand, _int_0F_hand
GLOBAL	_int_10_hand, _int_11_hand, _int_12_hand, _int_13_hand
GLOBAL	_int_14_hand, _int_15_hand, _int_16_hand, _int_17_hand
GLOBAL	_int_18_hand, _int_19_hand, _int_1A_hand, _int_1B_hand
GLOBAL	_int_1C_hand, _int_1D_hand, _int_1E_hand, _int_1F_hand
GLOBAL  _mascaraPIC1,_mascaraPIC2,_Cli,_Sti
GLOBAL  phys_copy, port_in, port_out, _portw_out, _portw_in, _lock, _unlock
GLOBAL	_task_load_state_, _task_save_state_
GLOBAL	_newStack, _scheduler
GLOBAL  _debug

EXTERN  int_20, int_21, int_80, int_00, int_96, fault_handler, _task_scheduler

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


; _lidt
; Loads IDTR with a value sent as a parameter.

_lidt:

    ; Stack frame is built.
    push        EBP
    mov        EBP, ESP

    ; Registers are backed-up.
    push        EBX

    ; IDTR pointer is loaded.
    mov        EBX, [SS: EBP+8]

    ; IDTR is loaded.
    lidt        [DS: EBX]

    ; Backed-up registers are loaded.
    pop        EBX

    ; Stack frame is destroyed.
    mov        ESP, EBP
    pop        EBP

    retn


_int_00_hand:
    mov [0xB8000], dword 65
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
    
_int_20_hand: ; Timer tick interruption handler
    cli
    pushad

	    call	int_20

        mov     ebx, esp
        
        push    ebx
    	    call _task_scheduler
        pop     ebx

        mov esp, eax ; STACK SWITCH!
	    
	    mov		al, 20h			; PIC ACK
	    out		20h, al
	
	popad
	sti
	iret
	
nada:
    push eax
    call nadanada
    pop eax
    ret

nadanada:
    push eax
    call nadanadanada
    pop eax
    ret
    
nadanadanada:
    push eax
    pop eax
    ret
    
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
	

_int_96_hand:
	cli
	push	ds
	push	es
	pusha
	
	mov	ax, 10h
	mov	ds, ax
	mov	es, ax

	call	int_96
	
	mov	al,20h
	out	20h,al
	
	popa
	pop	es
	pop	ds
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
; Functions for context changing

_scheduler:						; Changing a process;
	push ebp
	mov ebp, esp
	int 0x20
	mov esp, ebp
	pop ebp
	ret

;-------------------------------------------------------------------------------
;	Creating new stack

_newStack:
    push ebp
    mov ebp, esp
    
            mov eax, [ebp + 12] ; Pointer to the bottom of the new stack.
            mov esp, eax        ; We switch to the new stack

            mov eax, [ebp + 16] ; This is the char* line argument
            push eax            ; Pushed so it can be read.
            
            mov eax, [ebp + 20] ; Function to clean-up after task is done.
            push eax            ; To the new stack it goes!
            
            mov eax, 512        ; New flags register, to be iret'd
            push eax            ; There we go.                       
            
            push cs             ; Code segment.
            
            mov eax, [ebp + 8]  ; Function for the new task to run.
            push eax            ; To the new stack it goes as well!
            
            pushad              ; Trash. Won't matter.
            mov eax, esp        ; Return the moved stack pointer.
            
    mov esp, ebp
    pop ebp
    ret
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



;===========================================================================
;				phys_copy
;===========================================================================
; This routine copies a	block of physical memory.  It is called	by:
;    phys_copy(	(long) source, (long) destination, (long) bytecount)

phys_copy:
	pushf			; save flags
	cli				; disable interrupts
	push bp			; save the registers
	push ax			; save ax
	push bx			; save bx
	push cx			; save cx
	push dx			; save dx
	push si			; save si
	push di			; save di
	push ds			; save ds
	push es			; save es
	mov bp,sp		; set bp to point to saved es

  L0:	mov ax,[bp + 28]		; ax = high-order word of 32-bit destination
	mov di,[bp + 26]			; di = low-order word of 32-bit	destination
	mov cx,4				; start	extracting click number	from dest
  L1:	rcr ax,1		; click	number is destination address /	16
	rcr di,1		; it is	used in	segment	register for copy
	loop L1			; 4 bits of high-order word are	used
	mov es,di		; es = destination click

	mov ax,[bp + 24]		; ax = high-order word of 32-bit source
	mov si,[bp + 22]		; si = low-order word of 32-bit	source
	mov cx,4		; start	extracting click number	from source
  L2:	rcr ax,1		; click	number is source address / 16
	rcr si,1		; it is	used in	segment	register for copy
	loop L2			; 4 bits of high-order word are	used
	mov ds,si		; ds = source click

	mov di,[bp + 26]		; di = low-order word of dest address
	and di,000Fh		; di = offset from paragraph  in es
	mov si,[bp + 22]		; si = low-order word of source	address
	and si,000Fh		; si = offset from paragraph  in ds

	mov dx,[bp + 32]		; dx = high-order word of byte count
	mov cx,[bp + 30]		; cx = low-order word of byte count

	test cx,8000h		; if bytes >= 32768, only do 32768
	jnz L3			; per iteration
	test dx,0FFFFh		; check	high-order 17 bits to see if bytes
	jnz L3			; if bytes >= 32768 then go to L3
	jmp short L4		; if bytes < 32768 then	go to L4
  L3:	mov cx,8000h		; 0x8000 is unsigned 32768
  L4:	mov ax,cx		; save actual count used in ax;	needed later

	test cx,0001h		; should we copy a byte	or a word at a time?
	jz L5			; jump if even
	rep movsb		; copy 1 byte at a time
	jmp short L6		; check	for more bytes

  L5:	shr cx,1		; word copy
	rep movsw		; copy 1 word at a time

  L6:	mov dx,[bp + 32]		; decr count, incr src & dst, iterate if needed
	mov cx,[bp + 30]		; dx ;;	cx is 32-bit byte count
	xor bx,bx		; bx ;;	ax is 32-bit actual count used
	sub cx,ax		; compute bytes	- actual count
	sbb dx,bx		; dx ;;	cx is  bytes not yet processed
	or cx,cx		; see if it is 0
	jnz L7			; if more bytes	then go	to L7
	or dx,dx		; keep testing
	jnz L7			; if loop done,	fall through

	pop es			; restore all the saved	registers
	pop ds			; restore ds
	pop di			; restore di
	pop si			; restore si
	pop dx			; restore dx
	pop cx			; restore cx
	pop bx			; restore bx
	pop ax			; restore ax
	pop bp			; restore bp
	popf			; restore flags
	ret			; return to caller

L7:	mov [bp + 32],dx		; store	decremented byte count back in mem
	mov [bp + 30],cx		; as a long
	add [bp + 26],ax		; increment destination
	adc [bp + 28],bx		; carry	from low-order word
	add [bp + 22],ax		; increment source
	adc [bp + 24],bx		; carry	from low-order word
	jmp L0			; start	next iteration


;===========================================================================
;				port_out
;===========================================================================
; port_out(port, value)	writes 'value' on the I/O port 'port'.

port_out:
	push bx			; save bx
	mov bx,sp		; index	off bx
	push ax			; save ax
	push dx			; save dx
	mov dx,[bx+4]	; dx = port
	mov ax,[bx+6]	; ax = value
	out dx,al		; output 1 byte
	pop dx			; restore_lock dx
	pop ax			; restore ax
	pop bx			; restore bx
	ret				; return to caller

;===========================================================================
;				port_in
;===========================================================================
; port_in(port,	&value)	reads from port	'port' and puts	the result in 'value'.
port_in:
	push bx			; save bx
	mov bx,sp		; index	off bx
	push ax			; save ax
	push dx			; save dx
	mov dx,[bx+4]		; dx = port
	in  al,dx		; input	1 byte
	xor ah,ah		; clear	ah
	mov bx,[bx+6]	; fetch	address	where byte is to go
	mov [bx],ax		; return byte to caller	in param
	pop dx			; restore dx
	pop ax			; restore ax
	pop bx			; restore bx
	ret				; return to caller


;*===========================================================================*
;*				portw_out				     *
;*===========================================================================*
; portw_out(port, value) writes 'value' on the I/O port 'port'.

_portw_out:
	push bx			; save bx
	mov bx,sp		; index off bx
	push ax			; save ax
	push dx			; save dx
	mov dx,[bx+4]	; dx = port
	mov ax,[bx+6]	; ax = value
	out	dx,ax			; output 1 word
	pop dx			; restore dx
	pop ax			; restore ax
	pop bx			; restore bx
	ret			; return to caller


;*===========================================================================*
;*				portw_in				     *
;*===========================================================================*
; portw_in(port, &value) reads from port 'port' and puts the result in 'value'.
_portw_in:
	push bx			; save bx
	mov bx,sp		; index off bx
	push ax			; save ax
	push dx			; save dx
	mov dx,[bx+4]	; dx = port
	in ax,dx		; input 1 word
	mov bx,[bx+6]	; fetch address where byte is to go
	mov [bx],ax		; return byte to caller in param
	pop dx			; restore dx
	pop ax			; restore ax
	pop bx			; restore bx
	ret			; return to caller



;===========================================================================
;				lock
;===========================================================================
; Disable CPU interrupts.
_lock:
	cli				; disable interrupts
	ret				; return to caller


;===========================================================================
;				unlock
;===========================================================================
; Enable CPU interrupts.
_unlock:
	sti			; enable interrupts
	ret			; return to caller

