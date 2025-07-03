#pragma once

class ContainerManagerModel {
public: // if any of these ever changes im gonna castrate myself
    virtual ~ContainerManagerModel();
    virtual ContainerID getContainerId(void);
    virtual void setContainerId(ContainerID);
    virtual ContainerType getContainerType(void);
    virtual void setContainerType(ContainerType);
    virtual void serverInitItemStackIds(void);
    virtual void getItemCopies(void); // std::vector<ItemStack> getItemCopies(void);
    virtual void setSlot(int, ItemStack const&, bool);
    virtual void Function8(); //
    virtual ItemStack* getStack(int); // Fuck u mojang
    virtual void setData(int, int);
    virtual void broadcastChanges(void);
    virtual void debitPlayerLevels(int);
    virtual bool isCreativeMode(void);
    virtual bool isClientSide(void);
    virtual bool isServerAuthoritative(void);
    virtual bool isValid(float);
    virtual bool tick(void);
    virtual void _postInit(void);
};