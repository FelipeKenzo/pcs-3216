
; ====== PROGRAMA FONTE PARA A PRIMEIRA QUESTÃO DA PROVA  =======

        ORG  $100
; inicia contador de letras
START:  LD   COMP   ; comprimento do vetor de letras
        MM   CONT   ; inicia o contador do loop
; constrói inicialmente a instrução LD INIC adiante
; (a ser incrementada a cada execução do loop)

IN:     LD   LDA0_1
        MM   LDA_1
        LD   LDA0_2
        MM   LDA_2

; executa a instrução construída logo a seguir:
LDA_1:  CON  $00    ; pega a próxima letra no vetor de letras
LDA_2:  CON  $00
        PD   SAIDA  ; grava-a no arquivo de saída

; testa se é a última letra
        LD   CONT
        SUB  UM     ; decrementa contador do loop
        MM   CONT
        JZ   FORA   ; se zerar, terminou o loop, se não, prossegue

; incrementa o endereço a ser acessado no vetor de letras
        LD   LDA_2
        ADD  UM     ; incrementa byte menos significativo
        MM   LDA_2

        JZ   INCR   ; se deu zero, vai incrementar o outro byte
        JP   LDA_1  ; se não, executa mais uma instancia do loop

; incrementa o byte mais significativo do endereço a acessar
INCR:   LD   LDA_1   
        ADD  UM     ; incrementa byte mais significativo
        MM   LDA_1
        JP   LDA_1  ; e vai executar outra instancia do loop

; final do loop
FORA:   HM   START  ; pára a máquina e volta ao início se for reacionada

; ========================================================================
;
; Dispositivos virtuais de entrada e saída
;
; os endereços 3 e 4 são reservados para operarem como registradores de dados 
; da interface dos dispositivos de entrada e saída respectivamente. 
;
; A instrução GD da MVN deposita na posição reservada 3 de memória o byte lido 
; a partir do arquivo texto que simula a fita de entrada.
;
; A instrução PD da MVN extrai da posição reservada 4 de memória o byte 
; a ser transferido para o arquivo texto que simula a fita de saída.

            ORG   $71
ENTRADA:    CON   $00     ; /71 = endereço do dispositivo virtual de entrada
            ORG   $72
SAIDA:      CON   $00     ; /72 = endereço do dispositivo virtual de saída

; ==========================  ÁREA DE DADOS   ============================

            ORG   $200    ; a partir do endereço /200

; constantes e variáveis

UM:         CON   1       ; constante 1
CONT:       CON   0       ; variável CONT iniciada com zero
LDA0_1:     CON   $8F     ; código da instrução LD INIC
LDA0_2:     CON   $01

; área do texto a ser manipulado
            ORG   $F00    ; este texto está alocado no endereço /F00 e seguintes
COMP:       CON   5       ; comprimento do texto
INIC:       CON   $74     ; primeira letra do texto
            CON   $65
            CON   $78
            CON   $74
            CON   $6F     ; última letra do texto

; ========================================================================

            END START     ; o  endereço inicial do programa é START.