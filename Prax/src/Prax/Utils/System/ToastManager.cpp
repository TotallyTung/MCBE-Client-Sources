//
// Created by Flash on 9/2/2023.
//

#include "ToastManager.h"
#include <xorstr.hpp>

using namespace winrt::Windows::UI::Notifications;


void ToastManager::Init() {
    winrt::init_apartment();
    ShowToast(Prax::ClientName, Prax::ClientName + std::string(PRAX_VERSION) + xorstr_(" has been initialized"));
    // "Prax version: " + std::string(PRAX_VERSION) + " has been initialized"

}

void ToastManager::ShowToast(std::string title, std::string message) {
    ToastNotification notif = ToastNotification(ToastNotificationManager::GetTemplateContent(ToastTemplateType::ToastImageAndText02));

    // Set the title
    winrt::Windows::Data::Xml::Dom::IXmlNodeList txtElmnt = notif.Content().GetElementsByTagName(L"text");
    txtElmnt.Item(0).InnerText(winrt::to_hstring(title));

    // Set the message
    txtElmnt.Item(1).InnerText(winrt::to_hstring(message));

    // Show
    ToastNotificationManager::CreateToastNotifier().Show(notif);
}