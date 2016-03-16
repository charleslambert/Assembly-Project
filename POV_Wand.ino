#include "POV_Wand.h"

//
struct MES {
  String message;
  unsigned char r;
  unsigned char g;
  unsigned char b;
};

// Input messages
//==============================================================================

//To insure spacing add spaces at the beginning and end of messages
MES mes1 = {
  " HELLO ",
  50,
  0,
  0
};
MES mes2 = {
  " CRUEL ",
  0,
  50,
  0
};
MES mes3 = {
  " WORLD ",
  0,
  0,
  50
};

MES messages[] = {mes1,mes2,mes3};

// Character bitmaps for Minecraft 16pt
//==============================================================================
const static unsigned char letters[] PROGMEM= 
{
  // @0 ' ' (17 pixels wide)
  //                  
  //                  
  //                  
  //                  
  //                  
  //                  
  //                  
  //                  
  //                  
  //                  
  //                  
  //                  
  //                  
  //                  
  //                  
  //                  
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

  // @34 'A' (17 pixels wide)
  //      #######     
  //      #######     
  //      #######     
  //   ####     ####  
  //   ####     ####  
  //   #############  
  //   #############  
  //   #############  
  //   ####     ####  
  //   ####     ####  
  //   ####     ####  
  //   ####     ####  
  //   ####     ####  
  //   ####     ####  
  //   ####     ####  
  //   ####     ####  
  0x00, 0x00, 0xF8, 0xF8, 0xF8, 0xFF, 0xE7, 0xE7, 0xE7, 0xE7, 0xE7, 0xFF, 0xF8, 0xF8, 0xF8, 0x00, 0x00, 
  0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 

  // @68 'B' (17 pixels wide)
  //  ############    
  //  ############    
  //  ############    
  //  ####      ####  
  //  ####      ####  
  //  ####      ####  
  //  ####      ####  
  //  ############    
  //  ############    
  //  ####      ####  
  //  ####      ####  
  //  ####      ####  
  //  ####      ####  
  //  ############    
  //  ############    
  //  ############    
  0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x87, 0x87, 0x87, 0x87, 0x87, 0x87, 0xFF, 0xFF, 0x78, 0x78, 0x00, 0x00, 
  0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xE1, 0xE1, 0xE1, 0xE1, 0xE1, 0xE1, 0xFF, 0xFF, 0x1E, 0x1E, 0x00, 0x00, 

  // @102 'C' (17 pixels wide)
  //     ###########  
  //     ###########  
  //     ###########  
  //  ####        ### 
  //  ####        ### 
  //  ####        ### 
  //  ####            
  //  ####            
  //  ####            
  //  ####            
  //  ####            
  //  ####        ### 
  //  ####        ### 
  //     ###########  
  //     ###########  
  //     ###########  
  0x00, 0xF8, 0xF8, 0xF8, 0xFF, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x3F, 0x3F, 0x38, 0x00, 
  0x00, 0x1F, 0x1F, 0x1F, 0xFF, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xF8, 0xF8, 0x18, 0x00, 

  // @136 'D' (17 pixels wide)
  //  ############    
  //  ############    
  //  ############    
  //  ####      ####  
  //  ####      ####  
  //  ####      ####  
  //  ####      ####  
  //  ####      ####  
  //  ####      ####  
  //  ####      ####  
  //  ####      ####  
  //  ####      ####  
  //  ####      ####  
  //  ############    
  //  ############    
  //  ############    
  0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0xFF, 0xFF, 0xF8, 0xF8, 0x00, 0x00, 
  0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xFF, 0xFF, 0x1F, 0x1F, 0x00, 0x00, 

  // @170 'E' (17 pixels wide)
  //  ##############  
  //  ##############  
  //  ##############  
  //  ####            
  //  ####            
  //  ########        
  //  ########        
  //  ########        
  //  ####            
  //  ####            
  //  ####            
  //  ####            
  //  ####            
  //  ##############  
  //  ##############  
  //  ##############  
  0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xE7, 0xE7, 0xE7, 0xE7, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x00, 0x00, 
  0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0x00, 0x00, 

  // @204 'F' (17 pixels wide)
  //   #############  
  //   #############  
  //   #############  
  //   ####           
  //   ####           
  //   #########      
  //   #########      
  //   #########      
  //   ####           
  //   ####           
  //   ####           
  //   ####           
  //   ####           
  //   ####           
  //   ####           
  //   ####           
  0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xE7, 0xE7, 0xE7, 0xE7, 0xE7, 0x07, 0x07, 0x07, 0x07, 0x00, 0x00, 
  0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

  // @238 'G' (17 pixels wide)
  //     ############ 
  //     ############ 
  //     ############ 
  //  ####            
  //  ####            
  //  ####     ###### 
  //  ####     ###### 
  //  ####     ###### 
  //  ####        ### 
  //  ####        ### 
  //  ####        ### 
  //  ####        ### 
  //  ####        ### 
  //     ##########   
  //     ##########   
  //     ##########   
  0x00, 0xF8, 0xF8, 0xF8, 0xFF, 0x07, 0x07, 0x07, 0x07, 0x07, 0xE7, 0xE7, 0xE7, 0xE7, 0xE7, 0xE7, 0x00, 
  0x00, 0x1F, 0x1F, 0x1F, 0xFF, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xFF, 0x1F, 0x1F, 0x00, 

  // @272 'H' (17 pixels wide)
  //  ####      ####  
  //  ####      ####  
  //  ####      ####  
  //  ####      ####  
  //  ####      ####  
  //  ####      ####  
  //  ####      ####  
  //  ##############  
  //  ##############  
  //  ####      ####  
  //  ####      ####  
  //  ####      ####  
  //  ####      ####  
  //  ####      ####  
  //  ####      ####  
  //  ####      ####  
  0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 
  0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 

  // @306 'I' (17 pixels wide)
  //       #######    
  //       #######    
  //       #######    
  //         ###      
  //         ###      
  //         ###      
  //         ###      
  //         ###      
  //         ###      
  //         ###      
  //         ###      
  //         ###      
  //         ###      
  //       #######    
  //       #######    
  //       #######    
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x07, 0xFF, 0xFF, 0xFF, 0x07, 0x07, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0xE0, 0xFF, 0xFF, 0xFF, 0xE0, 0xE0, 0x00, 0x00, 0x00, 0x00, 

  // @340 'J' (17 pixels wide)
  //           ###    
  //           ###    
  //           ###    
  //           ###    
  //           ###    
  //           ###    
  //           ###    
  //           ###    
  //           ###    
  //           ###    
  //           ###    
  //    ####   ###    
  //    ####   ###    
  //       #####      
  //       #####      
  //       #####      
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x18, 0x18, 0x18, 0xF8, 0xE0, 0xE0, 0xE0, 0xFF, 0x1F, 0x1F, 0x00, 0x00, 0x00, 0x00, 

  // @374 'K' (17 pixels wide)
  //   ####           
  //   ####           
  //   ####     ####  
  //   ####     ####  
  //   ####   ###     
  //   ####   ###     
  //   ####   ###     
  //   ########       
  //   ########       
  //   ####   ###     
  //   ####   ###     
  //   ####   ###     
  //   ####     ####  
  //   ####     ####  
  //   ####     ####  
  //   ####     ####  
  0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x80, 0x80, 0x80, 0xF0, 0x70, 0x7C, 0x0C, 0x0C, 0x0C, 0x00, 0x00, 
  0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x01, 0x01, 0x0F, 0x0E, 0xFE, 0xF0, 0xF0, 0xF0, 0x00, 0x00, 

  // @408 'L' (17 pixels wide)
  //   ####           
  //   ####           
  //   ####           
  //   ####           
  //   ####           
  //   ####           
  //   ####           
  //   ####           
  //   ####           
  //   ####           
  //   ####           
  //   ####           
  //   ####           
  //   #############  
  //   #############  
  //   #############  
  0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0x00, 0x00, 

  // @442 'M' (17 pixels wide)
  //  ####      ####  
  //  ####      ####  
  //  ####      ####  
  //  ######  ######  
  //  ######  ######  
  //  ##############  
  //  #### #### ####  
  //  #### #### ####  
  //  ####      ####  
  //  ####      ####  
  //  ####      ####  
  //  ####      ####  
  //  ####      ####  
  //  ####      ####  
  //  ####      ####  
  //  ####      ####  
  0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x38, 0xF8, 0xE0, 0xE0, 0xF8, 0x38, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 
  0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 

  // @476 'N' (17 pixels wide)
  //  ####      ####  
  //  ####      ####  
  //  ######    ####  
  //  ######    ####  
  //  #### #### ####  
  //  #### #### ####  
  //  #### #### ####  
  //  ####    ######  
  //  ####    ######  
  //  ####      ####  
  //  ####      ####  
  //  ####      ####  
  //  ####      ####  
  //  ####      ####  
  //  ####      ####  
  //  ####      ####  
  0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x0C, 0x7C, 0x70, 0x70, 0xF0, 0x80, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 
  0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 

  // @510 'O' (17 pixels wide)
  //    ###########   
  //    ###########   
  //    ###########   
  // ####         ####
  // ####         ####
  // ####         ####
  // ####         ####
  // ####         ####
  // ####         ####
  // ####         ####
  // ####         ####
  // ####         ####
  // ####         ####
  //    ###########   
  //    ###########   
  //    ###########   
  0xF8, 0xF8, 0xF8, 0xFF, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0xFF, 0xF8, 0xF8, 0xF8, 
  0x1F, 0x1F, 0x1F, 0xFF, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xFF, 0x1F, 0x1F, 0x1F, 

  // @544 'P' (17 pixels wide)
  //  ############    
  //  ############    
  //  ############    
  //  ####      ####  
  //  ####      ####  
  //  ##############  
  //  ############    
  //  ############    
  //  ####            
  //  ####            
  //  ####            
  //  ####            
  //  ####            
  //  ####            
  //  ####            
  //  ####            
  0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xE7, 0xE7, 0xE7, 0xE7, 0xE7, 0xE7, 0xFF, 0xFF, 0x38, 0x38, 0x00, 0x00, 
  0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

  // @578 'Q' (17 pixels wide)
  //    ###########   
  //    ###########   
  //    ###########   
  // ####         ####
  // ####         ####
  // ####         ####
  // ####         ####
  // ####         ####
  // ####         ####
  // ####         ####
  // ####         ####
  // ####         ####
  // ####    #### ####
  //    ###########   
  //    ###########   
  //    ##############
  0xF8, 0xF8, 0xF8, 0xFF, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0xFF, 0xF8, 0xF8, 0xF8, 
  0x1F, 0x1F, 0x1F, 0xFF, 0xE0, 0xE0, 0xE0, 0xE0, 0xF0, 0xF0, 0xF0, 0xF0, 0xE0, 0xFF, 0x9F, 0x9F, 0x9F, 

  // @612 'R' (17 pixels wide)
  // #############    
  // #############    
  // #############    
  // ####        ###  
  // ####        ###  
  // ###############  
  // #############    
  // #############    
  // ####    ####     
  // ####    ####     
  // ####    ####     
  // ####      ####   
  // ####      ####   
  // ####         ####
  // ####         ####
  // ####         ####
  0xFF, 0xFF, 0xFF, 0xFF, 0xE7, 0xE7, 0xE7, 0xE7, 0xE7, 0xE7, 0xE7, 0xE7, 0xFF, 0x38, 0x38, 0x00, 0x00, 
  0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x07, 0x07, 0x1F, 0x1F, 0x18, 0xF8, 0xE0, 0xE0, 0xE0, 

  // @646 'S' (17 pixels wide)
  //      ##########  
  //      ##########  
  //      ##########  
  //   ####           
  //   ####           
  //   ##########     
  //      #######     
  //      #######     
  //            ####  
  //            ####  
  //            ####  
  //            ####  
  //            ####  
  //   ##########     
  //   ##########     
  //   ##########     
  0x00, 0x00, 0x38, 0x38, 0x38, 0xFF, 0xE7, 0xE7, 0xE7, 0xE7, 0xE7, 0xE7, 0x07, 0x07, 0x07, 0x00, 0x00, 
  0x00, 0x00, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xFF, 0x1F, 0x1F, 0x1F, 0x00, 0x00, 

  // @680 'T' (17 pixels wide)
  //  ##############  
  //  ##############  
  //  ##############  
  //       ####       
  //       ####       
  //       ####       
  //       ####       
  //       ####       
  //       ####       
  //       ####       
  //       ####       
  //       ####       
  //       ####       
  //       ####       
  //       ####       
  //       ####       
  0x00, 0x07, 0x07, 0x07, 0x07, 0x07, 0xFF, 0xFF, 0xFF, 0xFF, 0x07, 0x07, 0x07, 0x07, 0x07, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

  // @714 'U' (17 pixels wide)
  //   ####     ####  
  //   ####     ####  
  //   ####     ####  
  //   ####     ####  
  //   ####     ####  
  //   ####     ####  
  //   ####     ####  
  //   ####     ####  
  //   ####     ####  
  //   ####     ####  
  //   ####     ####  
  //   ####     ####  
  //   ####     ####  
  //      ########    
  //      ########    
  //      ########    
  0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 
  0x00, 0x00, 0x1F, 0x1F, 0x1F, 0xFF, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xFF, 0xFF, 0x1F, 0x1F, 0x00, 0x00, 

  // @748 'V' (17 pixels wide)
  //  ####      ####  
  //  ####      ####  
  //  ####      ####  
  //  ####      ####  
  //  ####      ####  
  //  ####      ####  
  //  ####      ####  
  //     ###  ####    
  //     ###  ####    
  //     ###  ####    
  //     ###  ####    
  //       ####       
  //       ####       
  //       ####       
  //       ####       
  //       ####       
  0x00, 0x7F, 0x7F, 0x7F, 0xFF, 0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0xFF, 0xFF, 0x7F, 0x7F, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x07, 0x07, 0xFF, 0xF8, 0xF8, 0xFF, 0x07, 0x07, 0x07, 0x00, 0x00, 0x00, 0x00, 

  // @782 'W' (17 pixels wide)
  //  ####      ####  
  //  ####      ####  
  //  ####      ####  
  //  ####      ####  
  //  ####      ####  
  //  ####      ####  
  //  ####      ####  
  //  ####      ####  
  //  #### #### ####  
  //  #### #### ####  
  //  #### #### ####  
  //  ######  ######  
  //  ######  ######  
  //  ####      ####  
  //  ####      ####  
  //  ####      ####  
  0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 
  0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x18, 0x1F, 0x07, 0x07, 0x1F, 0x18, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 

  // @816 'X' (17 pixels wide)
  //  ####      ####  
  //  ####      ####  
  //  ####      ####  
  //     ###  ####    
  //     ###  ####    
  //     #########    
  //       ####       
  //       ####       
  //     ###  ####    
  //     ###  ####    
  //     ###  ####    
  //  ####      ####  
  //  ####      ####  
  //  ####      ####  
  //  ####      ####  
  //  ####      ####  
  0x00, 0x07, 0x07, 0x07, 0x3F, 0x38, 0xF8, 0xE0, 0xE0, 0xF8, 0x38, 0x3F, 0x3F, 0x07, 0x07, 0x00, 0x00, 
  0x00, 0xF8, 0xF8, 0xF8, 0xFF, 0x07, 0x07, 0x00, 0x00, 0x07, 0x07, 0xFF, 0xFF, 0xF8, 0xF8, 0x00, 0x00, 

  // @850 'Y' (17 pixels wide)
  //  ####      ####  
  //  ####      ####  
  //  ####      ####  
  //     ###  ####    
  //     ###  ####    
  //     #########    
  //       ####       
  //       ####       
  //       ####       
  //       ####       
  //       ####       
  //       ####       
  //       ####       
  //       ####       
  //       ####       
  //       ####       
  0x00, 0x07, 0x07, 0x07, 0x3F, 0x38, 0xF8, 0xE0, 0xE0, 0xF8, 0x38, 0x3F, 0x3F, 0x07, 0x07, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

  // @884 'Z' (17 pixels wide)
  //  ##############  
  //  ##############  
  //  ##############  
  //            ####  
  //            ####  
  //          ######  
  //          ####    
  //          ####    
  //       ####       
  //       ####       
  //       ####       
  //     ###          
  //     ###          
  //  ##############  
  //  ##############  
  //  ##############  
  0x00, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0xE7, 0xE7, 0xFF, 0xFF, 0x3F, 0x3F, 0x00, 0x00, 
  0x00, 0xE0, 0xE0, 0xE0, 0xF8, 0xF8, 0xFF, 0xE7, 0xE7, 0xE7, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0x00, 0x00, 
};


