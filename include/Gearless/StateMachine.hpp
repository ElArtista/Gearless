/*********************************************************************************************************************/
/*                                                  /===-_---~~~~~~~~~------____                                     */
/*                                                 |===-~___                _,-'                                     */
/*                  -==\\                         `//~\\   ~~~~`---.___.-~~                                          */
/*              ______-==|                         | |  \\           _-~`                                            */
/*        __--~~~  ,-/-==\\                        | |   `\        ,'                                                */
/*     _-~       /'    |  \\                      / /      \      /                                                  */
/*   .'        /       |   \\                   /' /        \   /'                                                   */
/*  /  ____  /         |    \`\.__/-~~ ~ \ _ _/'  /          \/'                                                     */
/* /-'~    ~~~~~---__  |     ~-/~         ( )   /'        _--~`                                                      */
/*                   \_|      /        _)   ;  ),   __--~~                                                           */
/*                     '~~--_/      _-~/-  / \   '-~ \                                                               */
/*                    {\__--_/}    / \\_>- )<__\      \                                                              */
/*                    /'   (_/  _-~  | |__>--<__|      |                                                             */
/*                   |0  0 _/) )-~     | |__>--<__|     |                                                            */
/*                   / /~ ,_/       / /__>---<__/      |                                                             */
/*                  o o _//        /-~_>---<__-~      /                                                              */
/*                  (^(~          /~_>---<__-      _-~                                                               */
/*                 ,/|           /__>--<__/     _-~                                                                  */
/*              ,//('(          |__>--<__|     /                  .----_                                             */
/*             ( ( '))          |__>--<__|    |                 /' _---_~\                                           */
/*          `-)) )) (           |__>--<__|    |               /'  /     ~\`\                                         */
/*         ,/,'//( (             \__>--<__\    \            /'  //        ||                                         */
/*       ,( ( ((, ))              ~-__>--<_~-_  ~--____---~' _/'/        /'                                          */
/*     `~/  )` ) ,/|                 ~-_~>--<_/-__       __-~ _/                                                     */
/*   ._-~//( )/ )) `                    ~~-'_/_/ /~~~~~~~__--~                                                       */
/*    ;'( ')/ ,)(                              ~~~~~~~~~~                                                            */
/*   ' ') '( (/                                                                                                      */
/*     '   '  `                                                                                                      */
/*********************************************************************************************************************/
#ifndef _STATE_MACHINE_HPP_
#define _STATE_MACHINE_HPP_

#include <memory>
#include <vector>
#include <tuple>
#include <Gearless/TypeId.hpp>

namespace Gearless
{
    template <class... Types>
    struct Packer {};

    template <class Prev, class Ev, class Next, void Fn(const Ev&)>
    struct Transition
    {
        using PrevState = Prev;
        using Event = Ev;
        using NextState = Next;
        void (*F)(const Ev&) = Fn;
    };

    class TransitionInfo
    {
        public:
            template<class Transit>
            static TransitionInfo FromTransition()
            {
                TransitionInfo t;
                t.mPrevStateTypeId = GetTypeId<typename Transit::PrevState>();
                t.mEventTypeId = GetTypeId<typename Transit::Event>();
                t.mNextStateTypeId = GetTypeId<typename Transit::NextState>();
                return t;
            }

            TypeId GetPrevStateTypeId() const { return mPrevStateTypeId; };
            TypeId GetEventTypeId() const { return mEventTypeId; };
            TypeId GetNextStateTypeId() const { return mNextStateTypeId; };

        private:
            TypeId mPrevStateTypeId;
            TypeId mEventTypeId;
            TypeId mNextStateTypeId;
    };

    template <class... Transitions>
    class TransitionTable
    {
        public:
            TransitionTable()
            {
                Add();
            }

        private:
            void Add()
            {
                mTransitions.insert(
                        std::end(mTransitions),
                        {TransitionInfo::FromTransition<Transitions>()...});
            }

            std::vector<TransitionInfo> mTransitions;
    };

    template <class... Transitions>
    struct ConvertToTransitionTable
    {
        using type = TransitionTable<Transitions...>;
    };

    template <class... Transitions>
    struct ConvertToTransitionTable<Packer<Transitions...>>
    {
        using type = typename ConvertToTransitionTable<Transitions...>::type;  
    };

    template <class InitState, class TransitionsPack>
    class StateMachine
    {
        public:
            /// Starts the operation of the State Machine
            void Start();

            /// Stops the operation of the State Machine
            void Stop();

            template <class Event>
            void ProcessEvent(const Event& ev);

        private:
            /// Stores the currently active transition id
            TypeId mCurStateTypeId;

            /// The transition table translated into type id's
            using TT = typename ConvertToTransitionTable<TransitionsPack>::type;
            TT mTransitionTbl;
    };

    template <class InitState, class TransitionsPack>
    inline void StateMachine<InitState, TransitionsPack>::Start()
    {
        mCurStateTypeId = GetTypeId<InitState>();
    }

    template <class InitState, class TransitionsPack>
    inline void StateMachine<InitState, TransitionsPack>::Stop()
    {

    }

    template <class InitState, class TransitionsPack>
    template <class Event>
    inline void StateMachine<InitState, TransitionsPack>::ProcessEvent(const Event& ev)
    {
        (void) ev;
    }
}

#endif // ! _STATE_MACHINE_HPP_
