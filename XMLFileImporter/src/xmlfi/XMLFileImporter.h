#pragma once

namespace cse380 {
  namespace xmlfi {

    // Technically we don't need this class anymore since I upgraded to tinyxml2,
    // but I wanna keep it around in case you want to put in some common error-handling
    // functionality or something.
    // -JTG
    class XMLFileImporter {
    public:
      virtual ~XMLFileImporter() {}
    };
  }
}