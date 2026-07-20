//*****************************************************************************
//	febuary 24 2012    Version 1.57
//				modified:
//                       
//					                   			FN_OBD_CAN_ENABLE			at  0BBh to enable  OBD_CAN librairie activity 
//										FN_OBD_CAN_DISABLE			at  0BCh to disable OBD_CAN librairie activity
//
//
//      July	19 2011    Version 1.56
//				Modifed:
//										SET_DTC_CODE				at 035h return 3 bytes instand of 4 bytes
//										AL_ALARM_ALERT				at 0B9h duplicated as FN_ALARM_TRIGGERED
//
//      July	15 2011    Version 1.55
//				Modified:
//										FN_START_STOP_REM			at 099h	rename to FN_START_REM
//										FN_START_REM				at 09Bh rename to FN_START_ONLY
//
//		July	11 2011	   Version 1.54
//				Modified:
//										SET_EXTENDED_STATUS 		at 075h  Bit2 of Byte3 in SET_EXTENDED_STATUS defined as bit of engine running state change
//																             Bit set means bit 6 of Byte 1 has changed value
//
//		June	28 2011	   Version 1.53
//				Adding code:
//										REQ_VIN						at 017h request the entire VIN data
//										AL_NEW_DTC_CODE				at 018h alert the new DTC code presents					
//
//				Modified:
//										REQ_MIL/SET_MIL renamed to REQ_ODO/SET_ODO
//
//		June	20 2011	   Version 1.51
//				Adding code:
//										FN_EXTEND_RUNTIME	   		at 027h  command to extend runtime. No effect if not in runtime mode
//										REQ_PROTOCOL_VER	   		at 049h  request protocol version
//										SET_PROTOCOL_VER	   		at 04Ah  return protocol version
//										AL_QUERY			   		at 04Bh  query alert status
//										REQ_MODULE_INFO		   		at 050h	 request module information
//										SET_PLATFORM_ID		   		at 051h	 return platform ID
//										SET_HW_VERSION		   		at 052h  return hardware version
//										SET_FW_ID			   		at 053h	 return firmware ID
//										SET_FW_VERSION		   		at 055h	 return firmware version
//										REQ_EXTENDED_STATUS_SUPPORT	at 056h  request extended support status
//										SET_EXTENDED_STATUS_SUPPORT	at 057h  return extended support status
//										REQ_RUNTIME			   		at 059h  request remaining runtime
//										SET_RUNTIME			   		at 05Ah	 return remaining runtime
//										AL_ALERT_STATUS				at 05Bh  return alert status
//										AL_FAILED_TO_UNLOCK			at 05Ch  return alert of failed to unlock
//										AL_FAILED_TO_START			at 05Dh  return alert of failed to start
//										AL_FAILED_TO_STOP			at 05Eh  return alert of failed to stop
//										AL_FAILED_TO_LOCK			at 05Fh  return alert of failed to lock
//										AL_ALERT_CLEAR				at 070h  function to clear all alert
//										SET_EXTENDED_STATUS			at 075h  return extended status
//										REQ_2WAY_STATUS				at 07Eh  request 2-way status
//										REQ_EXTENDED_STATUS			at 07Fh  request extended status
//										FN_START_REM				at 09Bh  function to start engine only
//
//				Modified:
//										AL_EIPS_SHUTDOWN_FAIL		at 073h  duplicated as SET_EIPS_SHUTDOWN_FAIL
//										AL_EIPS_ALERT				at 0D2h  duplicated as SET_EIPS_ALERT
//										AL_EIPS_SHUTDOWN			at 0D3h  duplicated as SET_EIPS_SHUTDOWN
//
//
//      March    8 2011    Version 1.13
//              Adding code:       
//                                      SET_EIPS_SHUTDOWN_FAIL at 073h  send command to notify the user the car still running after shutdown 
//                                      REQ_DTC_CODE           at 003h  request current dtc code
//                                      SET_DTC_CODE           at 035h  return 4 bytes (byte 1 is counter byte 2 to 4 is dtc code)
//
//
//      March    2  2011   Version 1.12
//              Adding code:       
//                                      SET_EIPS_ALERT    at 0D2h. SEND ALERT COMMAND TO NOTIFY THE USER THE CAR RUNNING WITHOUT SMARTKEY (EVERY XMIN)
//                                      SET_EIPS_SHUTDOWN at 0D3h. SEND SHUTDOWN COMMAND TO NOTIFY THE USER THE CAR SHUTDOWN AFTER XMIN WITHOUT SMARTKEY
//
//      December 16 2010   Version 1.11
//              Adding code:       
//                                      FN_VALET_TOGGLE at 0C7h. Each time this function is seen, the valet has been toggle through RF remote or Smart Start or ...
//    
//      September 14 2010  Version 1.10
//              Adding code:       
//                                      REQ_DTC         at 004h request number of dtc code
//                                      REQ_CLR_DTC     at 006h clear dtc code
//                                      REQ_FUEL_LEVEL  at 007h request fuel level (0% to 100%)
//                                      REQ_MIL         at 00Ah request millage (KM)
//                                      REQ_BAT_VOLT    at 00Bh request battery voltage 0 to 15.94
//                                      REQ_TPMS        at 00Eh request TPMS state
//
//                                      REQ_VIN_1       at 011h request vin digit 1  to 2
//                                      REQ_VIN_2       at 012h request vin digit 3  to 5
//                                      REQ_VIN_3       at 013h request vin digit 6  to 8 
//                                      REQ_VIN_4       at 014h request vin digit 9  to 11
//                                      REQ_VIN_5       at 015h request vin digit 12 to 14
//                                      REQ_VIN_6       at 016h request vin digit 15 to 17
//
//                                      SET_VIN_1       at 021h return 2 bytes digit 1  to 2  
//                                      SET_VIN_2       at 022h return 3 bytes digit 3  to 5  
//                                      SET_VIN_3       at 023h return 3 bytes digit 6  to 8  
//                                      SET_VIN_4       at 024h return 3 bytes digit 9  to 11 
//                                      SET_VIN_5       at 025h return 3 bytes digit 12 to 14 
//                                      SET_VIN_6       at 026h return 3 bytes digit 15 to 17
// 
//                                      SET_DTC         at 034h return 1 byte  (number of dtc code store)
//                                      SET_DTC_CLR     at 036h return 1 byte  (1 = success : 0 = false) 
//                                      SET_FUEL_LEVEL  at 037h return 1 byte  (0 = 0% : 100 = 100%)
//                                      SET_MIL         at 03Ah return 3 bytes (0 to 999999)
//                                      SET_BAT_VOLT    at 03Bh return 1 byte  (0 to 15.94 Volts) value/16 ex: 196/16 = 12.25V
//                                      SET_TPMS        at 03Eh return 1 byte  (bit 7 -> 1 = on : 0 = off)
//                                                                     
//
//      April 27th 2010     Version 1.09
//              Adding code:            FN_ALARM_TRIGGERED at 0B9h (mostly for Smart Start signaling)
//
//      July 29th 2009     Version 1.08
//              Adding code:            FN_CAR_IGN_ON at 0C3h and FN_CAR_IGN_OFF at 043h
//
//      August 25th 2008   Version 1.06
//              Adding code:            FN_CAR_FINDER at 0B8h
//                                      SET_HANDBRAKE_ON at 0CFh and SET_HANDBRAKE_OFF at 04Fh
//                          Also add HANDBRAKE in REQ_INPUT_STATUS BIT7
//      Updating Code:      Remove reserved code 043h, 045h, 046h, 050h, 052h
//
//      July 18th 2008     Version 1.051
//              Adding code:            Reserved code 031h, 032h, 033h
//
//      Nov 14th 2007      Version 1.05
//              Renaming Code:          Change SET_KEYLESS_UNLOCK (0DDH) to SET_KEYLESS_DISARM(0DDH) and change comment
//                                                      Change SET_KEYLESS_LOCK (0DEH) to SET_KEYLESS_ARM(0DEH) and change comment
//              Updating Code:          Add code SET_KEYLESS_UNLOCK (0D4H) and add comment
//                                                      Add code SET_KEYLESS_LOCK (0D5H) and add comment
//
//      Jan 3rd 2007    
//              Adding Code:            Add Keyless Panic 0D7h
//              Adding Code:            Add Keyless Aux3 0D6h
//              Updating Code:          Change Name of  SET_KEYLESS_4TH_BUTTON(0DCH) WITH SET_KEYLESS_AUX4(0DCH)
//
//      Dec 21st 2006
//              Updating Code:          Add reserve for Code 0D1H
//
//      Dec 14th 2006
//              Updating Code:          Add reserve for Code 0A9H
//
//      Dec 6th 2006    
//              Updating Code:          Add reserve for Code 0A8H
//
//      August 28th 2006        
//              Adding code:            RESERVE_09 & RESERVE_F6 this code is reserve for a Manufacturer
//
//      August 8th 2006 
//              Adding code:            Add reserve for Code 0D0H
//                                                      SET_KEYLESS_AUX1 for left/right slide
//                                                      SET_KEYLESS_AUX2 for left/right slide
//
//      July 16th 2006
//              Updating Comment:       modify comment in 0F0H
//
//      May 15th 2006   
//              Adding code:            REQ. vehicule speed(00Dh), send Vehicule Speed (0FAh)+ 1 byte, park/neutral sw. active(077h=No & 0F7h = yes),
//                                                      from keyless:start(0DAh), stop(0DBh), trunk(0DFh) & 4TH BUTTON(0DCh) OR ANY OTHER BUTTON
//              Updating code:          adding bit(6) for park/neutral SW. in STATUS(0FEh)
//                                                      FN_PARK_ON TO FN_PARKING_ON , FN_PARK_OFF TO FN_PARKING_OFF (to eliminate confusion between PARK/NEUTRAL and PARKING functions)
//              Updating Code:          engine temp(00Ah) to (005h)
//
//******************************************************************************/

