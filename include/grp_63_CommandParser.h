// group-63
// group-63
#ifndef GRP_63_COMMAND_PARSER_H
#define GRP_63_COMMAND_PARSER_H

#include <iosfwd>
#include <string>

#include "grp_63_Registrar.h"

namespace acaderp {

class CommandParser {
private:
    Registrar registrar_;
    int lineNumber_;

public:
    CommandParser();

    void execute(
        const std::string& line,
        std::ostream& output,
        std::ostream& error
    );

    void run(
        std::istream& input,
        std::ostream& output,
        std::ostream& error
    );
};

} // namespace acaderp

#endif