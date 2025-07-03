//
// Created by vastrakai on 7/19/2024.
//

#include "UIProfanityContext.hpp"

#include <SDK/OffsetProvider.hpp>

bool UIProfanityContext::getEnabled() const
{
    return hat::member_at<bool>(this, OffsetProvider::UIProfanityContext_mEnabled);
}

void UIProfanityContext::setEnabled(bool enabled)
{
    hat::member_at<bool>(this, OffsetProvider::UIProfanityContext_mEnabled) = enabled;
}
