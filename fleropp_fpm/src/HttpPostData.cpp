//---------------------------------------------------------------------------
//
// Copyright (c) Parallel and Cloud Computing Laboratory (PC2Lab)
// Miami University, Oxford, OH.
// All rights reserved.
//
// PC2Lab and Miami University (MU) make no representations or
// warranties about the suitability of the software, either express or
// implied, including but not limited to the implied warranties of
// merchantability, fitness for a particular purpose, or
// non-infringement.  PC2Lab or MU shall not be liable for any damages
// suffered by licensee as a result of using, result of using,
// modifying or distributing this software or its derivatives.
//
// By using or copying this Software, Licensee agrees to abide by the
// intellectual property laws, and all other applicable laws of the
// U.S., and the terms of this license.
//
// Authors: Dhananjai M. Rao       raodm@muohio.edu
//
//---------------------------------------------------------------------------

#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include "HttpPostData.h"


// Parse out values for "name" or "filename" (specified as field) from
// HTTP headers of the form:
// Content-Disposition: form-data; name="file"; filename="test.txt"
std::string
HttpPostData::getHeaderVal(const std::string& header, std::string field) const {
    std::istringstream data_stream(header);
    field += "=\"";
    std::string nameVal;
    while (data_stream >> nameVal) {
        if (nameVal.find(field) == 0) {
            const size_t valLen = nameVal.rfind('\"') - field.size();
            return nameVal.substr(field.size(), valLen);
        }
    }
    return "";
}

// Convenience method to parse HTTP headers until a blank line in encountered
// This method also suitably sets values of instance variables.
void
HttpPostData::loadHeaders(std::istream& is) {
    std::string header;
    while (std::getline(is, header) && (header != "\r")) {
        // Setup values of variaous instance variables based on header values
        if (header.find("Content-Disposition:") == 0) {
            name     = getHeaderVal(header, "name");
            filename = getHeaderVal(header, "filename");
        } else if (header.find("Content-Type:") == 0) {
            contentType = header.substr(14);
        }
    }
}

void
HttpPostData::storeData(const std::string& input, std::ostream& out,
                        int skipLast) {
    // Suitably handle data.
    if (!out.good() && !input.empty()) {
        data.insert(data.end(), input.begin(), input.end() - skipLast);
    } else if (!input.empty()) {
        out.write(input.data(), input.size() - skipLast);
    }
}

// Load one multi-part POST entry while optinally saving file data in
// a given directory (sotreDir).
bool
HttpPostData::load(std::istream& is, const std::string& bndry,
                   const std::string& storeDir) {
    const std::string lBndry = bndry.substr(0, bndry.size() - 1) + "--\r";
    loadHeaders(is);  // Load necessary information into the headers
    // Create output stream to write data to file if requested
    std::ofstream out;
    if ((storeDir != "") && (filename != "")) {
        out.open(storeDir + "/" + filename);
    } else {
        out.setstate(std::ios::badbit);
    }
    // Read data until boundary is hit. Since the loop processes
    // line-by-line, here we use prevLine to avoid storing the
    // boundary line.

    std::string prevLine, currLine;
    while (std::getline(is, currLine) && (currLine != bndry) &&
           (currLine != lBndry)) {
        // Suitably handle data (write or store)
        storeData(prevLine, out);
        // The current line will be written/stored if next line is not
        // the boundary string.
        prevLine = currLine + "\n";
    }
    // Write peinding data. Do not include the last "\r\n" as it is
    // part of boundary (not data) by specifying 2 as last argument.
    storeData(prevLine, out, 2);
    // Return true if the last boundary has been read and no further
    // entries need to be read.
    return (currLine == lBndry);
}

std::vector<HttpPostData>
HttpPostData::loadPostData(std::istream& is, const std::string& storeDir) {
    is >> std::noskipws;  // Don't skip white space as we need that data!
    std::vector<HttpPostData> postData;
    // First line is always mime boundary string
    std::string mimeBoundary;
    std::getline(is, mimeBoundary);
    // process each part of the multi-part HTTP POST data.
    bool lastBoundaryFound = false;
    while (is.good() && !lastBoundaryFound) {
        HttpPostData pde;
        lastBoundaryFound = pde.load(is, mimeBoundary, storeDir);
        postData.push_back(pde);
    }
    return postData;
}

// End of source code
