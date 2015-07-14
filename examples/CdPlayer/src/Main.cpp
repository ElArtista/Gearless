#include <iostream>
#include <string>
#include <Gearless/Gearless.hpp>

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
/*
        struct transition_table : mpl::vector<
            //    Start     Event         Next      Action				 Guard
            //  +---------+-------------+---------+---------------------+----------------------+
          a_row < Stopped , play        , Playing , &p::start_playback                         >,
          a_row < Stopped , open_close  , Open    , &p::open_drawer                            >,
           _row < Stopped , stop        , Stopped                                              >,
            //  +---------+-------------+---------+---------------------+----------------------+
          a_row < Open    , open_close  , Empty   , &p::close_drawer                           >,
            //  +---------+-------------+---------+---------------------+----------------------+
          a_row < Empty   , open_close  , Open    , &p::open_drawer                            >,
            row < Empty   , cd_detected , Stopped , &p::store_cd_info   ,&p::good_disk_format  >,
            row < Empty   , cd_detected , Playing , &p::store_cd_info   ,&p::auto_start        >,
            //  +---------+-------------+---------+---------------------+----------------------+
          a_row < Playing , stop        , Stopped , &p::stop_playback                          >,
          a_row < Playing , pause       , Paused  , &p::pause_playback                         >,
          a_row < Playing , open_close  , Open    , &p::stop_and_open                          >,
            //  +---------+-------------+---------+---------------------+----------------------+
          a_row < Paused  , end_pause   , Playing , &p::resume_playback                        >,
          a_row < Paused  , stop        , Stopped , &p::stop_playback                          >,
          a_row < Paused  , open_close  , Open    , &p::stop_and_open                          >
            //  +---------+-------------+---------+---------------------+----------------------+
        > {};
*/


///==============================================================
///= Sample Usage
///==============================================================

void Test()
{
    /*
    Gearless::StateMachine<InitState, TransitionTbl> sm;
    sm.Start();
    sm.ProcessEvent(OpenClose());
    sm.ProcessEvent(CdDetected("Bee Gees Spirits Having Flown", Cd));
    sm.ProcessEvent(Play());
    sm.ProcessEvent(Pause());
    sm.ProcessEvent(Stop());
    sm.ProcessEvent(Stop());
    sm.Stop();
    */
}

int main()
{
    Test();
    return 0;
}

