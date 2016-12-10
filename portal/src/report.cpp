
#include <string>
#include "report.h"

static const char *htmlHead =
    "<!DOCTYPE html>"
    "<html>"
    "<head>"
    "<style>"
    "body {"
    "background-color: #f5f5f5;"
    "}"
    "h1 {"
    "padding: 30px 0 0 0;"
    "font-size: 1.7em;"
    "text-align: center;"
    "}"
    "h2 {"
    "position: relative;"
    "left: 12.5%;"
    "font-size: 1.2em;"
    "margin: 20px 0 2px 0;"
    "}"
    "h1, h2, th, td {"
    "font-family: \"Palatino Linotype\", \"Book Antiqua\", Palatino, serif;"
    "}"
    "table {"
    "width: 75%;"
    "text-align: center;"
    "box-shadow: 1px 1px 1px #999;"
    "background-color: #ffffff;"
    "}"
    "th {"
    "font-size: 1.2em;"
    "height: 35px;"
    "}"
    "td {"
    "margin: 5px 0 5px 0;"
    "padding: 10px 0 10px 0;"
    "padding-left: 10px;"
    "}"
    "td:hover {"
    "background-color: rgba(0, 100, 200, .4);"
    "}"
    "div#play-time-container {"
    "position: relative;"
    "left: 12.5%"
    "}"
    "/* D3 styles. */"
    ".bar {"
    "fill: steelblue;"
    "}"
    ".bar:hover {"
    "fill: brown;"
    "}"
    ".axis--x path {"
    "display: none;"
    "}"
    "student-plot {"
    "text-align: center;"
    "}"
    "</style>"
    "<script src=\"https://d3js.org/d3.v4.min.js\"></script>"
    "</head>"
    "<body>"
    "<h1>Loque Class Report - ";

static const char *preSummary =
    "</h1>"
    "<h2>Summary</h2>"
    "<table id=\"summary-table\" align=\"center\">"
    "<tr>"
    "<th>Total Time Played</th>"
    "<th>Highest Scoring Student</th>"
    "<th>Most Frequent Player</th>"
    "</tr>";

static std::string timeStrFromSeconds(int sec) {
    int hours = (sec / 60) / 60;
    int remaining = sec - (hours * 60 * 60); 
    int min = remaining / 60;
    remaining -= (min * 60);
    return std::to_string(hours) + " hours " + std::to_string(min) +
        " minutes " + std::to_string(remaining) + " seconds"; 
}

static std::string summaryRow(const ClassStats& stats) {
    const UserStats *highestScoring = stats.highestScoringStudent();
    const UserStats *mostFrequent = stats.mostFrequentPlayer();
    return "<tr><td>" + timeStrFromSeconds(stats.totalSecPlayed()) +
        "</td><td>" + (highestScoring == nullptr ? "" : highestScoring->username) +
        "</td><td>" + (mostFrequent == nullptr ? "" : mostFrequent->username) + "</td></tr>";
        
}

static const char *preEnabledLevels =
    "</table>"
    "<h2>Enabled Levels</h2>"
    "<table id=\"levels-table\" align=\"center\">"
    "<tr>"
    "<th>Name</th>"
    "<th>Number</th>"
    "<th>Description</th>"
    "</tr>";

static std::string enabledLevelRow(const LevelInfo& level) {
    return "<tr><td>" + level.name +
        "</td><td>" + std::to_string(level.id) +
        "</td><td>" + level.description + "</td></tr>";
}

static const char *preStudentDetail =
    "</table>"
    "<h2>Student Detail</h2>"
    "<table id=\"student-detail-table\" align=\"center\">"
    "<tr>"
    "<th>Student</th>"
    "<th>Total Time Played</th>"
    "<th>Total Score</th>"
    "<th>Levels Complete</th>"
    "</tr>";

static std::string studentRow(const UserStats& student) {
    std::vector<int> levelsComplete;
    student.levelsComplete(levelsComplete);
    std::string levelsStr; 
    for (int i = 0; i < (int)levelsComplete.size(); i++) {
        levelsStr += std::to_string(levelsComplete.at(i));
        if (i < (int)levelsComplete.size() - 1) levelsStr += ", ";
    }
    return "<tr><td>" + student.username +
        "</td><td>" + timeStrFromSeconds(student.totalSecPlayed) +
        "</td><td>" + std::to_string(student.totalScore) +
        "</td><td>" + levelsStr + "</td></tr>";
}

static const char *preStudentPlayTimePlot =
    "</table>"
    "<h2>Student Play Time Plot (Minutes)</h2>"
    "<div id=\"play-time-container\">"
    "<svg id=\"student-plot\" width=\"960\" height=\"500\"></svg>"
    "</div>"
    "<script>"
    "var svg = d3.select(\"svg\"),"
    "margin = {top: 20, right: 20, bottom: 30, left: 40},"
    "width = +svg.attr(\"width\") - margin.left - margin.right,"
    "height = +svg.attr(\"height\") - margin.top - margin.bottom;"
    "var x = d3.scaleBand().rangeRound([0, width]).padding(0.1),"
    "y = d3.scaleLinear().rangeRound([height, 0]);"
    "var g = svg.append(\"g\")"
    ".attr(\"transform\", \"translate(\" + margin.left + \",\" + margin.top + \")\");"
    "var data = [";

static std::string studentPlayTimeObject(const UserStats& student) {
    return "{name: '" + student.username + "', minutes: " +
        std::to_string((student.totalSecPlayed / 60)) + "}";
}

static const char *htmlCloser =
    "];"
    "x.domain(data.map(function(d) { return d.name; }));"
    "y.domain([0, d3.max(data, function(d) { return d.minutes; })]);"
    "g.append(\"g\")"
    ".attr(\"class\", \"axis axis--x\")"
    ".attr(\"transform\", \"translate(0,\" + height + \")\")"
    ".call(d3.axisBottom(x));"
    "g.append(\"g\")"
    ".attr(\"class\", \"axis axis--y\")"
    ".call(d3.axisLeft(y).ticks(10))"
    ".append(\"text\")"
    ".attr(\"transform\", \"rotate(-90)\")"
    ".attr(\"y\", 6)"
    ".attr(\"dy\", \"0.71em\")"
    ".attr(\"text-anchor\", \"end\")"
    ".text(\"Minutes\");"
    "g.selectAll(\".bar\")"
    ".data(data)"
    ".enter().append(\"rect\")"
    ".attr(\"class\", \"bar\")"
    ".attr(\"x\", function(d) { return x(d.name); })"
    ".attr(\"y\", function(d) { return y(d.minutes); })"
    ".attr(\"width\", x.bandwidth())"
    ".attr(\"height\", function(d) { return height - y(d.minutes); });"
    "</script>"
    "</body>"
    "</html>";


void writeHtmlReport(std::ostream& out, const ClassStats& stats) {
    out << htmlHead <<
        stats.className <<
        preSummary <<
        summaryRow(stats) <<
        preEnabledLevels;
    for (auto& level : stats.enabledLevels) {
        out << enabledLevelRow(level);
    }
    out << preStudentDetail;
    for (auto& student : stats.studentStats) {
        out << studentRow(student); 
    }
    out << preStudentPlayTimePlot;
    for (int i = 0; i < (int)stats.studentStats.size(); i++) {
        out << studentPlayTimeObject(stats.studentStats.at(i));
        if (i < (int)stats.studentStats.size() - 1) out << ", ";
    }
    out << htmlCloser; 
}
