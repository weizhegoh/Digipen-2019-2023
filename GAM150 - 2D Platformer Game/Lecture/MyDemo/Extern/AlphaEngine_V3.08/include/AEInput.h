/******************************************************************************/
/*!
\par        Project: Alpha Engine
\file       AEInput.h

\author     Sun Tjen Fam
\date       January 31, 2008

\brief      Header file for the input library.

\copyright  Copyright (C) 2013 DigiPen Institute of Technology. Reproduction 
            or disclosure of this file or its contents without the prior 
            written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#ifndef AE_INPUT_H
#define AE_INPUT_H

// ---------------------------------------------------------------------------
// Defines/Enums

// These defines don't match the direct input defines and you should be using these
// defines instead of the ones in "dinput.h"
// For more key codes go to http://msdn.microsoft.com/en-us/library/windows/desktop/dd375731(v=vs.85).aspx

#define AEVK_LBUTTON		 VK_LBUTTON	    //!< Left mouse button
#define AEVK_RBUTTON		 VK_RBUTTON	    //!< Right mouse button
#define AEVK_MBUTTON		 VK_MBUTTON	    //!< Middle mouse button

#define AEVK_BACK           VK_BACK         //!< Backspace
#define AEVK_TAB            VK_TAB      
#define AEVK_RETURN		    VK_RETURN		
#define AEVK_LSHIFT		    VK_LSHIFT		
#define AEVK_RSHIFT		    VK_RSHIFT		
#define AEVK_LCTRL		    VK_LCONTROL		
#define AEVK_RCTRL		    VK_RCONTROL		
#define AEVK_LALT		    VK_LMENU		
#define AEVK_RALT		    VK_RMENU		
#define AEVK_PRINTSCREEN    VK_SNAPSHOT
#define AEVK_SCROLLLOCK	    VK_SCROLL
#define AEVK_PAUSE		    VK_PAUSE		
#define AEVK_CAPSLOCK	    VK_CAPITAL

#define AEVK_ESCAPE         VK_ESCAPE   

#define AEVK_SPACE		    VK_SPACE		
#define AEVK_PAGEUP		    VK_PRIOR		
#define AEVK_PAGEDOWN	    VK_NEXT
#define AEVK_END		    VK_END		
#define AEVK_HOME		    VK_HOME		
#define AEVK_LEFT		    VK_LEFT		
#define AEVK_UP			    VK_UP			
#define AEVK_RIGHT		    VK_RIGHT		
#define AEVK_DOWN		    VK_DOWN		
#define AEVK_INSERT		    VK_INSERT		
#define AEVK_DELETE		    VK_DELETE		
	 
#define AEVK_0              0x30        
#define AEVK_1              0x31        
#define AEVK_2              0x32        
#define AEVK_3              0x33        
#define AEVK_4              0x34        
#define AEVK_5              0x35        
#define AEVK_6              0x36        
#define AEVK_7              0x37        
#define AEVK_8              0x38        
#define AEVK_9              0x39        
	 
#define AEVK_A              0x41        
#define AEVK_B              0x42        
#define AEVK_C              0x43        
#define AEVK_D              0x44        
#define AEVK_E              0x45        
#define AEVK_F              0x46        
#define AEVK_G              0x47        
#define AEVK_H              0x48        
#define AEVK_I              0x49        
#define AEVK_J              0x4A        
#define AEVK_K              0x4B        
#define AEVK_L              0x4C        
#define AEVK_M              0x4D        
#define AEVK_N              0x4E        
#define AEVK_O              0x4F        
#define AEVK_P              0x50        
#define AEVK_Q              0x51        
#define AEVK_R              0x52        
#define AEVK_S              0x53        
#define AEVK_T              0x54        
#define AEVK_U              0x55        
#define AEVK_V              0x56        
#define AEVK_W              0x57        
#define AEVK_X              0x58        
#define AEVK_Y              0x59        
#define AEVK_Z              0x5A        

#define AEVK_NUMPAD0	    VK_NUMPAD0	
#define AEVK_NUMPAD1	    VK_NUMPAD1	
#define AEVK_NUMPAD2	    VK_NUMPAD2	
#define AEVK_NUMPAD3	    VK_NUMPAD3	
#define AEVK_NUMPAD4	    VK_NUMPAD4	
#define AEVK_NUMPAD5	    VK_NUMPAD5	
#define AEVK_NUMPAD6	    VK_NUMPAD6	
#define AEVK_NUMPAD7	    VK_NUMPAD7	
#define AEVK_NUMPAD8	    VK_NUMPAD8	
#define AEVK_NUMPAD9	    VK_NUMPAD9	

#define AEVK_NUM_MULTIPLY   VK_MULTIPLY
#define AEVK_NUM_PLUS	    VK_ADD
#define AEVK_NUM_MINUS	    VK_SUBTRACT
#define AEVK_NUM_PERIOD	    VK_DECIMAL
#define AEVK_NUM_DIVIDE	    VK_DIVIDE
#define AEVK_NUMLOCK	    VK_NUMLOCK	

#define AEVK_F1			    VK_F1			
#define AEVK_F2			    VK_F2			
#define AEVK_F3			    VK_F3			
#define AEVK_F4			    VK_F4			
#define AEVK_F5			    VK_F5			
#define AEVK_F6			    VK_F6			
#define AEVK_F7			    VK_F7			
#define AEVK_F8			    VK_F8			
#define AEVK_F9			    VK_F9			
#define AEVK_F10		    VK_F10		
#define AEVK_F11		    VK_F11		
#define AEVK_F12		    VK_F12		

#define AEVK_SEMICOLON	    VK_OEM_1        //!< ; :
#define AEVK_SLASH		    VK_OEM_2		//!< / ?
#define AEVK_BACKQUOTE		VK_OEM_3        //!< ` ~
#define AEVK_LBRACKET	    VK_OEM_4        //!< [ {
#define AEVK_BACKSLASH	    VK_OEM_5        //!< \ |
#define AEVK_RBRACKET	    VK_OEM_6        //!< ] }
#define AEVK_QUOTE		    VK_OEM_7	    //!< ' "

#define AEVK_EQUAL		    VK_OEM_PLUS     //!< = +		
#define AEVK_MINUS		    VK_OEM_MINUS    //!< - _		
#define AEVK_PERIOD		    VK_OEM_PERIOD   //!< . >		
#define AEVK_COMMA		    VK_OEM_COMMA    //!< , <		

// ---------------------------------------------------------------------------
// Function prototypes

#ifdef __cplusplus

extern "C"
{
#endif

// ---------------------------------------------------------------------------

/******************************************************************************/
/*!
\fn		    s32 AEInputInit()

\brief      Initialize AEInput.

\warning    This function is already called in AESysInit.

\retval     s32
            Return 1 if initialization is successful.
            Else return 0.
*/
/******************************************************************************/
AE_API s32  AEInputInit();

