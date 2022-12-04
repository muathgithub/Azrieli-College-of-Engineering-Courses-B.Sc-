; Exercise Two LCD 3 Digits Counter
; Student Name: Muath Abu Jamal

LIST P=PIC16F877 
include <P16f877.inc>
__CONFIG _CP_OFF & _WDT_OFF & _BODEN_OFF & _PWRTE_OFF & _HS_OSC & _LVP_OFF & _DEBUG_OFF & _CPD_OFF

;Giving meningfull names for the needed registers
Ones EQU 0x30
Tens EQU 0x31
Hundreds EQU 0x32

;Giving meningfull names for the needed values
Zero_Char_Hex EQU 0x30
PA3 EQU 0x03

org 0x00 
reset:
goto start 
org 0x04 

start:
	;Bank 1
	bsf STATUS, RP0
	bcf STATUS, RP1
	
	;Making the input/output types digital for all the ports
	movlw 0x06
	movwf ADCON1
	
	;Making the diriction of port A input
	movlw 0x3F
	movwf TRISA
	
	;Making the diriction of ports D and E outputs, so we can send/contact with the LCD
	clrf TRISD
	clrf TRISE
	
	;Bank 0 
	bcf STATUS, RP0
	
	;Calling the method init inorder to turn on/run and prepare the LCD 
	call init

;This lable restarts the counter to zeros
RESTART_DOWN:
	;Making the values of the needed registers(Counter registers) Zeros
	clrf Ones
	clrf Tens
	clrf Hundreds
	
	;Display or updatring the LCD with the rigesters values by calling the function Update_LCD [in this case all the digits are zeros]  
	call Update_LCD
	
	;checking the value of bit 3 of port A so we decide to increase or decrease the counter
	;if the value is (1) we jump to Ones_increase else (0) we jump to Ones_decrease
	btfss PORTA, PA3
	goto Ones_decrease
	goto Ones_increase

;This lable restarts the counter to 999 (nines)
RESTART_UP:
	;Moving the value 9 to all the rigisters
	movlw 0x09
	movwf Ones
	movwf Tens
	movwf Hundreds
	
	;checking the value of bit 3 of port A so we decide to increase or decrease the counter
	;if the value is (1) we jump to Ones_increase else (0) we jump to Ones_decrease
	btfss PORTA, PA3
	goto Ones_decrease
	goto Ones_increase

;This lable keeps increasing the register Ones each time by one and updating the LCD till it reaches Nine (9)
;then it jumps to the lable Tens_increase
;its also keeps checking the value of bit 3 of port A to decide either to increase or to decrease the counter (to which lable to jump)
Ones_increase:
	movlw 0x09
	subwf Ones, w
	btfsc STATUS, Z
	goto Tens_increase
	incf Ones
	call Update_LCD
	btfsc PORTA, PA3
	goto Ones_increase
;This lable keeps decreasing the register Ones each time by one and updating the LCD till it reaches Zero
;then it jumps to the lable Tens_decrease
;its also keeps checking the value of bit 3 of port A to decide either to increase or to decrease the counter (to which lable to jump)
Ones_decrease:
	movf Ones, Ones ;by this command which is to move the value of the rigister to itself we can know if its value is zero or nut because the moving effects/modifies the value of bit Z in status rigister
	btfsc STATUS, Z
	goto Tens_decrease
	decf Ones
	call Update_LCD
	btfsc PORTA, PA3
	goto Ones_increase
	goto Ones_decrease


;This lable reset/clrf the rigister Ones and increasing the register Tens each time by one when we jump to it and updating the LCD
;when the rigister reaches nine next attempt to increase it will jump to the lable Hundreds_increase
;its also keeps checking the value of bit 3 of port A to decide either to increase or to decrease the counter (to which lable to jump)
Tens_increase:
	clrf Ones
	movlw 0x09
	subwf Tens, w
	btfsc STATUS, Z
	goto Hundreds_increase
	incf Tens
	call Update_LCD
	btfsc PORTA, PA3
	goto Ones_increase
	goto Ones_decrease
