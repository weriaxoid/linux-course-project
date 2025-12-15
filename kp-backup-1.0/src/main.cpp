#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <chrono>
#include <filesystem>
#include <syslog.h>
#include <unistd.h>
#include <pwd.h>

namespace fs = std::filesystem;

const std::string SOURCE_FILE = "/var/log/secure"; 
const std::string BACKUP_DIR = "/var/tmp/backups"; 
const std::string USER_NAME = "zhopa"; //ПОЛЬЗОВАТЕЛЬ!!! ЕСЛИ НЕ БУДЕТ СОВПАДАТЬ, ТО ПРОГРАММА НЕ ЗАПУСТИТСЯ

void log_message(int priority, const std::string& message) {
    syslog(priority, "%s: %s", USER_NAME.c_str(), message.c_str());
}

int main() {
    openlog("kp-backup-daemon", LOG_PID, LOG_DAEMON);
    
    log_message(LOG_INFO, "Деамон запущен.");

    struct passwd *pw = getpwuid(getuid());
    if (pw) {
        std::string current_user(pw->pw_name);
        log_message(LOG_INFO, "Запуск с правами: " + current_user);
    }

    if (!fs::exists(BACKUP_DIR)) {
        try {
            fs::create_directories(BACKUP_DIR);
        } catch (const std::exception& e) {
            log_message(LOG_ERR, "Не удалось создать файл копий: " + std::string(e.what()));
            return 1;
        }
    }

    while (true) {
        try {
            std::string dest_filename = BACKUP_DIR + "/безопасная_копия_" + std::to_string(time(nullptr)); 
            std::ifstream src(SOURCE_FILE, std::ios::binary);
            if (!src.is_open()) {
                log_message(LOG_ERR, "Разрешение отклонено, или упущено: " + SOURCE_FILE);
            } else {
                std::ofstream dst(dest_filename, std::ios::binary);
                dst << src.rdbuf();
                log_message(LOG_INFO, "Копия создана: " + dest_filename);
            }
        } catch (const std::exception& e) {
            log_message(LOG_ERR, "Ошибка: " + std::string(e.what()));
        }
        std::this_thread::sleep_for(std::chrono::seconds(60));
    }

    closelog();
    return 0;
}
