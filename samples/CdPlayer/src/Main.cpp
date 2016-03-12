#include <iostream>
#include <string>
#include <Gearless/Gearless.hpp>
#include <Gearless/StateMachine.hpp>

///==============================================================
///= State Diagram
///==============================================================

//
//                        * Initial
//                        |
//       open_close    ___v___
//      .------------>| Empty |  cd_detected
//      | .-----------|_______|--------------.
//      | |open_close                        |
//      | |                                  |
//    __|_v_                             ____v____
//   | Open |        open_close         | Stopped |<-. stop
//   |______|<--------------------------|_________|--'
//      ^ ^                                  ^ ^ |
//      | |   open_close    ________     stop| | |
//      | '----------------| Paused |--------' | |
//      |                  |________|          | |
//      |                      ^ |         stop| |play
//      |                      | |             | |
//      |                 pause| |end_pause    | |
//      |                      | |             | |
//      |                      | v_________    | |
//      |   open_close         '-| Playing |---' |
//      '------------------------|_________|<----'
//

///==============================================================
///= Events
///==============================================================
struct Play {};
struct EndPause {};
struct Stop {};
struct Pause {};
struct OpenClose {};

enum class DiskType { Cd, Dvd };
struct CdDetected
{
    CdDetected(std::string name, DiskType diskType)
        : mName(name), mDiskType(diskType) {}
    std::string mName;
    DiskType mDiskType;
};

///==============================================================
///= States
///==============================================================
struct Empty {};
struct Open {};
struct Stopped {};
struct Paused {};
struct Playing {};

// Settup initial state
using InitState = Empty;

///==============================================================
///= Transition Actions
///==============================================================
void OpenDrawer(const OpenClose&) { std::cout << "Drawer oppened." << std::endl; }
void CloseDrawer(const OpenClose&) { std::cout << "Drawer closed." << std::endl; }
void StoreCdInfo(const CdDetected& cd) { std::cout << "Cd Detected! " << "Name: " << cd.mName << std::endl; }
void StartPlayback(const Play&) { std::cout << "Playback started!" << std::endl; }
void PausePlayback(const Pause&) { std::cout << "Playback paused." << std::endl; }
void ResumePlayback(const Play&) { std::cout << "Playback resumed!" << std::endl; }
void StopPlayback(const Stop&) { std::cout << "Playback stopped!" << std::endl; }
void StopAndOpen(const OpenClose&) { std::cout << "Playback stopped! Drawer is oppening..." << std::endl; }
void StoppedAgain(const Stop&) { std::cout << "Playback already stopped." << std::endl; }

///==============================================================
///= Transition Table
///==============================================================
// Aliases to make the transition table more compact
template <class PrevState, class Event, class NextState, typename Fn>
using tr = Gearless::Transition<PrevState, Event, NextState, Fn>;

using OpenDrawerW  = Gearless::TFunct<OpenClose, OpenDrawer>;
using CloseDrawerW = Gearless::TFunct<OpenClose, CloseDrawer>;
using StoreCdInfoW = Gearless::TFunct<CdDetected, StoreCdInfo>;
using StartPlaybackW = Gearless::TFunct<Play, StartPlayback>;
using PausePlaybackW = Gearless::TFunct<Pause, PausePlayback>;
using ResumePlaybackW = Gearless::TFunct<Play, ResumePlayback>;
using StopPlaybackW = Gearless::TFunct<Stop, StopPlayback>;
using StopAndOpenW = Gearless::TFunct<OpenClose, StopAndOpen>;
using StoppedAgainW = Gearless::TFunct<Stop, StoppedAgain>;

// The transition table
using TransitionTbl = Gearless::Packer<
    //    Start     Event         Next      Action				 Guard
    //  +---------+-------------+---------+---------------------+---------------+
     tr < Stopped , Play        , Playing , StartPlaybackW                      >,
     tr < Stopped , OpenClose   , Open    , OpenDrawerW                         >,
     tr < Stopped , Stop        , Stopped , StoppedAgainW                       >,
    //  +---------+-------------+---------+---------------------+---------------+
     tr < Open    , OpenClose   , Empty   , CloseDrawerW                        >,
    //  +---------+-------------+---------+---------------------+---------------+
     tr < Empty   , OpenClose   , Open    , OpenDrawerW                         >,
     tr < Empty   , CdDetected  , Stopped , StoreCdInfoW                        >,
     tr < Empty   , CdDetected  , Playing , StoreCdInfoW                        >,
    //  +---------+-------------+---------+---------------------+---------------+
     tr < Playing , Stop        , Stopped , StopPlaybackW                       >,
     tr < Playing , Pause       , Paused  , PausePlaybackW                      >,
     tr < Playing , OpenClose   , Open    , StopAndOpenW                        >,
    //  +---------+-------------+---------+---------------------+---------------+
     tr < Paused  , EndPause    , Playing , ResumePlaybackW                     >,
     tr < Paused  , Stop        , Stopped , StopPlaybackW                       >,
     tr < Paused  , OpenClose   , Open    , StopAndOpenW                        >
    //  +---------+-------------+---------+---------------------+---------------+
>;

// The initial state
using InitState = Empty;

///==============================================================
///= Sample Usage
///==============================================================

void Test()
{
    Gearless::StateMachine<InitState, TransitionTbl> sm;
    sm.Start();
    sm.ProcessEvent(OpenClose());
    sm.ProcessEvent(OpenClose());
    sm.ProcessEvent(CdDetected("Bee Gees Spirits Having Flown", DiskType::Cd));
    sm.ProcessEvent(Play());
    sm.ProcessEvent(Pause());
    sm.ProcessEvent(Stop());
    sm.ProcessEvent(Stop());
    sm.Stop();
}

int main()
{
    Test();
    return 0;
}

