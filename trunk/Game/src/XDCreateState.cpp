/*
*  (c) 2013 XediXermawan < edi.ermawan@gmail.com >
*/
#include "pch.h"
#include "XDCreateState.h"
#include "XDStateLoading.h"
#include "XDStateMenu.h"

GameState* CreateState::NewState(GameState::STATE state, XDGameRenderer* renderer) {
    switch (state) {
    case GameState::STATE::LOADING:
        return ( new StateLoading(renderer) );
        break;
    case GameState::STATE::MENU:
        return ( new StateMenu(renderer) );
        break;
    default:
        return nullptr;
        break;
    }
}
