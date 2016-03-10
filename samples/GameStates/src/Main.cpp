#include <iostream>
#include <string>
#include <functional>
#include <Gearless/Gearless.hpp>
#include <Gearless/StateMachine.hpp>

///==============================================================
///= State Diagram
///==============================================================

//
//                 * Initial
//                 |
//              ___v______  GoToMainMenu
//             | MainMenu |<------------.
//             |__________|             |
//       LoadGame  |                    |
//              ___v_____               |
//             | Loading |              |
//             |_________|              |
//       StartGame |                    |
//              ___v__   PauseGame   ___|___
//             | Game |------------>| Pause |
//             |______|<------------|_______|
//                       ResumeGame
//

///==============================================================
///= Events
///==============================================================
struct LoadGame {};
struct StartGame {};
struct PauseGame {};
struct ResumeGame {};
struct GoToMainMenu {};

///==============================================================
///= States
///==============================================================
struct MainMenu {};
struct Loading {};
struct Game {};
struct PauseMenu {};

// Settup initial state
using InitState = MainMenu;

///==============================================================
///= Transition Actions
///==============================================================
void OnLoading(const LoadGame&) { std::cout << "Loading in Game resources..." << std::endl; }
void OnMainGameStart(const StartGame&) { std::cout << "Starting the Game..." << std::endl; }
void OnPause(const PauseGame&) { std::cout << "Game is paused." << std::endl; }
void OnResume(const ResumeGame&) { std::cout << "Game is resumed." << std::endl; }
void OnMainMenuReturn(const GoToMainMenu&) { std::cout << "Returning to MainMenu..." << std::endl; }

///==============================================================
///= Transition Table
///==============================================================

// Alias to make the transition table more compact
template <class PrevState, class Event, class NextState, typename Fn>
using tr = Gearless::Transition<PrevState, Event, NextState, Fn>;

using OnLoadingW        = Gearless::TFunct<LoadGame, OnLoading>;
using OnMainGameStartW  = Gearless::TFunct<StartGame, OnMainGameStart>;
using OnPauseW          = Gearless::TFunct<PauseGame, OnPause>;
using OnResumeW         = Gearless::TFunct<ResumeGame, OnResume>;
using OnMainMenuReturnW = Gearless::TFunct<GoToMainMenu, OnMainMenuReturn>;

// The transition table
using TransitionTbl = Gearless::Packer<
    //    Start       Event          Next        Action             Guard
    //  +-----------+--------------+-----------+-------------------+------------+
      tr< MainMenu  , LoadGame     , Loading   , OnLoadingW                     >,
    //  +-----------+--------------+-----------+-------------------+------------+
      tr< Loading   , StartGame    , Game      , OnMainGameStartW               >,
    //  +-----------+--------------+-----------+-------------------+------------+
      tr< Game      , PauseGame    , PauseMenu , OnPauseW                       >,
    //  +-----------+--------------+-----------+-------------------+------------+
      tr< PauseMenu , ResumeGame   , Game      , OnResumeW                      >,
      tr< PauseMenu , GoToMainMenu , MainMenu  , OnMainMenuReturnW              >
    //  +-----------+--------------+-----------+-------------------+------------+
>;

// The initial state
using InitState = MainMenu;

///==============================================================
///= Sample Usage
///==============================================================

void Test()
{
    Gearless::StateMachine<InitState, TransitionTbl> sm;
    sm.Start();
    sm.ProcessEvent(LoadGame());
    sm.ProcessEvent(PauseGame());
    sm.ProcessEvent(ResumeGame());
    sm.ProcessEvent(PauseGame());
    sm.ProcessEvent(GoToMainMenu());
    sm.Stop();
}

int main()
{
    Test();
    return 0;
}

