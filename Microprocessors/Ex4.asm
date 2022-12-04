; Exercise Four Power Meter
; Student Name: Muath Abu Jamal


LIST 	P=PIC16F877
include	<P16f877.inc>
__CONFIG _CP_OFF & _WDT_OFF & _BODEN_OFF & _PWRTE_OFF & _HS_OSC & _WRT_ENABLE_ON & _LVP_OFF & _DEBUG_OFF & _CPD_OFF

; Giving names to the needed bits
AN1 EQU 0x01

Ones EQU 0x30
Tens EQU 0x31
Hundreds EQU 0x32
Temp EQU 0x33

Zero_Char_Hex EQU 0x30

		org	0x00		
reset:	goto start
		org	0x10

start:	
	bsf	STATUS, RP0 ; bank1
	bcf	STATUS, RP1
	
	movlw 0x02 ; making the port A analog and keep port E digital
	movwf ADCON1 ; format : 6 lower bit of ADRESL =0

	movlw 0x3F ; making the diriction of port A input
	movwf TRISA
	
	;Making the diriction of ports D and E outputs, so we can send/contact with the LCD
	clrf TRISD
	clrf TRISE

	bcf	STATUS, RP0 ; bank 0
	
	movlw 0x89
	movwf ADCON0 ;Fosc/32; channel_1; ADC on

	;Calling the method init inorder to turn on/run and prepare the LCD 
	call init

Check_AN1:
	call d_20 ;calling dellay
	bsf ADCON0,GO ;start conversion

waitc: 
	btfsc ADCON0,GO ;wait end of conversion
	goto waitc
	call d_4 ;calling dellay
	movf ADRESH,W ;moving the result of the converstion from the register ADRESH to the register W 
	movwf Temp ; moving the value of w to the rigister Temp in order to sendet to the method C_To_Three_Digits
    call C_To_Three_Digits ; calling the method C_To_Three_Digits that converts the number from binary to 3 digits dicimal
	call Update_LCD ; updating the LCD with the new reading each time
	goto Check_AN1 ; jump to the lable Check_AN1 to repeate the check and the hole prossece again  


Update_LCD:

	movlw	0x84			 ;PLACE for the data on the LCD
	movwf	0x20
	call 	lcdc
	
	movlw 	0x56 ; V value in ASCII table

	movwf	0x20
	call 	lcdd

;---------	
	movlw	0x83			 ;PLACE for the data on the LCD
	movwf	0x20
	call 	lcdc
	
	;Adding the ASCII value of zero to the value of the rigister so we get the ASCII value of the number char in ASCII 
	movlw Zero_Char_Hex
	addwf Ones, w

	movwf	0x20
	call 	lcdd

;---------	
	movlw	0x82			 ;PLACE for the data on the LCD
	movwf	0x20
	call 	lcdc
	
	;Adding the ASCII value of zero to the value of the rigister so we get the ASCII value of the number char in ASCII 
	movlw Zero_Char_Hex
	addwf Tens, w

	movwf	0x20
	call 	lcdd

;---------			
	movlw	0x81			 ;PLACE for the data on the LCD
	movwf	0x20
	call 	lcdc

	movlw 0x2E ; dot ASCII value

	movwf	0x20
	call 	lcdd

;---------	
	movlw	0x80			 ;PLACE for the data on the LCD
	movwf	0x20
	call 	lcdc
	
	;Adding the ASCII value of zero to the value of the rigister so we get the ASCII value of the number char in ASCII 
	movlw Zero_Char_Hex
	addwf Hundreds, w

	movwf	0x20
	call 	lcdd

	return

C_To_Three_Digits:
			clrf 	Ones 	; 1's register
			clrf 	Tens 	; 10's register
			clrf 	Hundreds 	; 100's register

Count100s:	movlw 0x64		; Put 100 into W
			subwf Temp,F 	; Substruct 0x30 - 100 to calculate 100's in the number
			btfss STATUS,C	; Check C ;if C=0  there is a borrow and the result is negative	
			goto Find10s
			incf Hundreds,F 	; incf 0x21,F
			goto Count100s

