#include <iostream>
#include <string>
#include <Gearless/Gearless.hpp>

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

/*
// Alias to make the transition table more compact
template <class PrevState, class Event, class NextState, void Fn(const Event&)>
using tr = Gearless::Transition<PrevState, Event, NextState, Fn>;

// The transition table
using TransitionTbl = Gearless::TransitionTable<
    //    Start       Event          Next        Action             Guard
    //  +-----------+--------------+-----------+------------------+------------+
      tr< MainMenu  , LoadGame     , Loading   , OnLoading                     >,
    //  +-----------+--------------+-----------+------------------+------------+
      tr< Loading   , StartGame    , Game      , OnMainGameStart               >,
    //  +-----------+--------------+-----------+------------------+------------+
      tr< Game      , PauseGame    , PauseMenu , OnPause                       >,
    //  +-----------+--------------+-----------+------------------+------------+
      tr< PauseMenu , ResumeGame   , Game      , OnResume                      >,
      tr< PauseMenu , GoToMainMenu , MainMenu  , OnMainMenuReturn              >
    //  +-----------+--------------+-----------+------------------+------------+
>;

// The initial state
using InitState = MainMenu;
*/

///==============================================================
///= Sample Usage
///==============================================================

void Test()
{
    /*
    Gearless::StateMachine<InitState, TransitionTbl> sm;
    //sm.Start();
    sm.ProcessEvent(LoadGame());
    sm.ProcessEvent(PauseGame());
    sm.ProcessEvent(ResumeGame());
    sm.ProcessEvent(PauseGame());
    sm.ProcessEvent(GoToMainMenu());
    //sm.Stop();
    */
}

int main()
{
    Test();
    return 0;
}