;This lable decreasing the register Tens each time by one when we jump to it and updating the LCD also it moves the value Nine (9) to the rigister Ones
;when the rigister reaches Zero next attempt to increase it will jump to the lable Hundreds_decrease
;its also keeps checking the value of bit 3 of port A to decide either to increase or to decrease the counter (to which lable to jump)
Tens_decrease:
	movf Tens, Tens ;by this command which is to move the value of the rigister to itself we can know if its value is zero or nut because the moving effects/modifies the value of bit Z in status rigister
	btfsc STATUS, Z
	goto Hundreds_decrease
	decf Tens
	movlw 0x09
	movwf Ones
	call Update_LCD
	btfsc PORTA, PA3
	goto Ones_increase
	goto Ones_decrease

;This lable reset/clrf the rigister Ones and Tens also it increasing the register Hundreds each time by one when we jump to it and updating the LCD
;when the rigister reaches Nine next attempt to increase it will jump to the lable RESTART_DOWN
;its also keeps checking the value of bit 3 of port A to decide either to increase or to decrease the counter (to which lable to jump)
Hundreds_increase:
	clrf Ones
	clrf Tens
	movlw 0x09
	subwf Hundreds, w
	btfsc STATUS, Z
	goto RESTART_DOWN
	incf Hundreds
	call Update_LCD
	btfsc PORTA, PA3
	goto Ones_increase
	goto Ones_decrease
;This lable decreasing the register Hundreds each time by one when we jump to it and updating the LCD also it moves the value Nine (9) to the rigisters Ones and Tens
;when the rigister reaches Zero next attempt to increase it will move the value Nine (9) to the rigisters Ones and Tens then it will jump to the lable Ones_decrease or Ones_increase according to bit 3 of port A
;its also keeps checking the value of bit 3 of port A to decide either to increase or to decrease the counter (to which lable to jump)
Hundreds_decrease:
	movf Hundreds, Hundreds ;by this command which is to move the value of the rigister to itself we can know if its value is zero or nut because the moving effects/modifies the value of bit Z in status rigister
	btfsc STATUS, Z
	goto RESTART_UP
	decf Hundreds
	movlw 0x09
	movwf Ones
	movwf Tens
	call Update_LCD
	btfsc PORTA, PA3
	goto Ones_increase
	goto Ones_decrease

;This function updates the LCD with the values oth the counter registers each time we call it
Update_LCD:
	
	movlw	0x82			 ;PLACE for the data on the LCD
	movwf	0x20
	call 	lcdc
	call	My_Dellay
	
	;Adding the ASCII value of zero to the value of the rigister so we get the ASCII value of the number char in ASCII 
	movlw Zero_Char_Hex
	addwf Ones, w

	movwf	0x20
	call 	lcdd
	call	My_Dellay

	movlw	0x81			 ;PLACE for the data on the LCD
	movwf	0x20
	call 	lcdc
	call	My_Dellay
	
	;Adding the ASCII value of zero to the value of the rigister so we get the ASCII value of the number char in ASCII 
	movlw Zero_Char_Hex
	addwf Tens, w

	movwf	0x20
	call 	lcdd
	call	My_Dellay

	movlw	0x80			 ;PLACE for the data on the LCD
	movwf	0x20
	call 	lcdc
	call	My_Dellay
	
	;Adding the ASCII value of zero to the value of the rigister so we get the ASCII value of the number char in ASCII 
	movlw Zero_Char_Hex
	addwf Hundreds, w

	movwf	0x20
	call 	lcdd
	call	My_Dellay

	return

;-----------------------------*** MY_DELLAY ***---------------------------

; This method applies the delay mdel n times
My_Dellay:
	movlw 0x02 ;(n)
	movwf 0x25
In_Dellay:
	call mdel
	decf 0x25
	btfss STATUS, Z
	goto In_Dellay
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