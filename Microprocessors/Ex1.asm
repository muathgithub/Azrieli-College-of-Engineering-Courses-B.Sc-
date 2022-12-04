; Exercise One [BCD] ;
; Student Name: Muath Abu Jamal


LIST P=PIC16F877 
include <P16f877.inc>
org 0x00 
reset:
goto start 
org 0x04 
start:

bcf STATUS, RP0; Bank 0
bcf STATUS, RP1; Bank 0

Restart:
	clrf 0x20 ; Clearing/Making the values of the registers  
	clrf 0x21 ; 0x20(BCD), 0x21(first 4 bits counter), 0x22(Second for bits counter), [Zero/es]
	clrf 0x22

loop_one:; loop for the first four bits of the counter  
	movf 0x20, w ; Move/Put the value of the reg 0x20 to the reg w
	xorlw 0x99 ; making xor logic operatin between w and the value 0x99 so we can figure of the caounter reached the end/maximum
	btfsc STATUS, Z; checking if the value of the xor operation is zero
	goto Restart; if true we jump to the label restart where we restart the counter and making the hole process again 
	movlw 0x09; ; Puting the value 0x09 in the reg w
	subwf 0x21, f; subtracting the value of reg (w [0x09]) form the reg 0x21 
	btfsc STATUS, Z; checking ig the result of the subtracion is zero
	goto loop_two; if true we jump the loop_two for increasing the value of the reg 0x22 by one and making the value of the reg 0x21 zero
	addwf 0x21, f; if false we add the value 0x09 to the reg 0x21 so we fix the subtraction
	incf 0x21, f; in creasing the value of reg 0x21 by one
	call Update_Counter; calling the function Update_Counter that updates the counter(0x20) whith the new values
	goto loop_one ; continue the loop_one

loop_two:; this loop is for increasing the value of the reg 0x22(Second for bits counter) by one each time we call it and it makes the value of the reg 0x21 zero
	clrf 0x21; clearing the reg 0x21 and making its value 0x00
	incf 0x22, f; increasing thevalue of the reg 0x22 by one
	call Update_Counter ; calling the function Update_Counter that updates the counter(0x20) whith the new values
	goto loop_one; jump to loop_one to start counting to 0x09 again or restarting the programe if the value of 0x20 is 0x99

Update_Counter:
	bcf STATUS, C; Making the value of the carry bit c in status reg zero so we make sure that we dont get an unexpexted behaviour
	
	rlf 0x22, f; *from here 
	rlf 0x22, f; we rotate the bits of the rigister 0x22 4 time to the right 
	rlf 0x22, f; so we get the counter number in the upper four bits as we need for [BCD]
	rlf 0x22, f; *to here
	
	clrw; clearing the reg w (making its value = 0x00)
	iorwf 0x21, w; making or operation between the value of the reg 0x21 and the value of the reg w so we get the lower for bits of the [BCD] counter
	iorwf 0x22, w; making or operation between the value of the reg 0x22 and the value of the reg w so we get the upper for bits of the [BCD] counter
	clrf 0x20; clearing the reg 0x20 (making its value = 0x00)
	iorwf 0x20, f; making or operation between the value of the reg 0x20 and the value of the reg w so we get the updated value of the [BCD] counter

	rrf 0x22, f; *from here
	rrf 0x22, f; we rotate the bits of the rigister 0x22 4 time to the left
	rrf 0x22, f; so we return the bits that we moved to the upper 4 bits to the lower 4 bits
	rrf 0x22, f; *to here

	movlw 0x09; making the value of the reg w 0x09 to continue the process of counting and checking
	
	return

end