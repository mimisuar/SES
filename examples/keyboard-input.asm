reset:
	lda $410	; Store the keyboard byte into the accumulator ;
	and #$02	; Keep only the status of the 'return' key ;
	cpa #$02	; Check to see if the 'return' key is pressed ;
	bne reset	; keep looping until the 'return' key is pressed ;
	poff		; power off