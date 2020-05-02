            org     $100
        
DADO:       CON     $01   
INIC:       LD      DADO    ; Primeira instrucao executavel do programa;
            ADD     DADO    ; AC += mem[DADO]
            PD      $FF     ; Print Data
            HM 
            INIC    ; Halts
            END     INIC    ; Indica INIC como endereco de partida do programa;