// Character descriptors for Minecraft 16pt
// { [Char width in bits], [Offset into minecraft_16ptCharBitmaps in bytes] }
const int minecraft_16ptDescriptors[][2] = 
{
  {17, 0},      //   
  {17, 34},     // A 
  {17, 68},     // B 
  {17, 102},    // C 
  {17, 136},    // D 
  {17, 170},    // E 
  {17, 204},    // F 
  {17, 238},    // G 
  {17, 272},    // H 
  {17, 306},    // I 
  {17, 340},    // J 
  {17, 374},    // K 
  {17, 408},    // L 
  {17, 442},    // M 
  {17, 476},    // N 
  {17, 510},    // O 
  {17, 544},    // P 
  {17, 578},    // Q 
  {17, 612},    // R 
  {17, 646},    // S 
  {17, 680},    // T 
  {17, 714},    // U 
  {17, 748},    // V 
  {17, 782},    // W 
  {17, 816},    // X 
  {17, 850},    // Y 
  {17, 884},    // Z 

};

//Functions for operating the LED string
void sendBit( bool bitVal ) {     
      
    asm volatile (
      "cpi %[bitVal_asm],1 \n\t"       // Conditional branch if bitVal is equal to 1
      "breq if \n\t"
                                       // Send a bit value of 0
      "sbi %[port], %[bit] \n\t"       // Set the output bit
      ".rept %[onCycles0] \n\t"        // Now timing actually matters. The 0-bit must be long enough to be detected but not too long or it will be a 1-bit
      "nop \n\t"                       // Execute NOPs to delay exactly the specified number of cycles
      ".endr \n\t"
      "cbi %[port], %[bit] \n\t"       // Clear the output bit
      ".rept %[offCycles0] \n\t"       // Execute NOPs to delay exactly the specified number of cycles
      "nop \n\t"
      ".endr \n\t"
      "rjmp else \n\t"
                                       // Send a bit value of 1
      "if: \n\t"
      "sbi %[port], %[bit] \n\t"       // Set the output bit
      ".rept %[onCycles1] \n\t"        // Execute NOPs to delay exactly the specified number of cycles
      "nop \n\t"
      ".endr \n\t"
      "cbi %[port], %[bit] \n\t"       // Clear the output bit
      ".rept %[offCycles1] \n\t"       // Execute NOPs to delay exactly the specified number of cycles
      "nop \n\t"
      ".endr \n\t"
      "else: \n\t"
      ::
      [port]    "I" (_SFR_IO_ADDR(PIXEL_PORT)),
      [bit]   "I" (PIXEL_BIT),
      [onCycles0]  "I" (NS_TO_CYCLES(T0H) - 2),
      [offCycles0] "I" (NS_TO_CYCLES(T0L) - 2),
      [onCycles1]  "I" (NS_TO_CYCLES(T1H) - 2),
      [offCycles1]   "I" (NS_TO_CYCLES(T1L) - 2),
      [bitVal_asm] "a" (bitVal)
      );
}
  
