;*****************************************************************************
;	febuary 24 2012    Version 1.57
;				modified:
;                       
;					                   			FN_OBD_CAN_ENABLE			at  0BBh to enable  OBD_CAN librairie activity 
;										FN_OBD_CAN_DISABLE			at  0BCh to disable OBD_CAN librairie activity
;
;
;      July	19 2011    Version 1.56
;				Modifed:
;										SET_DTC_CODE				at 035h return 3 bytes instand of 4 bytes
;										AL_ALARM_ALERT				at 0B9h duplicated as FN_ALARM_TRIGGERED
;
;      July	15 2011    Version 1.55
;				Modified:
;										FN_START_STOP_REM			at 099h	rename to FN_START_REM
;										FN_START_REM				at 09Bh rename to FN_START_ONLY
;
;		July	11 2011	   Version 1.54
;				Modified:
;										SET_EXTENDED_STATUS 		at 075h  Bit2 of Byte3 in SET_EXTENDED_STATUS defined as bit of engine running state change
;																             Bit set means bit 6 of Byte 1 has changed value
;
;		June	28 2011	   Version 1.53
;				Adding code:
;										REQ_VIN						at 017h request the entire VIN data
;										AL_NEW_DTC_CODE				at 018h alert the new DTC code presents					
;
;				Modified:
;										REQ_MIL/SET_MIL renamed to REQ_ODO/SET_ODO
;
;		June	20 2011	   Version 1.51
;				Adding code:
;										FN_EXTEND_RUNTIME	   		at 027h  command to extend runtime. No effect if not in runtime mode
;										REQ_PROTOCOL_VER	   		at 049h  request protocol version
;										SET_PROTOCOL_VER	   		at 04Ah  return protocol version
;										AL_QUERY			   		at 04Bh  query alert status
;										REQ_MODULE_INFO		   		at 050h	 request module information
;										SET_PLATFORM_ID		   		at 051h	 return platform ID
;										SET_HW_VERSION		   		at 052h  return hardware version
;										SET_FW_ID			   		at 053h	 return firmware ID
;										SET_FW_VERSION		   		at 055h	 return firmware version
;										REQ_EXTENDED_STATUS_SUPPORT	at 056h  request extended support status
;										SET_EXTENDED_STATUS_SUPPORT	at 057h  return extended support status
;										REQ_RUNTIME			   		at 059h  request remaining runtime
;										SET_RUNTIME			   		at 05Ah	 return remaining runtime
;										AL_ALERT_STATUS				at 05Bh  return alert status
;										AL_FAILED_TO_UNLOCK			at 05Ch  return alert of failed to unlock
;										AL_FAILED_TO_START			at 05Dh  return alert of failed to start
;										AL_FAILED_TO_STOP			at 05Eh  return alert of failed to stop
;										AL_FAILED_TO_LOCK			at 05Fh  return alert of failed to lock
;										AL_ALERT_CLEAR				at 070h  function to clear all alert
;										SET_EXTENDED_STATUS			at 075h  return extended status
;										REQ_2WAY_STATUS				at 07Eh  request 2-way status
;										REQ_EXTENDED_STATUS			at 07Fh  request extended status
;										FN_START_REM				at 09Bh  function to start engine only
;
;				Modified:
;										AL_EIPS_SHUTDOWN_FAIL		at 073h  duplicated as SET_EIPS_SHUTDOWN_FAIL
;										AL_EIPS_ALERT				at 0D2h  duplicated as SET_EIPS_ALERT
;										AL_EIPS_SHUTDOWN			at 0D3h  duplicated as SET_EIPS_SHUTDOWN
;
;
;      March    8 2011    Version 1.13
;              Adding code:       
;                                      SET_EIPS_SHUTDOWN_FAIL at 073h  send command to notify the user the car still running after shutdown 
;                                      REQ_DTC_CODE           at 003h  request current dtc code
;                                      SET_DTC_CODE           at 035h  return 4 bytes (byte 1 is counter byte 2 to 4 is dtc code)
;
;
;      March    2  2011   Version 1.12
;              Adding code:       
;                                      SET_EIPS_ALERT    at 0D2h. SEND ALERT COMMAND TO NOTIFY THE USER THE CAR RUNNING WITHOUT SMARTKEY (EVERY XMIN)
;                                      SET_EIPS_SHUTDOWN at 0D3h. SEND SHUTDOWN COMMAND TO NOTIFY THE USER THE CAR SHUTDOWN AFTER XMIN WITHOUT SMARTKEY
;
;      December 16 2010   Version 1.11
;              Adding code:       
;                                      FN_VALET_TOGGLE at 0C7h. Each time this function is seen, the valet has been toggle through RF remote or Smart Start or ...
;    
;      September 14 2010  Version 1.10
;              Adding code:       
;                                      REQ_DTC         at 004h request number of dtc code
;                                      REQ_CLR_DTC     at 006h clear dtc code
;                                      REQ_FUEL_LEVEL  at 007h request fuel level (0% to 100%)
;                                      REQ_MIL         at 00Ah request millage (KM)
;                                      REQ_BAT_VOLT    at 00Bh request battery voltage 0 to 15.94
;                                      REQ_TPMS        at 00Eh request TPMS state
;
;                                      REQ_VIN_1       at 011h request vin digit 1  to 2
;                                      REQ_VIN_2       at 012h request vin digit 3  to 5
;                                      REQ_VIN_3       at 013h request vin digit 6  to 8 
;                                      REQ_VIN_4       at 014h request vin digit 9  to 11
;                                      REQ_VIN_5       at 015h request vin digit 12 to 14
;                                      REQ_VIN_6       at 016h request vin digit 15 to 17
;
;                                      SET_VIN_1       at 021h return 2 bytes digit 1  to 2  
;                                      SET_VIN_2       at 022h return 3 bytes digit 3  to 5  
;                                      SET_VIN_3       at 023h return 3 bytes digit 6  to 8  
;                                      SET_VIN_4       at 024h return 3 bytes digit 9  to 11 
;                                      SET_VIN_5       at 025h return 3 bytes digit 12 to 14 
;                                      SET_VIN_6       at 026h return 3 bytes digit 15 to 17
; 
;                                      SET_DTC         at 034h return 1 byte  (number of dtc code store)
;                                      SET_DTC_CLR     at 036h return 1 byte  (1 = success : 0 = false) 
;                                      SET_FUEL_LEVEL  at 037h return 1 byte  (0 = 0% : 100 = 100%)
;                                      SET_MIL         at 03Ah return 3 bytes (0 to 999999)
;                                      SET_BAT_VOLT    at 03Bh return 1 byte  (0 to 15.94 Volts) value/16 ex: 196/16 = 12.25V
;                                      SET_TPMS        at 03Eh return 1 byte  (bit 7 -> 1 = on : 0 = off)
;                                                                     
;
;      April 27th 2010     Version 1.09
;              Adding code:            FN_ALARM_TRIGGERED at 0B9h (mostly for Smart Start signaling)
;
;      July 29th 2009     Version 1.08
;              Adding code:            FN_CAR_IGN_ON at 0C3h and FN_CAR_IGN_OFF at 043h
;
;      August 25th 2008   Version 1.06
;              Adding code:            FN_CAR_FINDER at 0B8h
;                                      SET_HANDBRAKE_ON at 0CFh and SET_HANDBRAKE_OFF at 04Fh
;                          Also add HANDBRAKE in REQ_INPUT_STATUS BIT7
;      Updating Code:      Remove reserved code 043h, 045h, 046h, 050h, 052h
;
;      July 18th 2008     Version 1.051
;              Adding code:            Reserved code 031h, 032h, 033h
;
;      Nov 14th 2007      Version 1.05
;              Renaming Code:          Change SET_KEYLESS_UNLOCK (0DDH) to SET_KEYLESS_DISARM(0DDH) and change comment
;                                                      Change SET_KEYLESS_LOCK (0DEH) to SET_KEYLESS_ARM(0DEH) and change comment
;              Updating Code:          Add code SET_KEYLESS_UNLOCK (0D4H) and add comment
;                                                      Add code SET_KEYLESS_LOCK (0D5H) and add comment
;
;      Jan 3rd 2007    
;              Adding Code:            Add Keyless Panic 0D7h
;              Adding Code:            Add Keyless Aux3 0D6h
;              Updating Code:          Change Name of  SET_KEYLESS_4TH_BUTTON(0DCH) WITH SET_KEYLESS_AUX4(0DCH)
;
;      Dec 21st 2006
;              Updating Code:          Add reserve for Code 0D1H
;
;      Dec 14th 2006
;              Updating Code:          Add reserve for Code 0A9H
;
;      Dec 6th 2006    
;              Updating Code:          Add reserve for Code 0A8H
;
;      August 28th 2006        
;              Adding code:            RESERVE_09 & RESERVE_F6 this code is reserve for a Manufacturer
;
;      August 8th 2006 
;              Adding code:            Add reserve for Code 0D0H
;                                                      SET_KEYLESS_AUX1 for left/right slide
;                                                      SET_KEYLESS_AUX2 for left/right slide
;
;      July 16th 2006
;              Updating Comment:       modify comment in 0F0H
;
;      May 15th 2006   
;              Adding code:            REQ. vehicule speed(00Dh), send Vehicule Speed (0FAh)+ 1 byte, park/neutral sw. active(077h=No & 0F7h = yes),
;                                                      from keyless:start(0DAh), stop(0DBh), trunk(0DFh) & 4TH BUTTON(0DCh) OR ANY OTHER BUTTON
;              Updating code:          adding bit(6) for park/neutral SW. in STATUS(0FEh)
;                                                      FN_PARK_ON TO FN_PARKING_ON , FN_PARK_OFF TO FN_PARKING_OFF (to eliminate confusion between PARK/NEUTRAL and PARKING functions)
;              Updating Code:          engine temp(00Ah) to (005h)
;
;******************************************************************************/

