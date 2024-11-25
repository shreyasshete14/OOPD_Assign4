#ifndef PUBLICATION_H
#define PUBLICATION_H

#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <cassert>

// Enumeration for types of publications
enum class PublicationType {
    ARTICLE,
    PROCEEDINGS,
    UNKNOWN
};

// Class representing an author
class Author {
private:
    std::string name;         // Author's name
    std::string affiliation;  // Author's affiliation

public:
    // Constructor
    Author(const std::string& name, const std::string& affiliation)
        : name(name), affiliation(affiliation) {}

    // Getters
    std::string getName() const { return name; }
    std::string getAffiliation() const { return affiliation; }

    // Check if the author is affiliated with a specific institute
    bool isFromInstitute(const std::string& institute) const {
        return affiliation == institute;
    }
};

// Class representing a publication
class Publication {
private:
    PublicationType type;                    // Type of publication (e.g., ARTICLE, PROCEEDINGS)
    std::string citationKey;                // Citation key
    std::string title;                      // Title of the publication
    std::string venue;                      // Venue of publication
    std::vector<std::shared_ptr<Author>> authors;  // List of authors
    std::string doi;                        // Digital Object Identifier (optional)
    int year;                               // Year of publication

public:
    // Constructor
    Publication(PublicationType type, const std::string& citationKey,
                const std::string& title, const std::string& venue, int year)
        : type(type), citationKey(citationKey), title(title), venue(venue), year(year) {}

    // Add an author to the publication
    void addAuthor(const std::shared_ptr<Author>& author) {
        assert(author != nullptr && "Author cannot be null");
        authors.push_back(author);
    }

    // Set DOI for the publication
    void setDOI(const std::string& doi) { this->doi = doi; }

    // Check if the publication has an author affiliated with a specific institute
    bool hasInstituteAuthor(const std::string& institute) const {
        for (const auto& author : authors) {
            if (author->isFromInstitute(institute)) return true;
        }
        return false;
    }

    // Check if the publication has a specific author
    bool hasAuthor(const std::string& authorName) const {
        for (const auto& author : authors) {
            if (author->getName() == authorName) return true;
        }
        return false;
    }

    // Get the number of co-authors for a given author
    size_t getCoauthorCount(const std::string& authorName) const {
        return authors.size() > 1 ? authors.size() - 1 : 0;
    }

    // Print publication details
    void print() const {
        std::cout << "Title: " << title << "\n"
                  << "Venue: " << venue << "\n"
                  << "Authors: ";
        for (size_t i = 0; i < authors.size(); ++i) {
            std::cout << authors[i]->getName();
            if (i < authors.size() - 1) std::cout << ", ";
        }
        std::cout << "\nYear: " << year << "\n";
        if (!doi.empty()) std::cout << "DOI: " << doi << "\n";
        std::cout << "\n";
    }

    // Getters
    std::string getTitle() const { return title; }
    std::string getVenue() const { return venue; }
    int getYear() const { return year; }
    const std::vector<std::shared_ptr<Author>>& getAuthors() const { return authors; }
    std::string getCitationKey() const { return citationKey; }
};

#endif  // PUBLICATION_H

