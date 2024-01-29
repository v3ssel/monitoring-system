#include <fstream>
#include <queue>

#include "AgentConfigWriter.h"

namespace s21 {
void AgentConfigWriter::write(const std::string &filename,
                              const std::string &metric_name,
                              const std::string &compare_sign,
                              const std::string &metric_value) {
    std::fstream file(filename, std::ios::in | std::ios::out);

    std::queue<std::string> file_contents;
    bool changed = false;

    std::string changed_delim, changed_value;
    std::string line;
    while (std::getline(file, line)) {
        size_t index = line.find_first_of(":><!=");

        if (!changed && index != std::string::npos && line.substr(0, index) == metric_name) {
            size_t metric_len = metric_name.length();

            if (line[index] == ':') {
                line.replace(index + 1, line.length(), metric_value);
                changed = true;
                file_contents.push(line);
                continue;
            }

            if (metric_len < line.length() - 1 && line[metric_len + 1] == '=') {
                changed_delim = {line[metric_len], line[metric_len + 1]};
                index++;
            } else {
                changed_delim = line[metric_len];
            }

            changed_delim = compare_sign.empty() ? changed_delim : compare_sign;
            changed_value = metric_value.empty() && !compare_sign.empty() ? line.substr(index + 1) : metric_value;

            line = metric_name + changed_delim + changed_value;
            changed = true;
        }

        file_contents.push(line);
    }
    file.close();

    if (!changed) {
        changed_delim = compare_sign.empty() ? "==" : compare_sign;
        changed_value = metric_value.empty() ? "0" : metric_value;
        file_contents.push(metric_name + changed_delim + changed_value);
    }

    file.open(filename, std::ios::out);
    while (!file_contents.empty()) {
        file << file_contents.front() << "\n";
        file_contents.pop();
    }
}
}