void sendByte( unsigned char byte ) {
    
    for( unsigned char bit = 0 ; bit < 8 ; bit++ ) {
      
      sendBit( bitRead( byte , 7 ) );                // Neopixel wants bit in highest-to-lowest order
                                                     // so send highest bit (bit #7 in an 8-bit byte since they start at 0)
      byte <<= 1;                                    // and then shift left so bit 6 moves into 7, 5 moves into 6, etc
      
    }           
} 
// Set the specified pin up as digital out

void ledsetup() {
  
  bitSet( PIXEL_DDR , PIXEL_BIT );
  
}

void sendPixel( unsigned char r, unsigned char g , unsigned char b )  {  
  
  sendByte(g);          // Neopixel wants colors in green then red then blue order
  sendByte(r);
  sendByte(b);
  
}


// Just wait long enough without sending any bots to cause the pixels to latch and display the last sent frame

void show() {
  _delay_us( (RES / 1000UL) + 1);       // Round up since the delay must be _at_least_ this long (too short might not work, too long not a problem)
}

//Functions for translating text into bitmap and then updating the LED display
//==============================================================================

MES mes_to_dis;

int translate(char c) {
  // Turns the characters in message string into indexes
  // into the charcter array bitmap
  if (c != ' ') {
    return (c - 'A')+1;
  }
  else {
    return 0;
  }
}

