;======== Programa Principal ========

            ORG $415

; Lê um byte do NUSP
LD_1:       CON $85     ; LD $508 (bit mais significativo)
LD_2:       CON $08     ; LD $508 (bit menos significativo)

            SC  ATOH    ; Converte o caractere lido para hexadecimal

; Aplica o algoritmo de codificação            
            ADD ACUM     ; Soma com o acumulador
            MM  ACUM     ; Atualiza o acumulador
            MUL CON1     ; Shifta 4 bits para a esquerda
            DIV CON1     ; Shifta 4 bits para a direita (isola o nibble)

            SC  HTOA    ; Converte esse nibble para ascii

; Escreve o resultado do algoritmo na memória
MM_1:       CON $95     ; MM $510 (bit mais significativo)
MM_2:       CON $10     ; MM $510 (bit menos significativo)

            PD          ; Imprime o nibble em ascii na fita

; Verifica se acabou o programa
            LD  COUNT   ; Carrega valor do contador
            SUB UM      ; Subtraio um
            MM  COUNT   ; Atualiza o valor do contador
            JZ  END     ; Se for zero, acabou o programa

            LD  LD_2    ; Carrego o MSB do endereço de leitura
            ADD UM      ; Somo um
            MM  LD_2    ; Atualizo o endereço
            JZ  INCR_1  ; Se houve overflow, preciso incrementar o MSB.

CONT:       LD  MM_2    ; Carrego o MSB do endereço de escrita
            ADD UM      ; Somo um
            MM  MM_2    ; Atualizo o endereço
            JZ  INCR_2  ; Se houve overflow, preciso incrementar o MSB.
            
            JP  LD_1    ; Retorna ao loop

; incrementa o byte mais significativo do endereço a ler
INCR_1:     LD   LD_1   
            ADD  UM     ; incrementa byte mais significativo
            MM   LD_1
            JP   CONT   ; prossegue para atualizar endereço de escrita

; incrementa o byte mais significativo do endereço a escrever
INCR_2:     LD   MM_1   
            ADD  UM     ; incrementa byte mais significativo
            MM   MM_1
            JP   LD_1   ; e vai executar outra instancia do loop

END:        HM   LD_1    ; Fim do programa, halt


;=========== Sub-Rotinas ===========

; Converte ascii para hexadecimal
ATOH:       JP  $000    ; Retorno da sub-rotina
            MM  ASCII   ; Salva o valor em ascii.
            SUB CON2    ; Compara com A em ascii.
            JN  SUB30   ; Se for negativo, o número está entre 0-9
            JP  SUB37   ; Caso contrário, está entre 10 - 15
SUB30:      LD  ASCII   ; Recupera o valor original
            SUB CON3    ; Converto para decimal subtraindo $30
            RS  ATOH    ; Retorno
SUB37:      LD  ASCII   ; Recupera o valor original
            SUB CON4    ; Converto para decimal subtraindo $37
            RS  ATOH    ; Retorno
ASCII:      CON $00     ; Guarda o valor original em ascii.

; Converte hexadecimal para ascii
HTOA:       JP  $000    ; Retorno da sub-rotina
            MM  HEX     ; Salva o valor em hexadecimal
            SUB CON6    ; Comparo com 10
            JN  ADD_30  ; Se for negativo, o número está entre 0-9
            JP  ADD_37  ; Caso contrário, está entre 10-15
ADD_30:     LD  HEX     ; Recupera o valor original
            ADD CON3    ; Converto para ascii somando $30
            RS  HTOA    ; Retorno
ADD_37:     LD  HEX     ; Recupera o valor original
            ADD CON4    ; Converto para ascii somando $37
            RS  HTOA    ; Retorno
HEX:        CON $00     ; Guarda o valor original em hexadecimal

;========== Area de Dados ==========

            ORG $500

COUNT:      CON $08     ; Contador
ACUM:       CON $00     ; Acumulador
UM:         CON $01     ; Constante de valor 1
CON1:       CON $10     ; Constante de valor 16
CON2:       CON $41     ; Constante de valor $41
CON3:       CON $30     ; Constante de valor $30
CON4:       CON $37     ; Constante de valor $37
CON6:       CON 10      ; Constante de valor 10

NUSP:       CON $35     ; '5'
            CON $31     ; '1'
            CON $34     ; '4'
            CON $34     ; '4'
            CON $37     ; '7'
            CON $32     ; '2'
            CON $30     ; '0'
            CON $31     ; '1'

CODIGO:     CON $00
            CON $00
            CON $00
            CON $00
            CON $00
            CON $00
            CON $00
            CON $00

            END