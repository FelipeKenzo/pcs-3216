            org     $100
            jp      init    ; goes to init
const1:     con     $0      ; const1 = 0
const2:     con     $1      ; const2 = 1
const3:     con     $FFFF   ; const3 = $FFFF (-1)
const4:     con     $ef0    ; const4 = $EF0
const5:     con     $3      ; const5 = 3
var1:       con     $0      ; var1 = 0
init:       ld      const2  ; ac = 1
            jz      jz      ; jump to "jz" if acc = 0 (shouldn't)
            ld      const1  ; ac = 0
            jz      jz      ; jump to "jz" if acc = 0 (should)
            add     const3
jz:         jn      jn      ; jump to "jn" if acc < 0 (shouldn't)
            ld      const3  ; ac = -1
            jn      jn      ; jump to "jn" if acc < 0 (should)
            add     const3
jn:         add     const4  ; ac += $EFO (expected to result in $EEF)
            sub     const4  ; ac -= $EF0 (expected to result in $FFFF)
            mul     const3  ; ac *= $FFFF (expected to result in 1)
            ld      const4  ; ac = $EF0
            div     const5  ; ac /= 3 (expected $4FA)
            mm      var1    ; var1 = ac
            ld      const1  ; ac = 0
            ld      var1    ; ac = var1 (should be $4FA)
            sc      subr    ; branch to subr (pc should be $300)     
            PD      $0      ; Print Data
            GD      $0      ; get data
            PD      $0 
            HM      init    ; Halts Machine
            
            org     $300
subr:       add     const2
            rs      $0
            
            END     init    ; Indica INIC como endereco de partida do programa;