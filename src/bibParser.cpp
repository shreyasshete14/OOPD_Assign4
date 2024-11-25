#include "bibParser.h"
#include <fstream>
#include <sstream>
#include <cassert>
#include <algorithm>
#include <regex>
#include <unordered_set>

// Load the faculty list
void BibParser::loadFacultyList(const std::string& csvFile) {
    std::ifstream file(csvFile);
    std::string line;
    while (std::getline(file, line)) {
        facultyList.push_back(normalizeName(line)); // Normalize faculty names
    }
}

// Normalize names (convert "Last, First" to "First Last")
std::string BibParser::normalizeName(const std::string& name) const {
    size_t commaPos = name.find(',');
    if (commaPos != std::string::npos) {
        std::string lastName = name.substr(0, commaPos);
        std::string firstName = name.substr(commaPos + 1);
        // Trim spaces and return normalized name as "First Last"
        return std::regex_replace(firstName + " " + lastName, std::regex("^ +| +$|( ) +"), "$1");
    }
    // If no comma, return the name as-is, trimmed
    return std::regex_replace(name, std::regex("^ +| +$|( ) +"), "$1");
}

// Extract a specific field from a BibTeX entry
std::string BibParser::extractField(const std::string& entry, const std::string& field) {
    std::regex pattern(field + "\\s*=\\s*\\{([^\\}]+)\\}");
    std::smatch matches;
    if (std::regex_search(entry, matches, pattern)) {
        return matches[1].str();
    }
    return "";
}

// Parse authors and normalize their names
std::vector<std::string> BibParser::parseAuthors(const std::string& authorStr) {
    std::vector<std::string> authors;
    size_t start = 0;
    size_t end = authorStr.find(" and ");

    while (end != std::string::npos) {
        std::string rawAuthor = authorStr.substr(start, end - start);
        authors.push_back(normalizeName(rawAuthor)); // Normalize author's name
        start = end + 5; // Move past " and "
        end = authorStr.find(" and ", start);
    }

    // Add the last author after the last "and"
    authors.push_back(normalizeName(authorStr.substr(start)));
    return authors;
}

// Parse the citation key and publication type
std::pair<std::string, std::string> BibParser::parseCitationKey(const std::string& line) {
    std::regex pattern("@(\\w+)\\{([^,]+),");
    std::smatch matches;
    if (std::regex_search(line, matches, pattern)) {
        return {matches[1].str(), matches[2].str()};
    }
    return {"", ""};
}

// Get the publication type
PublicationType BibParser::getPublicationType(const std::string& typeStr) {
    std::string upperType;
    std::transform(typeStr.begin(), typeStr.end(), std::back_inserter(upperType), ::toupper);

    if (upperType == "ARTICLE") return PublicationType::ARTICLE;
    if (upperType == "INPROCEEDINGS") return PublicationType::PROCEEDINGS;
    return PublicationType::UNKNOWN;
}

// Validate the BibTeX entry format
bool BibParser::validateBibEntry(const std::string& entry) {
    int openBraces = 0;
    bool inQuote = false;
    for (char c : entry) {
        if (c == '{') openBraces++;
        else if (c == '}') openBraces--;
        else if (c == '"') inQuote = !inQuote;

        if (openBraces < 0 || (inQuote && c == '}')) return false;
    }
    return openBraces == 0 && !inQuote;
}

// Check for duplicate authors
bool BibParser::checkAuthorDuplication(const std::vector<std::string>& authors) const {
    std::unordered_set<std::string> uniqueAuthors;
    for (const auto& author : authors) {
        if (uniqueAuthors.find(author) != uniqueAuthors.end()) {
            return false; // Duplicate author found
        }
        uniqueAuthors.insert(author);
    }
    return true; // No duplicates
}

// Check if any author is affiliated with the institute
bool BibParser::checkInstituteAffiliation(const std::vector<std::string>& authors) const {
    for (const auto& author : authors) {
        for (const auto& faculty : facultyList) {
            if (normalizeName(author).find(normalizeName(faculty)) != std::string::npos) {
                return true; // Match found
            }
        }
    }
    return false;
}


// Parse the BibTeX file
std::vector<std::shared_ptr<Publication>> BibParser::parseBibFile(const std::string& filename) {
    std::ifstream file(filename);
    assert(file.is_open() && "Cannot open bib file");

    std::vector<std::shared_ptr<Publication>> publications;
    std::string line;
    std::string currentEntry;

    while (std::getline(file, line)) {
        if (line.empty()) continue;

        if (line[0] == '@') {
            if (!currentEntry.empty()) {
                assert(validateBibEntry(currentEntry) && "Invalid bib entry format");

                auto [typeStr, citationKey] = parseCitationKey(currentEntry);
                auto type = getPublicationType(typeStr);

                std::string title = extractField(currentEntry, "title");
                std::string venue = extractField(currentEntry, "venue");
                if (venue.empty()) venue = extractField(currentEntry, "journal");

                std::string authorStr = extractField(currentEntry, "author");
                std::string yearStr = extractField(currentEntry, "year");
                std::string doi = extractField(currentEntry, "doi");

                int year = std::stoi(yearStr);

                auto pub = std::make_shared<Publication>(type, citationKey, title, venue, year);
                if (!doi.empty()) pub->setDOI(doi);

                auto authors = parseAuthors(authorStr);
                assert(checkAuthorDuplication(authors) && "Duplicate authors found");
                assert(checkInstituteAffiliation(authors) && "No institute-affiliated authors found");

                for (const auto& authorName : authors) {
                    auto author = std::make_shared<Author>(
                        authorName,
                        std::find(facultyList.begin(), facultyList.end(), normalizeName(authorName)) != facultyList.end()
                            ? instituteAffiliation
                            : "External");
                    pub->addAuthor(author);
                }

                publications.push_back(pub);
            }
            currentEntry = line;
        } else {
            currentEntry += line;
        }
    }

    if (!currentEntry.empty()) {
        assert(validateBibEntry(currentEntry) && "Invalid bib entry format");
        // Process the last entry (similar logic as above)
    }

    return publications;
}

double BibParser::calculateAverageCoAuthors(const std::vector<std::shared_ptr<Publication>>& publications, const std::string& authorName) const {
    int totalCoAuthors = 0;
    int paperCount = 0;

    for (const auto& publication : publications) {
        bool authorFound = false;

        for (const auto& author : publication->getAuthors()) {
            if (normalizeName(author->getName()) == normalizeName(authorName)) {
                authorFound = true;
                break;
            }
        }

        if (authorFound) {
            paperCount++;
            int coAuthors = publication->getAuthors().size() - 1;  // Exclude the author being searched
            totalCoAuthors += coAuthors;
        }
    }

    if (paperCount == 0) {
        return 0.0;
    }

    return static_cast<double>(totalCoAuthors) / paperCount;
}