#define FN_00                                  0x00 
#define REQ_INPUT_STATUS                       0x01 //REQUEST STATUS FOR DRIVER DOOR,OTHER DOOR,TRUNK,HOOD & BRAKE 
#define FN_02                                  0x02 //RESERVED
#define REQ_DTC_CODE                           0x03 //REQUEST CURRENT DTC CODE
#define REQ_DTC                                0x04 //REQUEST NUMBER OF DTC CODE
#define REQ_ENG_TEMP                           0x05 //REQUEST ENGINE COOLANT TEMPERATURE
#define REQ_CLR_DTC                            0x06 //CLEAR DTC CODE
#define REQ_FUEL_LEVEL                         0x07 //REQUEST FUEL LEVEL (0% TO 100%)
#define FN_GWR_OFF                             0x08 //GWR OFF
#define RESERVED_09                            0x09 //RESERVED 
#define REQ_ODO                                0x0A //REQUEST MILLAGE (KM)               
#define REQ_BAT_VOLT                           0x0B //REQUEST BATTERY VOLTAGE 0 TO 15.94 
#define REQ_RPM                                0x0C //REQUEST RPM 
#define REQ_SPEED                              0x0D //REQUEST VEHICULE SPEED
#define REQ_TPMS                               0x0E //REQUEST TPMS STATE
#define REQ_INT_TEMP                           0x0F //REQUEST INTAKE AIR TEMPERATURE(OUTSIDE TEMP)
#define FN_PANIC_REM1_OFF                      0x10 //FUNCTION PANIC OFF FROM REMOTE 1
#define REQ_VIN_1                              0x11 //REQUEST VIN DIGIT 1  TO 2     
#define REQ_VIN_2                              0x12 //REQUEST VIN DIGIT 3  TO 5     
#define REQ_VIN_3                              0x13 //REQUEST VIN DIGIT 6  TO 8     
#define REQ_VIN_4                              0x14 //REQUEST VIN DIGIT 9  TO 11    
#define REQ_VIN_5                              0x15 //REQUEST VIN DIGIT 12 TO 14    
#define REQ_VIN_6                              0x16 //REQUEST VIN DIGIT 15 TO 17    
#define REQ_VIN                                0x17	//REQ THE ENTIRE VIN DATA
#define AL_NEW_DTC_CODE                        0x18 //ALERT THE NEW DTC CODE PRESENTS
#define RESERVED_19                            0x19 //RESERVED
#define FN_1A                                  0x1A //RESERVED
#define FN_1B                                  0x1B //RESERVED
#define FN_1C                                  0x1C //RESERVED
#define FN_1D                                  0x1D //RESERVED
#define FN_1E                                  0x1E //RESERVED
#define FN_1F                                  0x1F //RESERVED
#define FN_PANIC_REM2_OFF                      0x20 //FUNCTION PANIC OFF FROM REMOTE 2      
#define SET_VIN_1                              0x21 //RETURN 2 BYTES DIGIT 1  TO 2   
#define SET_VIN_2                              0x22 //RETURN 3 BYTES DIGIT 3  TO 5   
#define SET_VIN_3                              0x23 //RETURN 3 BYTES DIGIT 6  TO 8   
#define SET_VIN_4                              0x24 //RETURN 3 BYTES DIGIT 9  TO 11  
#define SET_VIN_5                              0x25 //RETURN 3 BYTES DIGIT 12 TO 14  
#define SET_VIN_6                              0x26 //RETURN 3 BYTES DIGIT 15 TO 17  
#define FN_EXTEND_RUNTIME                      0x27 //FUNCTION EXTEND RUNTIME
#define FN_28                                  0x28 //RESERVED
#define FN_29                                  0x29 //RESERVED
#define FN_2A                                  0x2A //RESERVED
#define FN_2B                                  0x2B //RESERVED
#define FN_2C                                  0x2C //RESERVED
#define FN_2D                                  0x2D //RESERVED
#define FN_2E                                  0x2E //RESERVED
#define FN_2F                                  0x2F //RESERVED
#define FN_PANIC_REM3_OFF                      0x30 //FUNCTION PANIC OFF FROM REMOTE 3
#define RESERVED_31                            0x31 //RESERVED
#define RESERVED_32                            0x32 //RESERVED
#define RESERVED_33                            0x33 //RESERVED
#define SET_DTC                                0x34 //RETURN 1 BYTE  (NUMBER OF DTC CODE STORE)
#define SET_DTC_CODE                           0x35 //RETURN 3 BYTES (BYTE1 =  # OF CODE STORE) (BYTE 2 = BIT7:6 INDICATES LETTER P,C,U..., BIT5:4 FIRST DIGITAL OF DTC ) 
//                                                   BIT3:0 OF BYTE 2 AND BYTE 3 INDICATE THREE HEX CHARACTERS. EX: 0x12 0x01 0x26 (12"P"0126) = CODE 1 OF 2 IS P0126
#define SET_DTC_CLR                            0x36 //RETURN 1 BYTE  (1 = SUCCESS : 0 = FALSE)
#define SET_FUEL_LEVEL                         0x37 //RETURN 1 BYTE  (0 = 0% : 100 = 100%)        
#define FN_38                                  0x38 //RESERVED
#define FN_39                                  0x39 //RESERVED
#define SET_ODO                                0x3A //RETURN 3 BYTES (0 TO 999999)
#define SET_BAT_VOLT                           0x3B //RETURN 1 BYTE  (0 TO 15.94 VOLTS) VALUE/16 EX: 196/16 = 12.25V
#define FN_3C                                  0x3C //RESERVED
#define FN_3D                                  0x3D //RESERVED
#define SET_TPMS                               0x3E //RETURN 1 BYTE  (BIT 7-> 1 = ON : 0 = OFF)
#define FN_3F                                  0x3F //RESERVED
#define FN_DOME_OFF                            0x40 //FUNCTION DOME OFF FROM REMOTE STARTER
#define FN_ACC_OFF                             0x41 //FUNCTION ACC OFF FROM REMOTE STARTER
#define FN_IGN_OFF                             0x42 //FUNCTION IGNITION OFF FROM REMOTE STARTER
#define FN_CAR_IGN_OFF                         0x43 //FUNCTION CAR IGNITION OFF DETECTED BY IGNITION SENSE INPUT OF REMOTE STARTER
#define FN_START_OFF                           0x44 //FUNCTION STARTER OFF FROM REMOTE STARTER
#define FN_45                                  0x45 //RESERVED
#define FN_46                                  0x46 //RESERVED
#define FN_47                                  0x47 //RESERVED
#define FN_PARKING_OFF                         0x48 //FUNCTION PARKING OFF FROM REMOTE STARTER
#define REQ_PROTOCOL_VER                       0x49 //REQUEST D2D PROTOCOL VERSION
#define SET_PROTOCOL_VER                       0x4A //RETURN D2D PROTOCOL VERSION
#define AL_QUERY                               0x4B //QUERY ALERT STATUS
#define FN_4C                                  0x4C //RESERVED
#define FN_4D                                  0x4D //RESERVED
#define FN_4E                                  0x4E //RESERVED
#define SET_HANDBRAKE_OFF                      0x4F //HANDBRAKE IS DISENGAGED
#define REQ_MODULE_INFO                        0x50 //REQUEST MODULE INFORMATION
#define SET_PLATFORM_ID                        0x51 //RETURN PLATFORM ID
#define SET_HW_VERSION                         0x52 //RETURN HARDWARE VERSION
#define SET_FW_ID                              0x53 //RETURN FIRMWARE ID
#define RESERVED_54                            0x54 //RESERVED
#define SET_FW_VERSION                         0x55 //RETURN FIRMWARE VERSION
#define REQ_EXTENDED_STATUS_SUPPORT            0x56 //REQUEST EXTENDED STATUS SUPPORT
#define SET_EXTENDED_STATUS_SUPPORT            0x57 //RETURN EXTENDED STATUS SUPPORT
#define RESERVED_58                            0x58 //RESERVED
#define REQ_RUNTIME                            0x59 //REQUEST REMAINING ENGINE RUNTIME
#define SET_RUNTIME                            0x5A //RETURN REMAINING ENGINE RUNTIME
#define AL_ALERT_STATUS                        0x5B //RETURN ALERT STATUS
#define AL_FAILED_TO_UNLOCK                    0x5C //RETURN ALERT OF FAILED TO UNLOCK
#define AL_FAILED_TO_START                     0x5D //RETURN ALERT OF FAILED TO START
#define AL_FAILED_TO_STOP                      0x5E //RETURN ALERT OF FAILED TO STOP
#define AL_FAILED_TO_LOCK                      0x5F //RETURN ALERT OF FAILED TO LOCK
#define FN_60                                  0x60 //RESERVED FOR ENCRYPTION       
#define FN_61                                  0x61 //RESERVED FOR ENCRYPTION
#define FN_62                                  0x62 //RESERVED FOR ENCRYPTION       
#define FN_63                                  0x63 //RESERVED FOR ENCRYPTION       
#define FN_64                                  0x64 //RESERVED FOR ENCRYPTION       
#define FN_65                                  0x65 //RESERVED FOR ENCRYPTION       
#define FN_66                                  0x66 //RESERVED FOR ENCRYPTION       
#define FN_67                                  0x67 //RESERVED FOR ENCRYPTION       
#define FN_68                                  0x68 //RESERVED FOR ENCRYPTION       
#define FN_69                                  0x69 //RESERVED FOR ENCRYPTION       
#define FN_6A                                  0x6A //RESERVED FOR ENCRYPTION       
#define FN_6B                                  0x6B //RESERVED FOR ENCRYPTION       
#define FN_6C                                  0x6C //RESERVED FOR ENCRYPTION       
#define FN_6D                                  0x6D //RESERVED FOR ENCRYPTION       
#define FN_6E                                  0x6E //RESERVED FOR ENCRYPTION       
#define FN_6F                                  0x6F //RESERVED FOR ENCRYPTION       
#define AL_ALERT_CLEAR                         0x70 //CLEAR ALL PREVIOUS ALERT        
#define SET_HOOD_CLOSE                         0x71 //SEND HOOD CLOSE TO REMOTE STARTER
#define SET_TRUNK_CLOSE                        0x72 //SEND TRUNK CLOSE TO REMOTE STARTER
#define AL_EIPS_SHUTDOWN_FAIL                  0x73 //SEND NOTIFICATION TO USER THATS THE CAR STILL RUNNING AFTER SHUTDOWN
#define SET_EIPS_SHUTDOWN_FAIL                 AL_EIPS_SHUTDOWN_FAIL //duplicated
#define SET_DRIVER_CLOSE                       0x74 //SEND DRIVER CLOSE TO REMOTE STARTER
#define SET_EXTENDED_STATUS                    0x75 //RETURN UPDATED STATUS OF VEHICLE
//                              BYTE 1:
//                                      BIT0 = LOCK                     (1: LOCK)
//                                      BIT1 = ARMED                    (1: ARMED)
//                                      BIT2 = IGNITION         		(1: ON)
//                                      BIT3 = PARKING  LIGHT			(1: ON)
//                                      BIT4 = SIREN 					(1: ON)
//                                      BIT5 = VALET MODE       		(1: ACTIVATED)
//                                      BIT6 = START SUCCESSFULLY		(1: ENGINE IS RUNNING)
//                                      BIT7 = PAGING,CAR FIND OR PANIC (1: ON)
//                              BYTE 2:
//                                      BIT0 = DOOR TRIGGER             (1: OPEN)
//                                      BIT1 = TRUNK TRIGGER            (1: OPEN)
//                                      BIT2 = HOOD TRIGGER             (1: OPEN)
//                                      BIT3 = WARN AWAY        		(1: TRIGGERED)
//                                      BIT4 = SHOCK TRIGGER    		(1: TRIGGERED)
//                                      BIT5 = READY MODE				(1: NOT READY)
//                                      BIT6 = RESERVED
//                                      BIT7 = RESERVED
//                              BYTE 3:
//                                      BIT0 = DTC CODE PRESET				(1: YES)
//                                      BIT1 = ALERT CONDITION				(1: YES)
//                                      BIT2 = ENGINE RUNNING STATE CHANGE	(1: BIT6 OF BYTE1 VALUE CHANGED)
//                                      BIT3 = RESERVED
//                                      BIT4 = RESERVED
//                                      BIT5 = RESERVED
//                                      BIT6 = RESERVED
//                                      BIT7 = RESRVERD
//
#define FN_76                                  0x76 //RESERVED
#define SET_PRK_NTRL_NOT_ACTIVE                0x77 //SEND TRANSMISSION PARK/NEUTRAL SW ACTIVE = NO
#define SET_OTHER_CLOSE                        0x78 //SEND OTHER DOOR CLOSE TO REMOTE STARTER
#define SET_BRAKE_OFF                          0x79 //SEND BRAKE PEDAL RELASE TO REMOTE STARTER
#define FN_7A                                  0x7A //RESERVED
#define SET_GLOWPLUG_OFF                       0x7B //SEND GLOW PLUG OFF TO REMOTE STARTER
#define SET_KEY_OUT                            0x7C //SEND KEY OUT TO REMOTE STARTER
#define SET_IGN_OFF                            0x7D //SEND IGNITION OFF TO REMOTE STARTER
#define REQ_2WAY_STATUS                        0x7E //REQUEST UPDATED 2-WAY STATUS
#define REQ_EXTENDED_STATUS                    0x7F //REQUEST UPDATED STATUS OF VEHICLE
#define FN_80                                  0x80 //RESERVED
#define FN_ARM                                 0x81 //FUNCTION ARM ONLY
#define FN_DISARM                              0x82 //FUNCTION DISARM ONLY
#define FN_83                                  0x83 //RESERVED
#define FN_84                                  0x84 //RESERVED
#define FN_85                                  0x85 //RESERVED
#define FN_86                                  0x86 //RESERVED
#define FN_87                                  0x87 //RESERVED
#define FN_GWR_ON                              0x88 //FUNCTION GROUND WHEN RUNNING ON
#define FN_89                                  0x89 //RESERVED
#define FN_8A                                  0x8A //RESERVED
#define FN_8B                                  0x8B //RESERVED
#define FN_8C                                  0x8C //RESERVED
#define FN_8D                                  0x8D //RESERVED
#define FN_8E                                  0x8E //RESERVED
#define FN_8F                                  0x8F //RESERVED
#define FN_PANIC_REM1_ON                       0x90 //FUNCTION PANIC ON FROM REMOTE 1
#define FN_LOCK_REM1_ON                        0x91 //FUNCTION LOCK ON FROM REMOTE 1
#define FN_UNLOCK_DR_REM1_ON                   0x92 //FUNCTION UNLOCK DRIVER ON FROM REMOTE 1
#define FN_AUX1_REM1_ON                        0x93 //FUNCTION AUX1 ON FROM REMOTE 1
#define FN_TRUNK_REM1_ON                       0x94 //FUNCTION TRUNK ON FROM REMOTE 1
#define FN_AUX2_REM1_ON                        0x95 //FUNCTION AUX2 ON FROM REMOTE 1
#define FN_AUX3_REM1_ON                        0x96 //FUNCTION AUX3 ON FROM REMOTE 1
#define FN_AUX4_REM1_ON                        0x97 //FUNCTION AUX4 ON FROM REMOTE 1
#define FN_CHIRP                               0x98 //FUNCTION CHIRP FROM REMOTE 1
#define RESERVED_99                            0x99 //RESERVED
#define FN_UNLOCK_ALL_REM1_ON                  0x9A //FUNCTION UNLOCK ALL ON FROM REMOTE 1  
#define FN_START_ONLY                          0x9B //FUNCTION START ENGINE ONLY
#define FN_9C                                  0x9C //RESERVED
#define FN_9D                                  0x9D //RESERVED
#define FN_9E                                  0x9E //RESERVED
#define FN_9F                                  0x9F //RESERVED
#define FN_PANIC_REM2_ON                       0xA0 //FUNCTION PANIC ON FROM REMOTE 2
#define FN_LOCK_REM2_ON                        0xA1 //FUNCTION LOCK ON FROM REMOTE 2
#define FN_UNLOCK_DR_REM2_ON                   0xA2 //FUNCTION UNLOCK DRIVER ON FROM REMOTE 2
#define FN_AUX1_REM2_ON                        0xA3 //FUNCTION AUX1 ON FROM REMOTE 2
#define FN_TRUNK_REM2_ON                       0xA4 //FUNCTION TRUNK ON FROM REMOTE 2
#define FN_AUX2_REM2_ON                        0xA5 //FUNCTION AUX2 ON FROM REMOTE 2
#define FN_AUX3_REM2_ON                        0xA6 //FUNCTION AUX3 ON FROM REMOTE 2
#define FN_AUX4_REM2_ON                        0xA7 //FUNCTION AUX4 ON FROM REMOTE 2
#define RESERVED_A8                            0xA8 //RESERVED
#define RESERVED_A9                            0xA9 //RESERVED
#define FN_UNLOCK_ALL_REM2_ON                  0xAA //FUNCTION UNLOCK ALL ON FROM REMOTE 2  
#define FN_AB                                  0xAB //RESERVED
#define FN_AC                                  0xAC //RESERVED
#define FN_AD                                  0xAD //RESERVED
#define FN_AE                                  0xAE //RESERVED
#define FN_AF                                  0xAF //RESERVED
#define FN_PANIC_REM3_ON                       0xB0 //FUNCTION PANIC ON FROM REMOTE 3
#define FN_LOCK_REM3_ON                        0xB1 //FUNCTION LOCK ON FROM REMOTE 3
#define FN_UNLOCK_DR_REM3_ON                   0xB2 //FUNCTION UNLOCK DRIVER ON FROM REMOTE 3
#define FN_AUX1_REM3_ON                        0xB3 //FUNCTION AUX1 ON FROM REMOTE 3
#define FN_TRUNK_REM3_ON                       0xB4 //FUNCTION TRUNK ON FROM REMOTE 3
#define FN_AUX2_REM3_ON                        0xB5 //FUNCTION AUX2 ON FROM REMOTE 3
#define FN_AUX3_REM3_ON                        0xB6 //FUNCTION AUX2 ON FROM REMOTE 3
#define FN_AUX4_REM3_ON                        0xB7 //FUNCTION AUX3 ON FROM REMOTE 3
#define FN_CAR_FINDER                          0xB8 //FUNCTION CAR FINDER
#define FN_ALARM_TRIGGERED                     0xB9 //FUNCTION ALARM TRIGGERED FROM REMOTE STARTER  (mostly for Smart Start signaling)
#define AL_ALARM_ALERT                         FN_ALARM_TRIGGERED //duplicated
#define FN_UNLOCK_ALL_REM3_ON                  0xBA //FUNCTION UNLOCK ALL ON FROM REMOTE 3  
#define RESERVED_BB                            0xBB //RESERVED
#define RESERVED_BC                            0xBC //RESERVED
#define FN_BC                                  0xBC //RESERVED
#define FN_BD                                  0xBD //RESERVED
#define FN_BE                                  0xBE //RESERVED
#define FN_BF                                  0xBF //RESERVED
#define FN_DOME_ON                             0xC0 //FUNCTION DOME ON FROM REMOTE STARTER
#define FN_ACC_ON                              0xC1 //FUNCTION ACC ON FROM REMOTE STARTER
#define FN_IGN_ON                              0xC2 //FUNCTION IGNITION ON FROM REMOTE STARTER
#define FN_CAR_IGN_ON                          0xC3 //FUNCTION CAR IGNITION ON DETECTED BY IGNITION SENSE INPUT OF REMOTE STARTER
#define FN_START_ON                            0xC4 //FUNCTION STARTER ON FROM REMOTE STARTER
#define FN_C5                                  0xC5 //RESERVED
#define FN_C6                                  0xC6 //RESERVED
#define FN_VALET_TOGGLE                        0xC7 //FUNCTION VALET HAS BEEN TOGGLED FROM REMOTE OR SMART START OR ELSE
#define FN_PARKING_ON                          0xC8 //FUNCTION PARKING ON FROM REMOTE STARTER
#define FN_C9                                  0xC9 //RESERVED
#define FN_CA                                  0xCA //RESERVED
#define FN_CB                                  0xCB //RESERVED
#define FN_CC                                  0xCC //RESERVED
#define FN_CD                                  0xCD //RESERVED
#define FN_CE                                  0xCE //RESERVED
#define SET_HANDBRAKE_ON                       0xCF //HANDBRAKE IS ENGAGED
#define RESERVED_D0                            0xD0 //RESERVED
#define RESERVED_D1                            0xD1 //RESERVED
#define AL_EIPS_ALERT                          0xD2 //SEND ALERT COMMAND TO NOTIFY THE USER THE CAR RUNNING WITHOUT SMARTKEY (EVERY XMIN)
#define SET_EIPS_ALERT                         AL_EIPS_ALERT  //DUPLICATED
#define AL_EIPS_SHUTDOWN                       0xD3 //SEND SHUTDOWN COMMAND TO NOTIFY THE USER THE CAR SHUTDOWN AFTER XMIN WITHOUT SMARTKEY
#define SET_EIPS_SHUTDOWN                      AL_EIPS_SHUTDOWN  //DUPLICATED
#define SET_KEYLESS_UNLOCK                     0xD4 //SEND UNLOCK FROM FACTORY KEYLESS
#define SET_KEYLESS_LOCK                       0xD5 //SEND LOCK FROM FACTORY KEYLESS        
#define SET_KEYLESS_AUX3                       0xD6 //SEND AUX4 FROM FACTORY KEYLESS(GAZ DOOR ,SECOND TRUNK OR ANY 4TH BUTTON)
#define SET_KEYLESS_PANIC                      0xD7 //SEND PANIC FROM FACTORY KEYLESS
#define SET_KEYLESS_AUX1                       0xD8 //SEND AUX1 FROM FACTORY KEYLESS (LEFT/RIGHT SLIDE)
#define SET_KEYLESS_AUX2                       0xD9 //SEND AUX2 FROM FACTORY KEYLESS (LEFT/RIGHT SLIDE)
#define SET_KEYLESS_START                      0xDA //SEND START FROM FACTORY KEYLESS
#define SET_KEYLESS_STOP                       0xDB //SEND STOP FROM FACTORY KEYLESS 
#define SET_KEYLESS_AUX4                       0xDC //SEND AUX4 FROM FACTORY KEYLESS(GAZ DOOR ,SECOND TRUNK OR ANY 4TH BUTTON)
#define SET_KEYLESS_DISARM                     0xDD //SEND DISARM FROM FACTORY KEYLESS TO DISARM AFTERMARKET ALARM IF THE REMOTE IS LOSS OR BROKEN
#define SET_KEYLESS_ARM                        0xDE //SEND ARM FROM FACTORY KEYLESS TO ARM AFTERMARKET ALARM IF THE REMOTE IS LOSS OR BROKEN        
#define SET_KEYLESS_TRUNK                      0xDF //SEND TRUNK FROM FACTORY KEYLESS
#define FN_E0                                  0xE0 //RESERVED FOR ENCRYPTION       
#define FN_E1                                  0xE1 //RESERVED FOR ENCRYPTION       
#define FN_E2                                  0xE2 //RESERVED FOR ENCRYPTION       
#define FN_E3                                  0xE3 //RESERVED FOR ENCRYPTION       
#define FN_E4                                  0xE4 //RESERVED FOR ENCRYPTION       
#define FN_E5                                  0xE5 //RESERVED FOR ENCRYPTION       
#define FN_E6                                  0xE6 //RESERVED FOR ENCRYPTION       
#define FN_E7                                  0xE7 //RESERVED FOR ENCRYPTION       
#define FN_E8                                  0xE8 //RESERVED FOR ENCRYPTION       
#define FN_E9                                  0xE9 //RESERVED FOR ENCRYPTION       
#define FN_EA                                  0xEA //RESERVED FOR ENCRYPTION       
#define FN_EB                                  0xEB //RESERVED FOR ENCRYPTION       
#define FN_EC                                  0xEC //RESERVED FOR ENCRYPTION       
#define FN_ED                                  0xED //RESERVED FOR ENCRYPTION       
#define FN_EE                                  0xEE //RESERVED FOR ENCRYPTION       
#define FN_EF                                  0xEF //RESERVED FOR ENCRYPTION       
#define SET_INT_TEMP                           0xF0 //SEND INTAKE AIR TEMPERATURE OR OUTSIDE TEMP (1 BYTE) TO REMOTE STARTER 
#define SET_HOOD_OPEN                          0xF1 //SEND HOOD OPEN TO REMOTE STARTER
#define SET_TRUNK_OPEN                         0xF2 //SEND TRUNK OPEN TO REMOTE STARTER
#define SET_RPM                                0xF3 //SEND ENGINE RPM (2BYTES)  TO REMOTE STARTER EX:0 F3H + 003H + 0E8H = 1000RPM 
#define SET_DRIVER_OPEN                        0xF4 //SEND DRIVER DOOR OPEN TO REMOTE STARTER
#define SET_ENG_TEMP                           0xF5 //SEND ENGINE COOLANT  TEMPERATURE  TO REMOTE STARTER EX: 0F5H + MIN: 0 = -40C MAX: 0FFH = 215C
#define RESERVED_F6                            0xF6 //RESERVED 
#define SET_PRK_NTRL_ACTIVE                    0xF7 //SEND TRANSMISSION PARK/NEUTRAL SW ACTIVE = YES
#define SET_OTHER_OPEN                         0xF8 //SEND OTHER DOOR OPEN TO REMOTE STARTER
#define SET_BRAKE_ON                           0xF9 //SEND BRAKE PEDAL ENGAGE(ON) TO REMOTE STARTER
#define SET_SPEED                              0xFA //SEND VEHICULE SPEED, RETURN 1 BYTE (0 TO 255 km/h)
#define SET_GLOWPLUG_ON                        0xFB //SEND GLOW PLUG(ON) TO REMOTE STARTER
#define SET_KEY_IN                             0xFC //SEND KEY IN TO REMOTE STARTER
#define SET_IGN_ON                             0xFD //SEND IGNITION ON TO REMOTE STARTER
#define SET_STATUS                             0xFE //SEND STATUS BIT TO REMOTE  + 1BYTE(QUERY SUPPORT) + 1 BYTE(STATUS)
//                              BYTE1 QUERY SUPPORT     1= SUPPORT 0= NOT SUPPORT
//                                      BIT0 = DRIVER DOOR      
//                                      BIT1 = OTHER DOOR (RF + LR + RR)
//                                      BIT2 = TRUNK            
//                                      BIT3 = HOOD             
//                                      BIT4 = BRAKE            
//                                      BIT5 = KEYSENS
//                                      BIT6 = PARK/NEUTRAL SWITCH ACTIVE
//                                      BIT7 = HANDBRAKE
//                              BYTE STATUS
//                                      BIT0 = DRIVER DOOR                      1= OPEN   0= CLOSE
//                                      BIT1 = OTHER DOOR (RF + LR + RR)        1= OPEN   0= CLOSE
//                                      BIT2 = TRUNK                            1= OPEN   0= CLOSE
//                                      BIT3 = HOOD                             1= OPEN   0= CLOSE
//                                      BIT4 = BRAKE                            1= ENGAGE 0= RELEASE
//                                      BIT5 = KEYSENS                          1= KEY IN 0= KEY OUT
//                                      BIT6 = PARK/NEUTRAL SWITCH              1= ACTIVE 0 = NOT ACTIVE                        
//                                      BIT7 = HANDBRAKE            1= ENGAGE 0= RELEASE
//
#define FN_FF                                  0xFF
