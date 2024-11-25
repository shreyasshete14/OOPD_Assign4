Publication System
This project is a system to parse and manage publication data in BibTeX format. It allows users to search for publications by a given author and calculates the average number of co-authors per publication for the specified author. The system ensures that all publications adhere to certain rules, such as valid BibTeX format, authors affiliated with a specified institute, and no duplicate authors in a single publication entry.

Features
Search Publications by Author: Allows searching for publications by a specific author.
Average Co-Authors Calculation: Calculates the average number of co-authors per publication by a given author.
Validations:
Ensures the BibTeX file is correctly formatted.
Checks that no author is listed more than once in a publication entry.
Validates that all authors are affiliated with the specified institute.
Requirements
C++ Compiler (C++14 or higher)
Libraries:
Standard C++ Libraries (e.g., <iostream>, <vector>, <memory>, <string>, <regex>)
File Structure
src/ - Contains source code files:

main.cpp - The main entry point of the application.
bibParser.cpp - Contains implementation of the BibParser class.
include/ - Contains header files:

bibParser.h - Contains class declarations for handling publications and authors.
data/ - Stores sample data files:

faculty_list.csv - List of faculty members and their affiliations.
bib_file.bib - Sample BibTeX file containing publication entries.
Compilation
To compile the program, follow the steps below:

Navigate to the project directory.

Using g++:

bash
Copy code
g++ -std=c++14 -o publication_system src/main.cpp src/bibParser.cpp
Using Visual Studio or another IDE:

Include all the source files (main.cpp, bibParser.cpp) and header files in your project.
Ensure the C++14 (or higher) standard is enabled.
Usage
The program accepts three command-line arguments:

Path to the faculty list CSV file (faculty_list.csv).
Path to the BibTeX file containing publication entries (bib_file.bib).
The name of the author to search for in the publications.
Example:
bash
Copy code
./publication_system data/faculty_list.csv data/bib_file.bib "John Doe"
This will:

Load the faculty list from faculty_list.csv.
Parse the BibTeX entries from bib_file.bib.
Search for publications authored by "John Doe" (name will be normalized).
Print the publication details.
Calculate and display the average number of co-authors for publications by "John Doe".
Example Output:
text
Copy code
Searching for publications by "John Doe"...
Citation Key: Doe2023
Authors: John Doe (Institute Affiliation), Jane Smith (Institute Affiliation), 
Title: Advanced Research on AI
Venue: AI Journal 2023
------------------------------------------
Average number of co-authors for John Doe: 2.5
Functionality
Search for Publications: The program allows you to search for publications by a specified author name. The name is normalized (e.g., converting "Doe, John" to "John Doe") to handle different name formats.

Average Co-Authors Calculation: The system calculates the average number of co-authors (excluding the author being searched for) in publications by the specified author. This provides insights into how collaborative the author's research is.

Validation of BibTeX Format: The BibTeX file is validated for proper formatting:

Ensures all curly braces {} and commas are correctly placed.
Ensures no duplicate authors exist in the same publication entry.
Checks that all authors are affiliated with the specified institute.
Author Normalization: The system supports the normalization of author names (e.g., "Doe, John" becomes "John Doe") for easier searching and comparison.
I have taken help from Chatgpt for this Publication system.
