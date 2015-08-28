; Index Example ;
; By: Miguel Suarez-Rodriguez (sheepmanbuddy) ;
; This example shows how you can use indexs and loops ;

; for the sake of my sanity, I have copied over the code from the previous example into this one. ;

writeHelloWorld:
	rda $000			; Reading the first value in the file ;
	or #$20
	cpa #$68			; We also start to invert if the first letter is 'h' ;
	beq invertLetters
	
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
	rst			; go back to the beginning ;
	
invertLetters:
	ldi $000	; load $000 into the index, we will start here ;
	ldz #$D		; load $F into Z, this will be our counter. When this reaches zero, we stop inverting letters ;
tmp:
	rda I		; read address I ;
	xor #$20	; This is the actual inversion code ;
	cpa #$20	; If A is less than $20 then we have a non-printable character ;
	blt skip
	wra I		; write the newly inverted letter into the spot of the previous one ;
skip:
	adi $001	; Unlike the accumulator, index only can be added to or subtracted from and only works with addresses ;
	lda Z		; we load Z into A to do math ;
	sub #1		; decrement ;
	ldz	A		; we load A back into Z to prepare for the next loop ;
	cpa #0		; if it is zero we are done with the loop ;
	bgt tmp		; go back ;
	poff		; we finished ;