void update_display(unsigned char r, unsigned char g, unsigned char b, uint16_t slice) {
   cli();

    for( uint16_t bit = 0 ; bit < 16; bit++ ) {
    // Takes the slice of the charcter and sends it to the LED display
      
      bitRead( slice , 15) ? sendPixel(r,g,b) : sendPixel(0,0,0);                
      slice <<= 1;
      
    }
    sei();
    show();
    _delay_us(FRAME_DELAY);
}

void dis_letter ( unsigned char r, unsigned char g, unsigned char b,char message) {
  
  int index = translate(message);
  int width = 17;
  int i = minecraft_16ptDescriptors[index][1];
  int x;
  uint16_t value;

  for (x=0;x<width;x++){
    //Parses breaks up charaters into slices to be displayed
    value = (pgm_read_byte_near(letters+i+x+width) << 8) | pgm_read_byte_near(letters+i+x);
    update_display(r,g,b,value);
  } 
}

void parse(MES message) {
  int mes_len = message.message.length();
  
  for (int k=0; k<mes_len; k++){
  //Breaks up Messages into individual characters
     dis_letter(message.r,message.g,message.b,message.message[k]);
  }
}
//Functions for button control
//==============================================================================

// Button variables
int buttonVal = 0; // value read from button
int buttonLast = 0; // buffered value of the button's previous state
long btnDnTime; // time the button was pressed down
long btnUpTime; // time the button was released
boolean ignoreUp = false; // whether to ignore the button release because the click+hold was triggered

