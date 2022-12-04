; Exercise Three Timer0
; Student Name: Muath Abu Jamal


; ****** IMPORTANT NOTE ****** 
; When we make the calculations for Time0 using the equasion we got the values (ps = 2):
; ( d = 131, 0x83 ) for a half cycle in the frequence of [ 10 KHz ]
; ( d = 231, 0xE7 ) for a half cycle in the frequence of [ 50 KHz ]
; But when we run the this code on the board and the reding devices in the labe last week ( in collage ) 
; We didnt get We didn't get an accurate reading there was a slight/small difference
; So we increased the values a little:
; ( d = 140 (we added 9), 0x8C ) for a half cycle in the frequence of [ 10 KHz ]
; ( d = 240 (we added 9), 0xF0 ) for a half cycle in the frequence of [ 50 KHz ]
; The code runs with these valuse (+9 values)
; so if you run the code in the simulator or a device that works properly with the above values (the equastion values)
; please change the (+9 values) with the above values (the equastion values)

LIST 	P=PIC16F877
include	<P16f877.inc>
__CONFIG _CP_OFF & _WDT_OFF & _BODEN_OFF & _PWRTE_OFF & _HS_OSC & _WRT_ENABLE_ON & _LVP_OFF & _DEBUG_OFF & _CPD_OFF

; Giving names to the needed bits
PC0 EQU 0x00
PA2 EQU 0x02
PA3 EQU 0x03

		org	0x00		
reset:	goto start
		org	0x10

start:	
	bsf	STATUS, RP0 ; bank1
	bcf	STATUS, RP1
	
	movlw 0x06 ; making all the ports input's output's digital 
	movwf ADCON1

	movlw 0x3F ; making the diriction of port A input
	movwf TRISA
	
	clrf TRISC ; making the diriction of port C output
	bcf INTCON, T0IF ; clearing the T0IF bit which means the Timer0 didn't reach zero yet

	movlw 0xC0 ; making the ps = 2 and assigning it to Timer0 with the internal clock
	movwf OPTION_REG

	bcf	STATUS, RP0 ; bank 0
		
CHECK_STATUS: ; this method/lable checks if the bit PA2 is equals to zero if true it goes to the lable From_Zero else it goes to From_One
	btfsc PORTA, PA2
	goto From_One
	goto From_Zero

;(PA2 = 1, PA3= ?)
From_One: ; this method/lable checks if the bit PA3 is zero if true it goes to the lable One_Zero_Status else it goes to One_One_Status
	btfsc PORTA, PA3
	goto One_One_Status
	goto One_Zero_Status
	
;(PA2 = 0, PA3= ?)
From_Zero: ; this method/lable checks if the bit PA3 is one if true it goes to the lable Zero_One_Status else it goes to Zero_Zero_Status
	btfss PORTA, PA3
	goto Zero_Zero_Status
	goto Zero_One_Status

;(PA2 = 1, PA3= 1) [ In this status we decide to show one all the time ]
One_One_Status: ; this method/lable checks if the bit PC0 is equals to one if true we don't have to make it one again to get a better continuous reading
	btfss PORTC, PC0 ; and it keeps chking the status by jumping to the lalbe CHECK_STATUS
	bsf PORTC, PC0
	goto CHECK_STATUS

;(PA2 = 1, PA3= 0)
One_Zero_Status:
	movlw 0x01 ; moving the value 1 to the register w for making xor with port C to invert the value from one to zero and vice versa
	xorwf PORTC, f 
	bcf INTCON, T0IF ; clearing the T0IF bit which means the Timer0 didn't reach zero yet (restart)
	movlw 0x8C
	movwf TMR0 ; moving the value 0x8c (d=140) to Time0 which gives us the needed frequence (10 KHz) according to this code and the devices in the lab
	goto wait ; jumping to the lable wait whick loops till the Timer0 return to 0

;(PA2 = 0, PA3= 0) [ In this status we decide to show zero all the time ]
Zero_Zero_Status: ; this method/lable checks if the bit PC0 is equals to zero if true we don't have to make it zero again to get a better continuous reading
	btfsc PORTC, PC0 ; and it keeps chking the status by jumping to the lalbe CHECK_STATUS
	bcf PORTC, PC0
	goto CHECK_STATUS

;(PA2 = 0, PA3= 1)
Zero_One_Status: 
	movlw 0x01 ; moving the value 1 to the register w for making xor with port C to invert the value from one to zero and vice versa
	xorwf PORTC, f 
	bcf INTCON, T0IF ; clearing the T0IF bit which means the Timer0 didn't reach zero yet (restart)
	movlw 0xF0
	movwf TMR0 ; moving the value 0xF0 (d=240) to Time0 which gives us the needed frequence (50 KHz) according to this code and the devices in the lab
	goto wait ; jumping to the lable wait whick loops till the Timer0 return to 0

wait: ; this method/lable keeps checking if the bit T0IF became one if true it jumps to the lable CHECK_STATUS else it keeps going ( just cheking ) till the bit T0FI become one
	btfsc INTCON, T0IF ; which means that the Time0 returned to zero
	goto CHECK_STATUS 
	goto wait
		

end