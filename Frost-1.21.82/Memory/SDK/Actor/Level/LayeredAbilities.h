#pragma once

class LayeredAbilities {
public:
    PermissionsHandler       mPermissions;
    std::array<Abilities, 5> mLayers;
};