; LOADER (starts at $000)
            ld      fail        
            sub     fail
            mm      fail        ; Resets fail flag
            jp      readAddr    ; Jumps data area to first instruction

; //---------Data Area----------//

counter:    con     $00         ; Counts number of bytes of data to be read
one:        con     $01         ; 1 constant
opcode:     con     $90         ; MM opcode value
fail:       con     $00         ; If a chacksum failed
checksum:   con     $00         ; Calculated Checksum
compAux:    con     $FF         ; Auxiliar to calculate 1's complement

; //--------Main Program--------//

; Read base address:
readAddr:   ld      checksum    ; Loads checksum.
            sub     checksum    ; Zero.
            mm      checksum    ; Reset checksum.      
            sc      readByte    ; Reads first address byte from file
            add     opcode      ; Adds upper address to opcode
            mm      write_1     ; Writes result to write_1

;  -- Sees if end of code
            sub     opcode      ; Removes opcode
            sub     compAux     ; Compares with FF
            jz      eop         ; If it was equal to FF, terminate program.
;  --
            sc      readByte    ; Reads second address byte from file
            mm      write_2     ; Writes to write_2
            
            sc      readByte    ; Reads byte count from file
            mm      counter     ; Writes byte count to counter

; Memory writing loop
loop:       sc      readByte    ; Reads a byte of data
write_1:    con     $90         ; MM first byte (will be read as instruction)
write_2:    con     $00         ; MM second byte (will be read as instruction)
            ld      write_2     ; Loads MM second byte (lower address byte)
            add     one         ; write_2 += 1
            mm      write_2     ; Writes new lower address byte
            jz      addUpper    ; if equals zero (overflow) we add one to upper byte.  
write_3:    ld      counter     ; Loads value from counter
            sub     one         ; Counter--
            mm      counter     ; Writes new value of counter
            jz      checkChks   ; Compares checksums if counter = 0
            jp      loop        ; Goes back to loop if counter != 0

; Adds one to upper MM byte
addUpper:   ld      write_1     ; Loads upper MM byte.
            add     one         ; Write_1++
            mm      write_1     ; Writes result.
            jp      write_3     ; Continues back to loop

; Compares the checksums
checkChks:  ld      compAux     ; Loads $FF
            sub     checksum    ; One's complement to calculate checksum
            mm      checksum    ; Saves computed checksum to memory
            gd                  ; Reads checksum from file
            sub     checksum    ; Compares both checksums
            jz      readAddr    ; If they are different, we continue and read next address.
            mm      fail        ; If they are differetn, we raise error flag and terminate program.

; Terminates program
eop:        hm      readAddr    ; Halts machine.

; //--------Subroutines---------//

; Gets a byte and adds to checksum.
readByte:   jp      $000        ; Return address.
            gd                  ; Reads a byte of data.     
            mm      data        ; Saves read byte.
            add     checksum    ; Adds to checksum.
            mm      checksum    ; Saves results to checksum variable.
            ld      data        ; Recovers data that was read.
            rs      readByte    ; Returns from soubroutine.
; ReadByte data
data:       con     $00         ; Data that was read.


