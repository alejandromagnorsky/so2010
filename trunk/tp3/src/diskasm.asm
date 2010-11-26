getStatusRegister:
    mov dx, 1F7h
    in al, dx
    xor ah,ah
    ret

getDataRegister:
    mov dx, 1F0h
    in ax, dx
    ret

identifyDevice:

    cli

    mov dx, 1F6h
    mov al, 0
    out dx, al

    mov dx, 1f7h
    mov al, 0ECh
    out dx, al
    xor ah,ah

    sti

    ret


getErrorRegister:
    mov dx, 1f1h
    xor ax,ax
    in al, dx
    ret


sendDMAcomm:
    
    cli

    xor ah,ah

    mov dx, 1f2h    ; sector count register
    mov al, 1       ; QUEREMOS LEER 1 SECTOR
    out dx, al    	; setea el sector count register en 1

    ; SETEO LBA EN MADAFAKING 0, 
    ; ESTO ES LO MISMO QUE DECIR DISCO[0]
    ;Low
    mov dx, 1f3h
    mov al, 0
    out dx, al

    ;Mid
    mov dx, 1f4h
    mov al, 0
    out dx, al

    ;High
    mov dx, 1f5h
    mov al, 0
    out dx, al

    ; Device register, le meto el bit de "USO LBA" (6to) en 1 (los primeros 4 bits siguen siendo LBA, son 0)
    mov dx, 1f6h
    mov al, 40h
    out dx, al


    ; Ahora mando el comando
    mov dx, 1f7h
    mov al, 0C8h     ; el codigo del comando READ DMA
    out dx, al

    ; yay! mande el comando.

    nop
    nop
    nop
    ; me chupa la pija

    sti


    ret
