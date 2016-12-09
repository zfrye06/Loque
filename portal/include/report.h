#ifndef REPORT_H
#define REPORT_H

#include <ostream>
#include "../shared/loqueclient.h"

// Writes an html report describing the given class stats
// to the given output stream. 
void writeHtmlReport(std::ostream&, const ClassStats&);

#endif
