            org     $100
        
DADO:       CON     $FFF
AUX:        CON     $20     
INIC:       LD      AUX     ; Primeira instrucao executavel do programa;
ABBA:       MM      AUX     ; Salva acumulador em AUX
            ADD     $200    ; AC += mem[0x200]
            ADD     $202    ; AC += mem[0x202]
LOOP:       LD      DADO    ; Copia DADO para o acumulador
            JP      LOOP    ; Fecha o loop
            END     INIC    ; Indica INIC como endereco de partida do programa;