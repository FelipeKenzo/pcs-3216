; LOADER (starts at $000)
            jp    start     ; Jumps data area to first instruction
; Data Area
counter:    con   $00       ; Counts number of bytes of data to be read
one:        con   $01       ; 1 constant

; Program start
start:      gd              ; Reads first address byte from file
            add   write_1   ; Adds upper address to write_1
            mm    write_1   ; Writes result to write_1
            sub   $90       ; Subtracts MM opcode
            jz    zero_1    ; If equals to zero, we jump to zero_1 and continue to read the address
            gd              ; Reads second byte from file
            mm    write_2   ; Writes to write_2
readSize:   gd              ; Reads byte count from file
            mm    counter   ; Writes byte count to counter
            jp    loop      ; Jumps to memory write loop.

; If first address byte is zero
zero_1:     gd              ; Reads second byte from file
            jz    zero_2    ; If also zero, we reached end of program, halt loader.
            mm    write_2   ; Writes to write_2
            jp    readSize  ; goes back to reading 

zero_2:     hm    $000      ; Halts machine, end of program.

; Memory write loop
loop:       gd              ; Reads a byte of data
write_1:    con   $90       ; MM first byte (will be read as instruction)
write_2:    con   $00       ; MM second byte (will be read as instruction)
            ld    write_2   ; Loads MM second byte (lower address byte)
            add   one       ; write_2 += 1
            jz    addUpper  ; jumps to addUpper (adds one to upper address byte)  
write_3:    ld    counter   ; Loads value from counter
            sub   one       ; Counter--
            mm    counter   ; Writes new value of counter
            jz    start     ; Goes back to start if counter = 0
            jp    loop      ; Goes back to loop if counter != 0

; Add one to upper MM byte
addUpper:   ld    write_1   ; Loads upper MM byte
            add   one       ; Write_1++
            mm    write_1   ; Writes result
            jp    write_3   ; Returns to loop  
