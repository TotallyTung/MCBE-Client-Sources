//
// Created by vastrakai on 8/10/2023.
//
#pragma once

#include <Prax/Hook/Hook.h>
#include <Psapi.h>
#include <Prax/ExceptionHandler.h>
#include <Prax/Utils/System/Internet.h>

class ContainerScreenController {
public:
    /*virtual __int64 destructor();
    virtual __int64 unknownIntop0();
    virtual __int64 tick(void);
    virtual __int64 handleEvent(__int64 &);
    virtual __int64 getRoute(void);
    virtual __int64 setScreenState(__int64, std::string const &);
    virtual __int64 onOpen(void);
    virtual __int64 onTerminate(void);
    virtual __int64 onInit(void);
    virtual __int64 onDelete(void);
    virtual __int64 unknown1();
    virtual bool canExit(void);
    virtual __int64 tryExit(void);
    virtual __int64 areControllerTabsEnabled(void);
    virtual __int64 onCreation(void);
    virtual __int64 logCreationTime(std::string const &, double, double, unsigned char);
    virtual __int64 onLeave(void);
    virtual __int64 leaveScreen(void);
    virtual __int64 handleGameEventNotification(__int64);
    virtual __int64 bind(std::string const &, unsigned int, int, std::string const &, unsigned int, std::string const &, __int64 &);
    virtual __int64 bind(std::string const &, unsigned int, std::string const &, __int64 &);
    virtual __int64 handleLicenseChanged(void);
    virtual __int64 onDictationEvent(std::string const &);
    virtual void setAssociatedBlockPos(BlockPos const &);
    virtual void setAssociatedEntityUniqueID(__int64);
    virtual void setSuspendInput(bool);
    virtual __int64 getCallbackInterval(void);
    virtual __int64 onRender(void);
    virtual __int64 unknownIntop1(void);
    virtual __int64 addStaticScreenVars(__int64 &);
    virtual __int64 getAdditionalScreenInfo(void);
    virtual __int64 getTelemetryOverride(void);
    virtual __int64 addEventProperties(__int64) const;
    virtual __int64 getSceneType(void);
    virtual __int64 getScreenVersion(void);
    virtual __int64 unknownIntop2(void);
    virtual __int64 screenHandlesGamepadMenuButton(void);
    virtual __int64 getProxy(void);
    virtual __int64 onEntered(void);
    virtual __int64 getNameId(std::string const &);
    virtual __int64 unknownIntop3(void);
    virtual __int64 _doesScreenHaveExitBehavior(void);
    virtual __int64 _isStillValid(void);
    virtual __int64 _getGamepadHelperVisible(void);
    virtual __int64 _getMixedHelperVisible(void);
    virtual __int64 _getKeyboardHelperVisible(void);
    virtual __int64 unknownIntop4(void);
    virtual __int64 unknownIntop5(void);
    virtual __int64 _getButtonADescription(void);
    virtual __int64 _getButtonBDescription(void);
    virtual __int64 _getButtonXDescription(void);
    virtual __int64 _getButtonYDescription(void);
    virtual __int64 _getButtonKeyboardDescription(void);
    virtual __int64 unknownIntop6(void);
    virtual __int64 unknownIntop7(void);
    virtual __int64 unknownIntop8(void);
    virtual __int64 _handlePlaceAll(std::string const &, int);
    virtual __int64 _handlePlaceOne(std::string const &, int);
    virtual __int64 _handleSelectSlot(std::string const &, int);
    virtual __int64 _getSelectedSlotInfo(void);
    virtual __int64 unknown7();
    virtual __int64 unknown8();
    virtual __int64 unknownIntop9(void);
    virtual __int64 unknownIntop10(void);
    virtual ItemStack *_getItemStack(std::string, int) const;
    virtual __int64 _getVisualItemStack(std::string const &, int);
    virtual __int64 _getTakeableItemStackBase(std::string const &, int);
    virtual __int64 _onContainerSlotHovered(std::string const &, int);
    virtual __int64 _onContainerSlotSelected(std::string const &, int);
    virtual __int64 _onContainerSlotPressed(std::string const &, int);
    virtual __int64 _shouldSwap(std::string const &, int, std::string const &, int);
    virtual __int64 unknownIntop11(void);
    virtual __int64 _getCollectionName(__int64 *);
    virtual __int64 _canSplit(std::string const &, int);
    virtual __int64 _sendFlyingItem(__int64 const &, std::string const &, int, std::string const &, int);*/

    void __fastcall handleAutoPlace(const std::string& name, int slot);
    void* _tryExit();
    //class ItemStack* __fastcall getItemStack(const std::string& name, int slot);

};

class ContainerScreenControllerHook : public Hook {
public:
    ContainerScreenControllerHook() : Hook("ContainerScreenControllerHook") {

        InitOnConstruct = true;
    }

    void Init() override;
    void UnHook() override;
};


