; Overly Complex Hello World ;
; By: Miguel Suarez-Rodiguez (sheepmanbuddy) ;
; This example was to test some of the different operations the SASM can do ;

writeHelloWorld:
	lda #$48	; Load $48(H) into A ;
	wra $000	; Write 'H' into the first position ;
	or #$20	; AND it with 32, i.e. switch to lowercase h ;
	sub #3		; Subtract A by 3 setting it to $65(e)
	wra $001	; Write 'e' into the second position ;
	add #%0111	; Add 7 to A setting it to $6C(l) ;
	wra $002	; Write 'l' ;
	wra $003	; Write 'l' ;
	sta $0000	; Store 'l' in $0000 ;
	add #3		; Add 7 to A setting it to $6F(o) ;
	wra $004	; Write 'o' ;
	sub #$43	; Set to $2C(,) ;
	wra $005	; Write ',' ;
	and #$20	; Keep only $20( ) ;
	wra $006	; Write ' ' ;
	lsh #2		; Left-shift by two, keeping $80 ;
	sub #9		; Now we are at $77(w) ;
	wra $007	; Write 'w' ;
	sub #8		; A = $6F(o) ;
	wra $008	; Write 'o' ;
	add #3		; A = $72(r) ;
	wra $009	; Write 'r' ;
	lda $0000	; load 'l' ;
	wra $00A	; Write 'l' ;
	sub #8		; A = $64(d) ;
	wra $00B	; Write 'd' ;
	xor #$45	; A = $33(!) ;
	wra $00C
	poff		; power off	;