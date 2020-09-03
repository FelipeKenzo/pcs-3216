; ==========================
; ==     MAIN PROGRAM     ==
; ==========================
            ORG     $100

; -- CONSTANTS

ONE:        CON     1

            LV      $F
LOOP1:      JZ      END1    ; Meaningless loop
            SUB     ONE
            JP      LOOP1

; ==== Load Overlay A   ====
END1:       OS      $0      ; Overlay Driver (load)
            CON     $1      ;     |- Overlay A
            CON     $01     ;     |- At address $114
            CON     $14     

; ==== Load Overlay B   ====
            OS      $0      ; Overlay Driver (load)
            CON     $2      ;     |- Overlay B
            CON     $01     ;     |- At address $114
            CON     $14
            
            HM      $00     ; Halt machine - end of execution

; ==========================
; ==     Overlay A        ==
; ==========================

            OVS             ; Indicates to assembler start of overlay block
            LV      $A
            PD

; ==== Load Overlay AA  ====
            OS      $0      ; Overlay Driver (load)
            CON     $3      ;     |- Overlay AA
            CON     $00     ;     |- At address $010
            CON     $10

; ==== Load Overlay BB  ====
            OS      $0      ; Overlay Driver (load)
            CON     $4      ;     |- Overlay AB
            CON     $00     ;     |- At address $010
            CON     $10

            OS      $1      ; Overlay Driver (return)

            OVE             ; Indicates to assembler end of overlay block


; ==========================
; ==     Overlay B        ==
; ==========================

            OVS             ; Indicates to assembler start of overlay block
            LV      $B
            PD

            OS      $1      ; Overlay Driver (return)

            OVE     $1      ; Indicates to assembler end of overlay block

; ==========================
; ==     Overlay AA       ==
; ==========================

            OVS             ; Indicates to assembler start of overlay block
            LV      $AA
            PD

            OS      $1      ; Overlay Driver (return)

            OVE             ; Indicates to assembler end of overlay block

; ==========================
; ==     Overlay AB       ==
; ==========================

            OVS             ; Indicates to assembler start of overlay block
            LV      $BB
            PD

            OS      $1      ; Overlay Driver (return)

            OVE             ; Indicates to assembler end of overlay block

            END