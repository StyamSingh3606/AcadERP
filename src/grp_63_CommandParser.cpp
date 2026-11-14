
// group-63

#include "grp_63_CommandParser.h"

#include <iostream>
#include <sstream>
#include <vector>
#include <memory>
#include <string>
#include <iomanip>

#include "grp_63_Exceptions.h"
#include "grp_63_RegularCourse.h"
#include "grp_63_SuCourse.h"
#include "grp_63_ThesisCourse.h"
#include "grp_63_Student.h"

namespace acaderp {

CommandParser::CommandParser()
    : registrar_(), lineNumber_(0) {}

namespace {

std::vector<std::string> tokenize(const std::string& line) {
    std::istringstream stream(line);
    std::vector<std::string> tokens;
    std::string token;

    while (stream >> token) {
        tokens.push_back(token);
    }

    return tokens;
}

int parseInteger(const std::string& value) {
    try {
        std::size_t position = 0;
        int result = std::stoi(value, &position);

        if (position != value.size()) {
            throw ParseException("Invalid integer: " + value);
        }

        return result;
    }
    catch (const std::exception&) {
        throw ParseException("Invalid integer: " + value);
    }
}

int parseGpaHundredths(const std::string& value) {
    const std::size_t dot = value.find('.');

    if (dot == std::string::npos ||
        dot == 0 ||
        value.size() != dot + 3) {
        throw ParseException("Invalid GPA format: " + value);
    }

    for (char ch : value) {
        if (ch != '.' && (ch < '0' || ch > '9')) {
            throw ParseException("Invalid GPA format: " + value);
        }
    }

    const int whole = parseInteger(value.substr(0, dot));
    const int fraction = parseInteger(value.substr(dot + 1));

    if (whole < 0 || whole > 10 || fraction < 0 ||
        fraction > 99 || (whole == 10 && fraction != 0)) {
        throw ParseException("GPA out of range: " + value);
    }

    return whole * 100 + fraction;
}

bool isValidTerm(const std::string& term) {
    if (term.size() != 6 || term[4] != '-') {
        return false;
    }

    for (int i = 0; i < 4; ++i) {
        if (term[static_cast<std::size_t>(i)] < '0' ||
            term[static_cast<std::size_t>(i)] > '9') {
            return false;
        }
    }

    return term[5] >= '1' && term[5] <= '3';
}

} // namespace

void CommandParser::execute(
    const std::string& line,
    std::ostream& output,
    std::ostream& error
) {
    const std::vector<std::string> tokens = tokenize(line);

    if (tokens.empty() || tokens[0][0] == '#') {
        return;
    }

    try {
        const std::string& command = tokens[0];

        // PROGRAM command
        if (command == "PROGRAM") {
            if (tokens.size() != 8) {
                throw ParseException("Invalid PROGRAM syntax");
            }

            ProgramRules rules;

            const std::string expected[] = {
                "total=", "elective=", "mingpa=",
                "thesis_min_grade=", "thesis_min_credits=",
                "thesis_min_gpa=", "maxload="
            };

            std::vector<std::string> values;

            for (std::size_t i = 1; i < tokens.size(); ++i) {
                const std::size_t pos = tokens[i].find('=');

                if (pos == std::string::npos ||
                    tokens[i].substr(0, pos + 1) != expected[i - 1]) {
                    throw ParseException("Invalid PROGRAM argument");
                }

                values.push_back(tokens[i].substr(pos + 1));
            }

            rules.setTotalCredits(parseInteger(values[0]));
            rules.setElectiveCredits(parseInteger(values[1]));
            rules.setMinimumGpaHundredths(
                parseGpaHundredths(values[2]));
            rules.setThesisMinimumGrade(values[3]);
            rules.setThesisMinimumCredits(parseInteger(values[4]));
            rules.setThesisMinimumGpaHundredths(
                parseGpaHundredths(values[5]));
            rules.setMaximumLoad(parseInteger(values[6]));

            if (rules.totalCredits() <= 0 ||
                rules.electiveCredits() < 0 ||
                rules.thesisMinimumCredits() < 0 ||
                rules.maximumLoad() <= 0) {
                throw ParseException("Invalid PROGRAM values");
            }

            registrar_.setProgramRules(rules);
        }

        // COURSE command
        else if (command == "COURSE") {
            if (tokens.size() < 5) {
                throw ParseException("Invalid COURSE syntax");
            }

            const std::string& code = tokens[1];
            const std::string& type = tokens[2];
            const int credits = parseInteger(tokens[3]);
            const std::string& category = tokens[4];

            std::vector<std::string> prerequisites;
            std::vector<std::string> antiRequisites;

            for (std::size_t i = 5; i < tokens.size(); ++i) {
                if (tokens[i].rfind("PRE=", 0) == 0) {
                    const std::string value = tokens[i].substr(4);
                    std::istringstream stream(value);
                    std::string item;

                    while (std::getline(stream, item, ',')) {
                        if (item.empty()) {
                            throw ParseException(
                                "Invalid prerequisite");
                        }

                        const std::size_t colon = item.find(':');

                        if (colon != std::string::npos) {
                            if (colon == 0 || colon + 1 == item.size()) {
                                throw ParseException(
                                    "Invalid prerequisite grade");
                            }

                            const std::string minimumGrade =
                                item.substr(colon + 1);

                            const Grade grade = Grade::create(minimumGrade);

                            if (!grade.isFinal() ||
                                !grade.isPass() ||
                                minimumGrade == "I") {
                                throw ParseException(
                                    "Invalid prerequisite grade");
                            }
                        }

                        prerequisites.push_back(item);
                    }
                }
                else if (tokens[i].rfind("ANTI=", 0) == 0) {
                    const std::string value = tokens[i].substr(5);
                    std::istringstream stream(value);
                    std::string item;

                    while (std::getline(stream, item, ',')) {
                        if (item.empty()) {
                            throw ParseException(
                                "Invalid anti-requisite");
                        }

                        antiRequisites.push_back(item);
                    }
                }
                else {
                    throw ParseException("Invalid COURSE option");
                }
            }

            std::unique_ptr<Course> course;

            if (type == "REGULAR") {
                course = std::make_unique<RegularCourse>(
                    code, credits, category,
                    prerequisites, antiRequisites);
            }
            else if (type == "SU") {
                course = std::make_unique<SuCourse>(
                    code, credits, category,
                    prerequisites, antiRequisites);
            }
            else if (type == "THESIS") {
                course = std::make_unique<ThesisCourse>(
                    code, credits, category,
                    prerequisites, antiRequisites);
            }
            else {
                throw ParseException(
                    "Invalid course type: " + type);
            }

            registrar_.addCourse(std::move(course));
        }

        // STUDENT command
        else if (command == "STUDENT") {
            if (tokens.size() != 3) {
                throw ParseException("Invalid STUDENT syntax");
            }

            registrar_.addStudent(tokens[1], tokens[2]);
        }

        // REGISTER command
        else if (command == "REGISTER") {
            if (tokens.size() != 4) {
                throw ParseException("Invalid REGISTER syntax");
            }

            if (!isValidTerm(tokens[3])) {
                throw ParseException("Invalid term: " + tokens[3]);
            }

            registrar_.registerCourse(
                tokens[1], tokens[2], tokens[3]);
        }

        // GRADE command
        else if (command == "GRADE") {
            if (tokens.size() != 5) {
                throw ParseException("Invalid GRADE syntax");
            }

            if (!isValidTerm(tokens[3])) {
                throw ParseException("Invalid term: " + tokens[3]);
            }

            registrar_.submitGrade(
                tokens[1], tokens[2], tokens[3], tokens[4]);
        }

        // CONVERT command
        else if (command == "CONVERT") {
            if (tokens.size() != 4) {
                throw ParseException("Invalid CONVERT syntax");
            }

            registrar_.convertThesis(
                tokens[1], tokens[2], tokens[3]);
        }

        // GPA command
        else if (command == "GPA") {
            if (tokens.size() != 2) {
                throw ParseException("Invalid GPA syntax");
            }

            const int gpa =
                registrar_.calculateGpaHundredths(tokens[1]);

            output << "GPA " << tokens[1] << " "
                   << (gpa / 100) << "."
                   << std::setw(2) << std::setfill('0')
                   << (gpa % 100) << '\n';
        }

        // AVG command
        else if (command == "AVG") {
            if (tokens.size() != 2) {
                throw ParseException("Invalid AVG syntax");
            }

            output << "AVG " << tokens[1] << " "
                   << registrar_.calculateAverage(tokens[1])
                   << '\n';
        }
        else if (command == "TRANSCRIPT") {
    if (tokens.size() != 2)
        throw ParseException("Invalid TRANSCRIPT syntax");

    output << registrar_.generateTranscript(tokens[1]) << '\n';
}
else if (command == "GRAD") {
    if (tokens.size() != 2)
        throw ParseException("Invalid GRAD syntax");

    const std::vector<std::string> failures =
        registrar_.graduationFailures(tokens[1]);

    output << "GRAD " << tokens[1] << " ";

    if (failures.empty()) {
        output << "YES\n";
    } else {
        output << "NO ";
        for (std::size_t i = 0; i < failures.size(); ++i) {
            if (i > 0) output << ",";
            output << failures[i];
        }
        output << '\n';
    }
}
else if (command == "ELIGIBLE") {
    if (tokens.size() != 4)
        throw ParseException("Invalid ELIGIBLE syntax");

    if (!isValidTerm(tokens[3]))
        throw ParseException("Invalid term: " + tokens[3]);

    const std::string failure =
        registrar_.eligibilityFailure(tokens[1], tokens[2], tokens[3]);

    if (failure.empty()) {
        output << "ELIGIBLE " << tokens[1] << " " << tokens[2]
               << " YES\n";
    } else {
        output << "ELIGIBLE " << tokens[1] << " " << tokens[2]
               << " NO " << failure << '\n';
    }
}
        else {
            throw ParseException("Unsupported command: " + command);
        }
    }
    catch (const ErpException& exception) {
        output << "ERROR " << lineNumber_ << " "
               << exception.code() << '\n';

        error << exception.what() << '\n';
    }
    catch (const std::exception& exception) {
        output << "ERROR " << lineNumber_ << " E_PARSE\n";
        error << exception.what() << '\n';
    }
}

void CommandParser::run(
    std::istream& input,
    std::ostream& output,
    std::ostream& error
) {
    std::string line;

    while (std::getline(input, line)) {
        ++lineNumber_;

        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }

        execute(line, output, error);
    }
}

} // namespace acaderp
