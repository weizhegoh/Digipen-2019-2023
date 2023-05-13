/******************************************************************************/
/*!
\par        Project: Alpha Engine
\file       AEGameStateMgr.h

\author     Sun Tjen Fam
\date       October 10, 2007

\brief      Header file for the game state manager

\copyright  Copyright (C) 2013 DigiPen Institute of Technology. Reproduction 
            or disclosure of this file or its contents without the prior 
            written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#ifndef AE_GAME_STATE_MGR_H
#define AE_GAME_STATE_MGR_H

// ---------------------------------------------------------------------------
// defines and enums

#define AE_GS_RESTART	0xFFFFFFFE              //!< Special GameState: Restart
#define AE_GS_QUIT		0xFFFFFFFF              //!< Special GameState: Quit

// ---------------------------------------------------------------------------
// externs

extern AE_API u32	gAEGameStateInit;           //!< Initial GameState
extern AE_API u32	gAEGameStateCurr;           //!< Current GameState
extern AE_API u32	gAEGameStatePrev;           //!< Previous GameState
extern AE_API u32	gAEGameStateNext;           //!< Next GameState

// ---------------------------------------------------------------------------

extern AE_API void (*AEGameStateLoad)(void);    //!< Function pointer for load
extern AE_API void (*AEGameStateInit)(void);    //!< Function pointer for init
extern AE_API void (*AEGameStateUpdate)(void);  //!< Function pointer for update
extern AE_API void (*AEGameStateDraw)(void);    //!< Function pointer for draw
extern AE_API void (*AEGameStateFree)(void);    //!< Function pointer for free
extern AE_API void (*AEGameStateUnload)(void);  //!< Function pointer for unload

// ---------------------------------------------------------------------------
// Function prototypes
// ---------------------------------------------------------------------------

#ifdef __cplusplus

extern "C"
{
#endif
// ---------------------------------------------------------------------------

/******************************************************************************/
/*!
\fn         void AEGameStateMgrAdd(u32 gameStateIdx, 
                                   void (*pLoad)(), 
                                   void (*pInit)(), 
                                   void (*pUpdate)(), 
                                   void (*pDraw)(), 
                                   void (*pFree)(), 
                                   void (*pUnload)())

\brief      Create a new GameState and add it to GameStateManager.

\details    The new GameState is identified as gameStateIdx
            and will contain the 6 function pointers passed in.

\param      [in] gameStateIdx
            ID of the newly created GameState.

\param      [in] pLoad
            Function pointer to the load function of the new GameState.

\param      [in] pInit
            Function pointer to the init function of the new GameState.

\param      [in] pUpdate
            Function pointer to the update function of the new GameState.

\param      [in] pDraw
            Function pointer to the draw function of the new GameState.

\param      [in] pFree
            Function pointer to the free function of the new GameState.

\param      [in] pUnload
            Function pointer to the unload function of the new GameState.

\retval     void
            No return.
*/
/******************************************************************************/
AE_API void AEGameStateMgrAdd(u32 gameStateIdx, 
                              void (*pLoad)(), 
                              void (*pInit)(), 
                              void (*pUpdate)(), 
                              void (*pDraw)(), 
                              void (*pFree)(), 
                              void (*pUnload)());

/******************************************************************************/
/*!
\fn         void AEGameStateMgrInit(u32 gameStateInit)

\brief      Initialize the GameStateManager.

\details    Set the initial GameState and start the
            GameStateManager with that GameState.

\warning    This function should be called AFTER all the 
            GameStates have been added to the GameStateManager.

\param      [in] gameStateInit
            ID of the initial GameState.

\retval     void
            No return.
*/
/******************************************************************************/
AE_API void AEGameStateMgrInit(u32 gameStateInit);

/******************************************************************************/
/*!
\fn         void AEGameStateMgrUpdate();

\brief      Update the GameStateManager.

\details    Call this function when the GameState changes to
            set the function pointers for the new state.

\retval     void
            No return.
*/
/******************************************************************************/
AE_API void AEGameStateMgrUpdate();

// ---------------------------------------------------------------------------

#ifdef __cplusplus 
}
#endif

// ---------------------------------------------------------------------------

#endif // AE_GAME_STATE_MGR_H