//=================================================
// Events to trigger by click and press+hold

void shutoff() {
  asm volatile(
    "ldi r16,(1<<%[ASM_INT0]) \n\t"  // Turn on Button interrupt
    "out %[ASM_GIMSK], r16 \n\t"     
    "ldi r16,(1<<%[ASM_SE])|(1<<%[ASM_SM1])|(0<<%[ASM_SM0])|(0<<%[ASM_ISC01])|(0<<%[ASM_ISC00]) \n\t"  //Set sleep mode to powerdown
    "out %[ASM_MCUCR], r16 \n\t"
    "ldi r16, (0<<%[ASM_ADEN]) \n\t"  // Turn off ADC
    "out %[ASM_ADCSRA], r16 \n\t" 
    "sei\n\t"
    "sleep\n\t"  // Sleep
    "cli\n\t"
    "ldi r16,(0<<%[ASM_INT0]) \n\t"   // Turn off Button interrupt
    "out %[ASM_GIMSK], r16 \n\t"
    "ldi r16,(0<<%[ASM_SE]) \n\t"     // Turn off sleep enable
    "out %[ASM_MCUCR], r16 \n\t"
    "ldi r16, (1<<%[ASM_ADEN]) \n\t"  // Turn on ADC
    "out %[ASM_ADCSRA], r16 \n\t"
    "sei\n\t"
    ::
    [ASM_SE] "I" (i_SE),
    [ASM_MCUCR] "I" (_SFR_IO_ADDR(i_MCUCR)),
    [ASM_GIMSK] "I" (_SFR_IO_ADDR(i_GIMSK)),
    [ASM_ADCSRA] "I" (_SFR_IO_ADDR(i_ADCSRA)),
    [ASM_ADEN] "I" (i_ADEN),
    [ASM_INT0] "I" (i_INT0),
    [ASM_ISC00] "I" (i_ISC00),
    [ASM_ISC01] "I" (i_ISC01),
    [ASM_SM0] "I" (i_SM0),
    [ASM_SM1] "I" (i_SM1)
    );
}
ISR(INT0_vect) {
}

