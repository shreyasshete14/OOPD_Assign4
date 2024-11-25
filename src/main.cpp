#include "bibParser.h"
#include <iostream>
#include <vector>
#include <memory>
#include <regex>
#include <cstdlib> // For exit

void printUsage(const char* programName) {
    std::cerr << "Usage: " << programName << " <faculty_list.csv> <bib_file.bib> <author_name>" << std::endl;
}

void printPublication(const std::shared_ptr<Publication>& pub) {
    std::cout << "Citation Key: " << pub->getCitationKey() << std::endl;
    std::cout << "Authors: ";
    for (const auto& author : pub->getAuthors()) {
        std::cout << author->getName() << " (" << author->getAffiliation() << "), ";
    }
    std::cout << std::endl;
    std::cout << "Title: " << pub->getTitle() << std::endl;
    std::cout << "Venue: " << pub->getVenue() << std::endl;
    std::cout << "------------------------------------------" << std::endl;
}

std::string normalizeName(const std::string& name) {
    size_t commaPos = name.find(',');
    if (commaPos != std::string::npos) {
        std::string lastName = name.substr(0, commaPos);
        std::string firstName = name.substr(commaPos + 1);
        // Trim spaces and return normalized name as "First Last"
        return std::regex_replace(firstName + " " + lastName, std::regex("^ +| +$|( ) +"), "$1");
    }
    // If no comma, return the name as-is
    return std::regex_replace(name, std::regex("^ +| +$|( ) +"), "$1");
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        printUsage(argv[0]);
        return EXIT_FAILURE;
    }

    std::string facultyFile = argv[1];
    std::string bibFile = argv[2];
    std::string authorName = argv[3];

    BibParser parser;

    // Load the faculty list
    parser.loadFacultyList(facultyFile);

    // Parse the BibTeX file
    auto publications = parser.parseBibFile(bibFile);

    // Normalize the search query for comparison
    std::string normalizedSearchName = normalizeName(authorName);

    // Search for publications with matching authors
    std::cout << "Searching for publications by \"" << normalizedSearchName << "\"..." << std::endl;
    bool found = false;

    for (const auto& publication : publications) {
        for (const auto& author : publication->getAuthors()) {
            if (normalizeName(author->getName()).find(normalizedSearchName) != std::string::npos) {
                printPublication(publication);
                found = true;
                break;
            }
        }
    }

    if (!found) {
        std::cout << "No publications found for \"" << normalizedSearchName << "\"." << std::endl;
    }

    // Calculate the average number of co-authors for the given author
    double averageCoAuthors = parser.calculateAverageCoAuthors(publications, authorName);
    std::cout << "Average number of co-authors for \"" << authorName << "\": " << averageCoAuthors << std::endl;

    return EXIT_SUCCESS;
}