Find10s:	movlw 0x64		; Put 100 into W
			addwf Temp,F	; Fix last 100's substraction by adding 100 to 0x30 (Currently 0x30 is negative)
Count10s:	movlw 0x0A		; Put 10 into W	
			subwf Temp,F 	; Substruct 0x30 - 10 to calculate 10's in the number
			btfss STATUS,C	; Check C ;if C=0  there is a borrow and the result is negative	
			goto Find1s
			incf Tens,F 	; incf 0x21,F
			goto Count10s

Find1s:		movlw 0x0A		; Put 10 into W
			addwf Temp,F	; Fix last 10's substraction by adding 10 to 0x30 (Currently 0x30 is negative)
			movf Temp,W 	; Now 0x30 contains the 1's 
			movwf Ones      ;Puts 1's  in 0x20
			return

;--------------------------------------------------------------------------
d_20: 
	movlw 0x20
	movwf 0x22
lulaa9: 
	decfsz 0x22,1
	goto lulaa9
	return

d_4: 
	movlw 0x06
	movwf 0x22
lulaa0: 
	decfsz 0x22,1
	goto lulaa0
	return

;-----------------------------***LCD CODE ***-----------------------------
init	movlw	0x30
		movwf	0x20
		call 	lcdc
		call	del_41

		movlw	0x30
		movwf	0x20
		call 	lcdc
		call	del_01

		movlw	0x30
		movwf	0x20
		call 	lcdc
		call	mdel

		movlw	0x01		; display clear
		movwf	0x20
		call 	lcdc
		call	mdel

		movlw	0x06		; ID=1,S=0 increment,no  shift 000001 ID S
		movwf	0x20
		call 	lcdc
		call	mdel

		movlw	0x0c		; D=1,C=B=0 set display ,no cursor, no blinking
		movwf	0x20
		call 	lcdc
		call	mdel

		movlw	0x38		; dl=1 ( 8 bits interface,n=12 lines,f=05x8 dots)
		movwf	0x20
		call 	lcdc
		call	mdel
		return

;
;subroutine to write command to LCD
;

lcdc	movlw	0x00		; E=0,RS=0 
		movwf	PORTE
		movf	0x20,w
		movwf	PORTD
		movlw	0x01		; E=1,RS=0
		movwf	PORTE
        call	sdel
		movlw	0x00		; E=0,RS=0
		movwf	PORTE
		return

;
;subroutine to write data to LCD
;

lcdd	movlw		0x02		; E=0, RS=1
		movwf		PORTE
		movf		0x20,w
		movwf		PORTD
        movlw		0x03		; E=1, rs=1  
		movwf		PORTE
		call		sdel
		movlw		0x02		; E=0, rs=1  
		movwf		PORTE
		return

;----------------------------------------------------------

del_41	movlw		0xcd
		movwf		0x23
lulaa6	movlw		0x20
		movwf		0x22
lulaa7	decfsz		0x22,1
		goto		lulaa7
		decfsz		0x23,1
		goto 		lulaa6 
		return


del_01	movlw		0x20
		movwf		0x22
lulaa8	decfsz		0x22,1
		goto		lulaa8
		return


sdel	movlw		0x19		; movlw = 1 cycle
		movwf		0x23		; movwf	= 1 cycle
lulaa2	movlw		0xfa
		movwf		0x22
lulaa1	decfsz		0x22,1		; decfsz= 12 cycle
		goto		lulaa1		; goto	= 2 cycles
		decfsz		0x23,1
		goto 		lulaa2 
		return


mdel	movlw		0x0a
		movwf		0x24
lulaa5	movlw		0x19
		movwf		0x23
lulaa4	movlw		0xfa
		movwf		0x22
lulaa3	decfsz		0x22,1
		goto		lulaa3
		decfsz		0x23,1
		goto 		lulaa4 
		decfsz		0x24,1
		goto		lulaa5
		return
end