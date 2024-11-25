#ifndef BIBPARSER_H
#define BIBPARSER_H

#include <string>
#include <vector>
#include <memory>

enum class PublicationType {
    ARTICLE,
    PROCEEDINGS,
    UNKNOWN
};

class Author {
public:
    Author(const std::string& name, const std::string& affiliation)
        : name(name), affiliation(affiliation) {}

    std::string getName() const { return name; }
    std::string getAffiliation() const { return affiliation; }

private:
    std::string name;
    std::string affiliation;
};

class Publication {
public:
    Publication(PublicationType type, const std::string& citationKey,
                const std::string& title, const std::string& venue, int year)
        : type(type), citationKey(citationKey), title(title),
          venue(venue), year(year) {}

    void setDOI(const std::string& doi) { this->doi = doi; }
    void addAuthor(const std::shared_ptr<Author>& author) {
        authors.push_back(author);
    }

    // Getter functions for the publication attributes
    std::string getCitationKey() const { return citationKey; }
    std::string getTitle() const { return title; }
    std::string getVenue() const { return venue; }
    int getYear() const { return year; }
    std::string getDOI() const { return doi; }
    std::vector<std::shared_ptr<Author>> getAuthors() const { return authors; }

private:
    PublicationType type;
    std::string citationKey;
    std::string title;
    std::string venue;
    int year;
    std::string doi;
    std::vector<std::shared_ptr<Author>> authors;
};

class BibParser {
public:
    void loadFacultyList(const std::string& csvFile);
    std::vector<std::shared_ptr<Publication>> parseBibFile(const std::string& filename);
    double calculateAverageCoAuthors(const std::vector<std::shared_ptr<Publication>>& publications, const std::string& authorName) const;
private:
    std::vector<std::string> facultyList;
    std::string instituteAffiliation = "Institute Affiliation";

    // Helper functions
    std::string normalizeName(const std::string& name) const;
    std::pair<std::string, std::string> parseCitationKey(const std::string& line);
    std::vector<std::string> parseAuthors(const std::string& authorStr);
    bool validateBibEntry(const std::string& entry);
    bool checkAuthorDuplication(const std::vector<std::string>& authors) const;
    bool checkInstituteAffiliation(const std::vector<std::string>& authors) const;
    PublicationType getPublicationType(const std::string& typeStr);
    std::string extractField(const std::string& entry, const std::string& field);
};

#endif // BIBPARSER_H
