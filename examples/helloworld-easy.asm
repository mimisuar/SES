; Simple Hello, World Example ;
; By: Miguel Suarez-Rodriguez (sheepmanbuddy) ;
; This example shows a much simpler way of writing out hello world ;

writeHelloWorld:
	; spell out hello world ;
	lda #$48	
	wra $000	; 'H' ;
	lda #$65	
	wra $001	; 'e' ;
	lda #$6C	
	wra $002	; 'l' ;
	wra $003	; 'l' ;
	lda #$6F	
	wra $004	; 'o' ;
	lda #$2C
	wra $005	; ',' ;
	lda #$20
	wra $006	; ' ' ;
	lda #$77
	wra $007	; 'w' ;
	lda #$6F
	wra $008	; 'o' ;
	lda #$72
	wra $009	; 'r' ;
	lda #$6C	
	wra $00A	; 'l' ;
	lda #$64
	wra $00B	; 'd' ;
	lda #$21
	wra $00C	; '!' ;
	poff