void check_long_press(int buttonVal) {
    // Test for button held down for longer than the hold time
  if (buttonVal == LOW && (millis() - btnDnTime) > long(holdTime))
  {
    event2();
    ignoreUp = true;
    btnDnTime = millis();
  }
}

void check_short_press(int buttonVal, int buttonLast) {
  // Test for button pressed and store the down time
  if (buttonVal == LOW && buttonLast == HIGH && (millis() - btnUpTime) > long(debounce))
  {
    btnDnTime = millis();
  }
  
  // Test for button release and store the up time
  if (buttonVal == HIGH && buttonLast == LOW && (millis() - btnDnTime) > long(debounce))
  {
    if (ignoreUp == false) event1();
    else ignoreUp = false;
    btnUpTime = millis();
  }
}

void button_press(){
  buttonVal = digitalRead(buttonPin);

  cli();
  check_short_press(buttonVal,buttonLast);
  check_long_press(buttonVal);
  sei();
  
  buttonLast = buttonVal;
}

int count = 0;

void event1()
{
  count = ((count+1)%3);
  mes_to_dis = messages[count];
}

void event2()
{
  //Shut off display and go to sleep
  update_display(0,0,0,0x00);
  delay(1000);
  shutoff();
}

//Main setup
//==============================================================================

void setup() {
  mes_to_dis = messages[0];

  ledsetup();
  pinMode(buttonPin, INPUT);
  
}

void loop() {

  button_press();

  parse(mes_to_dis);
  
}

