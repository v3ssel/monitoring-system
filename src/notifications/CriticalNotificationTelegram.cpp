#include <sstream>
#include <iomanip>
#include <cctype>

#include "CriticalNotificationTelegram.h"

namespace s21 {
const std::string CriticalNotificationTelegram::token_ = "6480364672:AAGmS45L0A7KBn1vonIfT5w_J0lw3uTpwOs";

void CriticalNotificationTelegram::sendNotification(const std::string &receiver, const std::vector<std::string>& msgs) {
    std::string url = "\"https://api.telegram.org/bot" + token_ + "/sendMessage\"";
    std::string query = "chat_id=" + receiver + "&disable_web_page_preview=1&text=";

    std::string hostname = urlEncode("HOST: " + getHostname() + "\n");
    query.append(hostname);

    for (std::string msg : msgs) {
        std::string encoded_msg = urlEncode(msg + "\n");
        query.append(encoded_msg);
    }

    std::string command = "curl -s -d \"" + query + "\" " + url;

    std::system(command.c_str());
}

std::string CriticalNotificationTelegram::urlEncode(const std::string &str) const {
    std::ostringstream encoded;
    encoded.fill('0');
    encoded << std::hex;

    for (auto& c : str) {
        if (std::isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
            encoded << c;
            continue;
        }

        encoded << std::uppercase;
        encoded << '%' << std::setw(2) << int((unsigned char)c);
        encoded << std::nouppercase;
    }

    return encoded.str();
}
}
// 430803020