/******************************************************************************/
/*!
\fn         void AEInputReset()

\brief      Reset AEInput.

\warning    This function is already called in AESysReset.

\retval     void
            No return.
*/
/******************************************************************************/
AE_API void AEInputReset();

/******************************************************************************/
/*!
\fn         void AEInputUpdate()

\brief      Update AEInput.

\warning    This function is already called in AESysUpdate.

\retval     void
            No return.
*/
/******************************************************************************/
AE_API void AEInputUpdate();

/******************************************************************************/
/*!
\fn         void AEInputExit()

\brief      Free AEInput.

\warning    This function is already called in AESysExit.

\retval     void
            No return.
*/
/******************************************************************************/
AE_API void AEInputExit();

/******************************************************************************/
/*!
\fn         u8 AEInputCheckCurr(u8 key)

\brief      Check if a keyboard button is being pressed.

\details    Check if keyboard button given by key is currently 
            being pressed.

\param      [in] key
            Value corresponding to the keyboard button that is being
            checked for.

\retval     u8
            Return true if keyboard button is being pressed.
            Else return false.
*/
/******************************************************************************/
AE_API u8 AEInputCheckCurr		(u8 key);

/******************************************************************************/
/*!
\fn         u8 AEInputCheckPrev(u8 key)

\brief      Check if a keyboard button was pressed recently.

\details    Check if keyboard button given by key was pressed in
            the previous update.

\param      [in] key
            Value corresponding to the keyboard button that is being
            checked for.

\retval     u8
            Return true if keyboard button was pressed.
            Else return false.
*/
/******************************************************************************/
AE_API u8 AEInputCheckPrev		(u8 key);

/******************************************************************************/
/*!
\fn         u8 AEInputCheckTriggered(u8 key)

\brief      Check if a keyboard button was triggered.

\details    Check if the keyboard button given by key was recently
            triggerd (pressed and then quickly released).

\param      [in] key
            Value corresponding to the keyboard button that is being
            checked for.

\retval     u8
            Return true if keyboard button was pressed.
            Else return false.
*/
/******************************************************************************/
AE_API u8 AEInputCheckTriggered (u8 key);

/******************************************************************************/
/*!
\fn         u8 AEInputCheckReleased(u8 key)

\brief      Check if a keyboard button was released.

\details    Check if the keyboard button given by key was recently
            released.

\param      [in] key
            Value corresponding to the keyboard button that is being
            checked for.

\retval     u8
            Return true if keyboard button was pressed.
            Else return false.
*/
/******************************************************************************/
AE_API u8 AEInputCheckReleased	(u8 key);

/******************************************************************************/
/*!
\fn         void AEInputGetCursorPosition(s32 *pX, 
                                          s32 *pY)

\brief      Get the current position of the cursor, 
            in screen coordinates.

\param      [out] pX
            Pointer to s32 where the X value will be stored.

\param      [out] pY
            Pointer to s32 where the Y value will be stored.

\retval     void
            No return.
*/
/******************************************************************************/
AE_API void AEInputGetCursorPosition(s32 *pX, s32 *pY);

/******************************************************************************/
/*!
\fn         void AEInputGetCursorPositionDelta(s32 *pDeltaX, 
                                               s32 *pDeltaY)

\brief      Get the change in position of the cursor since the last
            update, in screen coordinates.

\param      [out] pDeltaX
            Pointer to s32 where the X value will be stored.

\param      [out] pDeltaY
            Pointer to s32 where the Y value will be stored.

\retval     void
            No return.
*/
/******************************************************************************/
AE_API void AEInputGetCursorPositionDelta(s32 *pDeltaX, s32 *pDeltaY);

/******************************************************************************/
/*!
\fn         void AEInputShowCursor(s32 Show)

\brief      Set if mouse cursor is showned.

\details    Setting true will allow mouse cursor to be rendered 
            on screen. Setting false will hide the mouse cursor.

\param      [in] Show
            Set if mouse cursor is showned or not.

\retval     void
            No return.
*/
/******************************************************************************/
AE_API void AEInputShowCursor(s32 Show);

// ---------------------------------------------------------------------------

#ifdef __cplusplus 
}
#endif

// ---------------------------------------------------------------------------

#endif // AE_INPUT_H

