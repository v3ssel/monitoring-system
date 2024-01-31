#include <vector>
#include <fstream>
#include <filesystem>
#include "CriticalNotificationEmail.h"

namespace s21 {
const std::string CriticalNotificationEmail::sender_mail_ = "mntrngsstm@gmail.com";
const std::string CriticalNotificationEmail::sender_password_ = "lckf gpvr bkzg zuuj";

void CriticalNotificationEmail::sendNotification(const std::string &receiver, const std::vector<std::string>& msgs) {
    std::filesystem::path mail_path = std::filesystem::current_path() / "mail.txt";
    std::string sender_query = "--user \"" + sender_mail_ + ":" + sender_password_ + "\" --mail-from \"" + sender_mail_ + "\"";
    std::string receiver_query = "--mail-rcpt \"" + receiver + "\" --upload-file " + mail_path.string();

    std::vector<std::string> mail_content;
    mail_content.reserve(4);
    mail_content.emplace_back("From: \"Monitoring System Critical Value Notifier\" <" + sender_mail_ + ">\n");
    mail_content.emplace_back("To: \"" + receiver.substr(0, receiver.find('@')) + "\" <" + receiver + ">\n");
    mail_content.emplace_back("Subject: Critical value notification\n\r");
    mail_content.emplace_back("HOST: " + getHostname() + "\n");

    for (auto &msg : msgs) {
        mail_content.emplace_back(msg + "\n");
    }

    std::ofstream file(mail_path);
    for (auto &line : mail_content) {
        file << line;
    }
    file.close();

    std::string command = "curl --silent --ssl-reqd --url \"smtps://smtp.gmail.com:465\" " + sender_query + " " + receiver_query;
    std::system(command.c_str());

    std::filesystem::remove(mail_path);
}   
} // namespace s21

// curl --ssl-reqd --url 'smtps://smtp.gmail.com:465' --user 'mntrngsstm@gmail.com:lckf gpvr bkzg zuuj' --mail-from 'mntrngsstm@gmail.com' --mail-rcpt 'danilpenski@gmail.com' --upload-file mail.txt