FN_00                                 EQU 0x00 
REQ_INPUT_STATUS                      EQU 0x01 ;REQUEST STATUS FOR DRIVER DOOR,OTHER DOOR,TRUNK,HOOD & BRAKE 
FN_02                                 EQU 0x02 ;RESERVED
REQ_DTC_CODE                          EQU 0x03 ;REQUEST CURRENT DTC CODE
REQ_DTC                               EQU 0x04 ;REQUEST NUMBER OF DTC CODE
REQ_ENG_TEMP                          EQU 0x05 ;REQUEST ENGINE COOLANT TEMPERATURE
REQ_CLR_DTC                           EQU 0x06 ;CLEAR DTC CODE
REQ_FUEL_LEVEL                        EQU 0x07 ;REQUEST FUEL LEVEL (0% TO 100%)
FN_GWR_OFF                            EQU 0x08 ;GWR OFF
RESERVED_09                           EQU 0x09 ;RESERVED 
REQ_ODO                               EQU 0x0A ;REQUEST MILLAGE (KM)               
REQ_BAT_VOLT                          EQU 0x0B ;REQUEST BATTERY VOLTAGE 0 TO 15.94 
REQ_RPM                               EQU 0x0C ;REQUEST RPM 
REQ_SPEED                             EQU 0x0D ;REQUEST VEHICULE SPEED
REQ_TPMS                              EQU 0x0E ;REQUEST TPMS STATE
REQ_INT_TEMP                          EQU 0x0F ;REQUEST INTAKE AIR TEMPERATURE(OUTSIDE TEMP)
FN_PANIC_REM1_OFF                     EQU 0x10 ;FUNCTION PANIC OFF FROM REMOTE 1
REQ_VIN_1                             EQU 0x11 ;REQUEST VIN DIGIT 1  TO 2     
REQ_VIN_2                             EQU 0x12 ;REQUEST VIN DIGIT 3  TO 5     
REQ_VIN_3                             EQU 0x13 ;REQUEST VIN DIGIT 6  TO 8     
REQ_VIN_4                             EQU 0x14 ;REQUEST VIN DIGIT 9  TO 11    
REQ_VIN_5                             EQU 0x15 ;REQUEST VIN DIGIT 12 TO 14    
REQ_VIN_6                             EQU 0x16 ;REQUEST VIN DIGIT 15 TO 17    
REQ_VIN                               EQU 0x17	;REQ THE ENTIRE VIN DATA
AL_NEW_DTC_CODE                       EQU 0x18 ;ALERT THE NEW DTC CODE PRESENTS
RESERVED_19                           EQU 0x19 ;RESERVED
FN_1A                                 EQU 0x1A ;RESERVED
FN_1B                                 EQU 0x1B ;RESERVED
FN_1C                                 EQU 0x1C ;RESERVED
FN_1D                                 EQU 0x1D ;RESERVED
FN_1E                                 EQU 0x1E ;RESERVED
FN_1F                                 EQU 0x1F ;RESERVED
FN_PANIC_REM2_OFF                     EQU 0x20 ;FUNCTION PANIC OFF FROM REMOTE 2      
SET_VIN_1                             EQU 0x21 ;RETURN 2 BYTES DIGIT 1  TO 2   
SET_VIN_2                             EQU 0x22 ;RETURN 3 BYTES DIGIT 3  TO 5   
SET_VIN_3                             EQU 0x23 ;RETURN 3 BYTES DIGIT 6  TO 8   
SET_VIN_4                             EQU 0x24 ;RETURN 3 BYTES DIGIT 9  TO 11  
SET_VIN_5                             EQU 0x25 ;RETURN 3 BYTES DIGIT 12 TO 14  
SET_VIN_6                             EQU 0x26 ;RETURN 3 BYTES DIGIT 15 TO 17  
FN_EXTEND_RUNTIME                     EQU 0x27 ;FUNCTION EXTEND RUNTIME
FN_28                                 EQU 0x28 ;RESERVED
FN_29                                 EQU 0x29 ;RESERVED
FN_2A                                 EQU 0x2A ;RESERVED
FN_2B                                 EQU 0x2B ;RESERVED
FN_2C                                 EQU 0x2C ;RESERVED
FN_2D                                 EQU 0x2D ;RESERVED
FN_2E                                 EQU 0x2E ;RESERVED
FN_2F                                 EQU 0x2F ;RESERVED
FN_PANIC_REM3_OFF                     EQU 0x30 ;FUNCTION PANIC OFF FROM REMOTE 3
RESERVED_31                           EQU 0x31 ;RESERVED
RESERVED_32                           EQU 0x32 ;RESERVED
RESERVED_33                           EQU 0x33 ;RESERVED
SET_DTC                               EQU 0x34 ;RETURN 1 BYTE  (NUMBER OF DTC CODE STORE)
SET_DTC_CODE                          EQU 0x35 ;RETURN 3 BYTES (BYTE1 =  # OF CODE STORE) (BYTE 2 = BIT7:6 INDICATES LETTER P,C,U..., BIT5:4 FIRST DIGITAL OF DTC ) 
;                                                   BIT3:0 OF BYTE 2 AND BYTE 3 INDICATE THREE HEX CHARACTERS. EX: 0x12 0x01 0x26 (12"P"0126) = CODE 1 OF 2 IS P0126
SET_DTC_CLR                           EQU 0x36 ;RETURN 1 BYTE  (1 = SUCCESS : 0 = FALSE)
SET_FUEL_LEVEL                        EQU 0x37 ;RETURN 1 BYTE  (0 = 0% : 100 = 100%)        
FN_38                                 EQU 0x38 ;RESERVED
FN_39                                 EQU 0x39 ;RESERVED
SET_ODO                               EQU 0x3A ;RETURN 3 BYTES (0 TO 999999)
SET_BAT_VOLT                          EQU 0x3B ;RETURN 1 BYTE  (0 TO 15.94 VOLTS) VALUE/16 EX: 196/16 = 12.25V
FN_3C                                 EQU 0x3C ;RESERVED
FN_3D                                 EQU 0x3D ;RESERVED
SET_TPMS                              EQU 0x3E ;RETURN 1 BYTE  (BIT 7-> 1 = ON : 0 = OFF)
FN_3F                                 EQU 0x3F ;RESERVED
FN_DOME_OFF                           EQU 0x40 ;FUNCTION DOME OFF FROM REMOTE STARTER
FN_ACC_OFF                            EQU 0x41 ;FUNCTION ACC OFF FROM REMOTE STARTER
FN_IGN_OFF                            EQU 0x42 ;FUNCTION IGNITION OFF FROM REMOTE STARTER
FN_CAR_IGN_OFF                        EQU 0x43 ;FUNCTION CAR IGNITION OFF DETECTED BY IGNITION SENSE INPUT OF REMOTE STARTER
FN_START_OFF                          EQU 0x44 ;FUNCTION STARTER OFF FROM REMOTE STARTER
FN_45                                 EQU 0x45 ;RESERVED
FN_46                                 EQU 0x46 ;RESERVED
FN_47                                 EQU 0x47 ;RESERVED
FN_PARKING_OFF                        EQU 0x48 ;FUNCTION PARKING OFF FROM REMOTE STARTER
REQ_PROTOCOL_VER                      EQU 0x49 ;REQUEST D2D PROTOCOL VERSION
SET_PROTOCOL_VER                      EQU 0x4A ;RETURN D2D PROTOCOL VERSION
AL_QUERY                              EQU 0x4B ;QUERY ALERT STATUS
FN_4C                                 EQU 0x4C ;RESERVED
FN_4D                                 EQU 0x4D ;RESERVED
FN_4E                                 EQU 0x4E ;RESERVED
SET_HANDBRAKE_OFF                     EQU 0x4F ;HANDBRAKE IS DISENGAGED
REQ_MODULE_INFO                       EQU 0x50 ;REQUEST MODULE INFORMATION
SET_PLATFORM_ID                       EQU 0x51 ;RETURN PLATFORM ID
SET_HW_VERSION                        EQU 0x52 ;RETURN HARDWARE VERSION
SET_FW_ID                             EQU 0x53 ;RETURN FIRMWARE ID
RESERVED_54                           EQU 0x54 ;RESERVED
SET_FW_VERSION                        EQU 0x55 ;RETURN FIRMWARE VERSION
REQ_EXTENDED_STATUS_SUPPORT           EQU 0x56 ;REQUEST EXTENDED STATUS SUPPORT
SET_EXTENDED_STATUS_SUPPORT           EQU 0x57 ;RETURN EXTENDED STATUS SUPPORT
RESERVED_58                           EQU 0x58 ;RESERVED
REQ_RUNTIME                           EQU 0x59 ;REQUEST REMAINING ENGINE RUNTIME
SET_RUNTIME                           EQU 0x5A ;RETURN REMAINING ENGINE RUNTIME
AL_ALERT_STATUS                       EQU 0x5B ;RETURN ALERT STATUS
AL_FAILED_TO_UNLOCK                   EQU 0x5C ;RETURN ALERT OF FAILED TO UNLOCK
AL_FAILED_TO_START                    EQU 0x5D ;RETURN ALERT OF FAILED TO START
AL_FAILED_TO_STOP                     EQU 0x5E ;RETURN ALERT OF FAILED TO STOP
AL_FAILED_TO_LOCK                     EQU 0x5F ;RETURN ALERT OF FAILED TO LOCK
FN_60                                 EQU 0x60 ;RESERVED FOR ENCRYPTION       
FN_61                                 EQU 0x61 ;RESERVED FOR ENCRYPTION
FN_62                                 EQU 0x62 ;RESERVED FOR ENCRYPTION       
FN_63                                 EQU 0x63 ;RESERVED FOR ENCRYPTION       
FN_64                                 EQU 0x64 ;RESERVED FOR ENCRYPTION       
FN_65                                 EQU 0x65 ;RESERVED FOR ENCRYPTION       
FN_66                                 EQU 0x66 ;RESERVED FOR ENCRYPTION       
FN_67                                 EQU 0x67 ;RESERVED FOR ENCRYPTION       
FN_68                                 EQU 0x68 ;RESERVED FOR ENCRYPTION       
FN_69                                 EQU 0x69 ;RESERVED FOR ENCRYPTION       
FN_6A                                 EQU 0x6A ;RESERVED FOR ENCRYPTION       
FN_6B                                 EQU 0x6B ;RESERVED FOR ENCRYPTION       
FN_6C                                 EQU 0x6C ;RESERVED FOR ENCRYPTION       
FN_6D                                 EQU 0x6D ;RESERVED FOR ENCRYPTION       
FN_6E                                 EQU 0x6E ;RESERVED FOR ENCRYPTION       
FN_6F                                 EQU 0x6F ;RESERVED FOR ENCRYPTION       
AL_ALERT_CLEAR                        EQU 0x70 ;CLEAR ALL PREVIOUS ALERT        
SET_HOOD_CLOSE                        EQU 0x71 ;SEND HOOD CLOSE TO REMOTE STARTER
SET_TRUNK_CLOSE                       EQU 0x72 ;SEND TRUNK CLOSE TO REMOTE STARTER
AL_EIPS_SHUTDOWN_FAIL                 EQU 0x73 ;SEND NOTIFICATION TO USER THATS THE CAR STILL RUNNING AFTER SHUTDOWN
SET_EIPS_SHUTDOWN_FAIL                EQU AL_EIPS_SHUTDOWN_FAIL ;duplicated
SET_DRIVER_CLOSE                      EQU 0x74 ;SEND DRIVER CLOSE TO REMOTE STARTER
SET_EXTENDED_STATUS                   EQU 0x75 ;RETURN UPDATED STATUS OF VEHICLE
;                              BYTE 1:
;                                      BIT0 = LOCK                     (1: LOCK)
;                                      BIT1 = ARMED                    (1: ARMED)
;                                      BIT2 = IGNITION         		(1: ON)
;                                      BIT3 = PARKING  LIGHT			(1: ON)
;                                      BIT4 = SIREN 					(1: ON)
;                                      BIT5 = VALET MODE       		(1: ACTIVATED)
;                                      BIT6 = START SUCCESSFULLY		(1: ENGINE IS RUNNING)
;                                      BIT7 = PAGING,CAR FIND OR PANIC (1: ON)
;                              BYTE 2:
;                                      BIT0 = DOOR TRIGGER             (1: OPEN)
;                                      BIT1 = TRUNK TRIGGER            (1: OPEN)
;                                      BIT2 = HOOD TRIGGER             (1: OPEN)
;                                      BIT3 = WARN AWAY        		(1: TRIGGERED)
;                                      BIT4 = SHOCK TRIGGER    		(1: TRIGGERED)
;                                      BIT5 = READY MODE				(1: NOT READY)
;                                      BIT6 = RESERVED
;                                      BIT7 = RESERVED
;                              BYTE 3:
;                                      BIT0 = DTC CODE PRESET				(1: YES)
;                                      BIT1 = ALERT CONDITION				(1: YES)
;                                      BIT2 = ENGINE RUNNING STATE CHANGE	(1: BIT6 OF BYTE1 VALUE CHANGED)
;                                      BIT3 = RESERVED
;                                      BIT4 = RESERVED
;                                      BIT5 = RESERVED
;                                      BIT6 = RESERVED
;                                      BIT7 = RESRVERD
;
FN_76                                 EQU 0x76 ;RESERVED
SET_PRK_NTRL_NOT_ACTIVE               EQU 0x77 ;SEND TRANSMISSION PARK/NEUTRAL SW ACTIVE = NO
SET_OTHER_CLOSE                       EQU 0x78 ;SEND OTHER DOOR CLOSE TO REMOTE STARTER
SET_BRAKE_OFF                         EQU 0x79 ;SEND BRAKE PEDAL RELASE TO REMOTE STARTER
FN_7A                                 EQU 0x7A ;RESERVED
SET_GLOWPLUG_OFF                      EQU 0x7B ;SEND GLOW PLUG OFF TO REMOTE STARTER
SET_KEY_OUT                           EQU 0x7C ;SEND KEY OUT TO REMOTE STARTER
SET_IGN_OFF                           EQU 0x7D ;SEND IGNITION OFF TO REMOTE STARTER
REQ_2WAY_STATUS                       EQU 0x7E ;REQUEST UPDATED 2-WAY STATUS
REQ_EXTENDED_STATUS                   EQU 0x7F ;REQUEST UPDATED STATUS OF VEHICLE
FN_80                                 EQU 0x80 ;RESERVED
FN_ARM                                EQU 0x81 ;FUNCTION ARM ONLY
FN_DISARM                             EQU 0x82 ;FUNCTION DISARM ONLY
FN_83                                 EQU 0x83 ;RESERVED
FN_84                                 EQU 0x84 ;RESERVED
FN_85                                 EQU 0x85 ;RESERVED
FN_86                                 EQU 0x86 ;RESERVED
FN_87                                 EQU 0x87 ;RESERVED
FN_GWR_ON                             EQU 0x88 ;FUNCTION GROUND WHEN RUNNING ON
FN_89                                 EQU 0x89 ;RESERVED
FN_8A                                 EQU 0x8A ;RESERVED
FN_8B                                 EQU 0x8B ;RESERVED
FN_8C                                 EQU 0x8C ;RESERVED
FN_8D                                 EQU 0x8D ;RESERVED
FN_8E                                 EQU 0x8E ;RESERVED
FN_8F                                 EQU 0x8F ;RESERVED
FN_PANIC_REM1_ON                      EQU 0x90 ;FUNCTION PANIC ON FROM REMOTE 1
FN_LOCK_REM1_ON                       EQU 0x91 ;FUNCTION LOCK ON FROM REMOTE 1
FN_UNLOCK_DR_REM1_ON                  EQU 0x92 ;FUNCTION UNLOCK DRIVER ON FROM REMOTE 1
FN_AUX1_REM1_ON                       EQU 0x93 ;FUNCTION AUX1 ON FROM REMOTE 1
FN_TRUNK_REM1_ON                      EQU 0x94 ;FUNCTION TRUNK ON FROM REMOTE 1
FN_AUX2_REM1_ON                       EQU 0x95 ;FUNCTION AUX2 ON FROM REMOTE 1
FN_AUX3_REM1_ON                       EQU 0x96 ;FUNCTION AUX3 ON FROM REMOTE 1
FN_AUX4_REM1_ON                       EQU 0x97 ;FUNCTION AUX4 ON FROM REMOTE 1
FN_CHIRP                              EQU 0x98 ;FUNCTION CHIRP FROM REMOTE 1
RESERVED_99                           EQU 0x99 ;RESERVED
FN_UNLOCK_ALL_REM1_ON                 EQU 0x9A ;FUNCTION UNLOCK ALL ON FROM REMOTE 1  
FN_START_ONLY                         EQU 0x9B ;FUNCTION START ENGINE ONLY
FN_9C                                 EQU 0x9C ;RESERVED
FN_9D                                 EQU 0x9D ;RESERVED
FN_9E                                 EQU 0x9E ;RESERVED
FN_9F                                 EQU 0x9F ;RESERVED
FN_PANIC_REM2_ON                      EQU 0xA0 ;FUNCTION PANIC ON FROM REMOTE 2
FN_LOCK_REM2_ON                       EQU 0xA1 ;FUNCTION LOCK ON FROM REMOTE 2
FN_UNLOCK_DR_REM2_ON                  EQU 0xA2 ;FUNCTION UNLOCK DRIVER ON FROM REMOTE 2
FN_AUX1_REM2_ON                       EQU 0xA3 ;FUNCTION AUX1 ON FROM REMOTE 2
FN_TRUNK_REM2_ON                      EQU 0xA4 ;FUNCTION TRUNK ON FROM REMOTE 2
FN_AUX2_REM2_ON                       EQU 0xA5 ;FUNCTION AUX2 ON FROM REMOTE 2
FN_AUX3_REM2_ON                       EQU 0xA6 ;FUNCTION AUX3 ON FROM REMOTE 2
FN_AUX4_REM2_ON                       EQU 0xA7 ;FUNCTION AUX4 ON FROM REMOTE 2
RESERVED_A8                           EQU 0xA8 ;RESERVED
RESERVED_A9                           EQU 0xA9 ;RESERVED
FN_UNLOCK_ALL_REM2_ON                 EQU 0xAA ;FUNCTION UNLOCK ALL ON FROM REMOTE 2  
FN_AB                                 EQU 0xAB ;RESERVED
FN_AC                                 EQU 0xAC ;RESERVED
FN_AD                                 EQU 0xAD ;RESERVED
FN_AE                                 EQU 0xAE ;RESERVED
FN_AF                                 EQU 0xAF ;RESERVED
FN_PANIC_REM3_ON                      EQU 0xB0 ;FUNCTION PANIC ON FROM REMOTE 3
FN_LOCK_REM3_ON                       EQU 0xB1 ;FUNCTION LOCK ON FROM REMOTE 3
FN_UNLOCK_DR_REM3_ON                  EQU 0xB2 ;FUNCTION UNLOCK DRIVER ON FROM REMOTE 3
FN_AUX1_REM3_ON                       EQU 0xB3 ;FUNCTION AUX1 ON FROM REMOTE 3
FN_TRUNK_REM3_ON                      EQU 0xB4 ;FUNCTION TRUNK ON FROM REMOTE 3
FN_AUX2_REM3_ON                       EQU 0xB5 ;FUNCTION AUX2 ON FROM REMOTE 3
FN_AUX3_REM3_ON                       EQU 0xB6 ;FUNCTION AUX2 ON FROM REMOTE 3
FN_AUX4_REM3_ON                       EQU 0xB7 ;FUNCTION AUX3 ON FROM REMOTE 3
FN_CAR_FINDER                         EQU 0xB8 ;FUNCTION CAR FINDER
FN_ALARM_TRIGGERED                    EQU 0xB9 ;FUNCTION ALARM TRIGGERED FROM REMOTE STARTER  (mostly for Smart Start signaling)
AL_ALARM_ALERT                        EQU FN_ALARM_TRIGGERED ;duplicated
FN_UNLOCK_ALL_REM3_ON                 EQU 0xBA ;FUNCTION UNLOCK ALL ON FROM REMOTE 3  
RESERVED_BB                           EQU 0xBB ;RESERVED
RESERVED_BC                           EQU 0xBC ;RESERVED
FN_BC                                 EQU 0xBC ;RESERVED
FN_BD                                 EQU 0xBD ;RESERVED
FN_BE                                 EQU 0xBE ;RESERVED
FN_BF                                 EQU 0xBF ;RESERVED
FN_DOME_ON                            EQU 0xC0 ;FUNCTION DOME ON FROM REMOTE STARTER
FN_ACC_ON                             EQU 0xC1 ;FUNCTION ACC ON FROM REMOTE STARTER
FN_IGN_ON                             EQU 0xC2 ;FUNCTION IGNITION ON FROM REMOTE STARTER
FN_CAR_IGN_ON                         EQU 0xC3 ;FUNCTION CAR IGNITION ON DETECTED BY IGNITION SENSE INPUT OF REMOTE STARTER
FN_START_ON                           EQU 0xC4 ;FUNCTION STARTER ON FROM REMOTE STARTER
FN_C5                                 EQU 0xC5 ;RESERVED
FN_C6                                 EQU 0xC6 ;RESERVED
FN_VALET_TOGGLE                       EQU 0xC7 ;FUNCTION VALET HAS BEEN TOGGLED FROM REMOTE OR SMART START OR ELSE
FN_PARKING_ON                         EQU 0xC8 ;FUNCTION PARKING ON FROM REMOTE STARTER
FN_C9                                 EQU 0xC9 ;RESERVED
FN_CA                                 EQU 0xCA ;RESERVED
FN_CB                                 EQU 0xCB ;RESERVED
FN_CC                                 EQU 0xCC ;RESERVED
FN_CD                                 EQU 0xCD ;RESERVED
FN_CE                                 EQU 0xCE ;RESERVED
SET_HANDBRAKE_ON                      EQU 0xCF ;HANDBRAKE IS ENGAGED
RESERVED_D0                           EQU 0xD0 ;RESERVED
RESERVED_D1                           EQU 0xD1 ;RESERVED
AL_EIPS_ALERT                         EQU 0xD2 ;SEND ALERT COMMAND TO NOTIFY THE USER THE CAR RUNNING WITHOUT SMARTKEY (EVERY XMIN)
SET_EIPS_ALERT                        EQU AL_EIPS_ALERT  ;DUPLICATED
AL_EIPS_SHUTDOWN                      EQU 0xD3 ;SEND SHUTDOWN COMMAND TO NOTIFY THE USER THE CAR SHUTDOWN AFTER XMIN WITHOUT SMARTKEY
SET_EIPS_SHUTDOWN                     EQU AL_EIPS_SHUTDOWN  ;DUPLICATED
SET_KEYLESS_UNLOCK                    EQU 0xD4 ;SEND UNLOCK FROM FACTORY KEYLESS
SET_KEYLESS_LOCK                      EQU 0xD5 ;SEND LOCK FROM FACTORY KEYLESS        
SET_KEYLESS_AUX3                      EQU 0xD6 ;SEND AUX4 FROM FACTORY KEYLESS(GAZ DOOR ,SECOND TRUNK OR ANY 4TH BUTTON)
SET_KEYLESS_PANIC                     EQU 0xD7 ;SEND PANIC FROM FACTORY KEYLESS
SET_KEYLESS_AUX1                      EQU 0xD8 ;SEND AUX1 FROM FACTORY KEYLESS (LEFT/RIGHT SLIDE)
SET_KEYLESS_AUX2                      EQU 0xD9 ;SEND AUX2 FROM FACTORY KEYLESS (LEFT/RIGHT SLIDE)
SET_KEYLESS_START                     EQU 0xDA ;SEND START FROM FACTORY KEYLESS
SET_KEYLESS_STOP                      EQU 0xDB ;SEND STOP FROM FACTORY KEYLESS 
SET_KEYLESS_AUX4                      EQU 0xDC ;SEND AUX4 FROM FACTORY KEYLESS(GAZ DOOR ,SECOND TRUNK OR ANY 4TH BUTTON)
SET_KEYLESS_DISARM                    EQU 0xDD ;SEND DISARM FROM FACTORY KEYLESS TO DISARM AFTERMARKET ALARM IF THE REMOTE IS LOSS OR BROKEN
SET_KEYLESS_ARM                       EQU 0xDE ;SEND ARM FROM FACTORY KEYLESS TO ARM AFTERMARKET ALARM IF THE REMOTE IS LOSS OR BROKEN        
SET_KEYLESS_TRUNK                     EQU 0xDF ;SEND TRUNK FROM FACTORY KEYLESS
FN_E0                                 EQU 0xE0 ;RESERVED FOR ENCRYPTION       
FN_E1                                 EQU 0xE1 ;RESERVED FOR ENCRYPTION       
FN_E2                                 EQU 0xE2 ;RESERVED FOR ENCRYPTION       
FN_E3                                 EQU 0xE3 ;RESERVED FOR ENCRYPTION       
FN_E4                                 EQU 0xE4 ;RESERVED FOR ENCRYPTION       
FN_E5                                 EQU 0xE5 ;RESERVED FOR ENCRYPTION       
FN_E6                                 EQU 0xE6 ;RESERVED FOR ENCRYPTION       
FN_E7                                 EQU 0xE7 ;RESERVED FOR ENCRYPTION       
FN_E8                                 EQU 0xE8 ;RESERVED FOR ENCRYPTION       
FN_E9                                 EQU 0xE9 ;RESERVED FOR ENCRYPTION       
FN_EA                                 EQU 0xEA ;RESERVED FOR ENCRYPTION       
FN_EB                                 EQU 0xEB ;RESERVED FOR ENCRYPTION       
FN_EC                                 EQU 0xEC ;RESERVED FOR ENCRYPTION       
FN_ED                                 EQU 0xED ;RESERVED FOR ENCRYPTION       
FN_EE                                 EQU 0xEE ;RESERVED FOR ENCRYPTION       
FN_EF                                 EQU 0xEF ;RESERVED FOR ENCRYPTION       
SET_INT_TEMP                          EQU 0xF0 ;SEND INTAKE AIR TEMPERATURE OR OUTSIDE TEMP (1 BYTE) TO REMOTE STARTER 
SET_HOOD_OPEN                         EQU 0xF1 ;SEND HOOD OPEN TO REMOTE STARTER
SET_TRUNK_OPEN                        EQU 0xF2 ;SEND TRUNK OPEN TO REMOTE STARTER
SET_RPM                               EQU 0xF3 ;SEND ENGINE RPM (2BYTES)  TO REMOTE STARTER EX:0 F3H + 003H + 0E8H = 1000RPM 
SET_DRIVER_OPEN                       EQU 0xF4 ;SEND DRIVER DOOR OPEN TO REMOTE STARTER
SET_ENG_TEMP                          EQU 0xF5 ;SEND ENGINE COOLANT  TEMPERATURE  TO REMOTE STARTER EX: 0F5H + MIN: 0 = -40C MAX: 0FFH = 215C
RESERVED_F6                           EQU 0xF6 ;RESERVED 
SET_PRK_NTRL_ACTIVE                   EQU 0xF7 ;SEND TRANSMISSION PARK/NEUTRAL SW ACTIVE = YES
SET_OTHER_OPEN                        EQU 0xF8 ;SEND OTHER DOOR OPEN TO REMOTE STARTER
SET_BRAKE_ON                          EQU 0xF9 ;SEND BRAKE PEDAL ENGAGE(ON) TO REMOTE STARTER
SET_SPEED                             EQU 0xFA ;SEND VEHICULE SPEED, RETURN 1 BYTE (0 TO 255 km/h)
SET_GLOWPLUG_ON                       EQU 0xFB ;SEND GLOW PLUG(ON) TO REMOTE STARTER
SET_KEY_IN                            EQU 0xFC ;SEND KEY IN TO REMOTE STARTER
SET_IGN_ON                            EQU 0xFD ;SEND IGNITION ON TO REMOTE STARTER
SET_STATUS                            EQU 0xFE ;SEND STATUS BIT TO REMOTE  + 1BYTE(QUERY SUPPORT) + 1 BYTE(STATUS)
;                              BYTE1 QUERY SUPPORT     1= SUPPORT 0= NOT SUPPORT
;                                      BIT0 = DRIVER DOOR      
;                                      BIT1 = OTHER DOOR (RF + LR + RR)
;                                      BIT2 = TRUNK            
;                                      BIT3 = HOOD             
;                                      BIT4 = BRAKE            
;                                      BIT5 = KEYSENS
;                                      BIT6 = PARK/NEUTRAL SWITCH ACTIVE
;                                      BIT7 = HANDBRAKE
;                              BYTE STATUS
;                                      BIT0 = DRIVER DOOR                      1= OPEN   0= CLOSE
;                                      BIT1 = OTHER DOOR (RF + LR + RR)        1= OPEN   0= CLOSE
;                                      BIT2 = TRUNK                            1= OPEN   0= CLOSE
;                                      BIT3 = HOOD                             1= OPEN   0= CLOSE
;                                      BIT4 = BRAKE                            1= ENGAGE 0= RELEASE
;                                      BIT5 = KEYSENS                          1= KEY IN 0= KEY OUT
;                                      BIT6 = PARK/NEUTRAL SWITCH              1= ACTIVE 0 = NOT ACTIVE                        
;                                      BIT7 = HANDBRAKE            1= ENGAGE 0= RELEASE
;
FN_FF                                 EQU 0xFF
