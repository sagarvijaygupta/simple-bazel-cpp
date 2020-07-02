// See README.txt for information and build instructions.
#include <iostream>
#include <fstream>
#include <istream>
#include <string>
#include <sstream>
#include <vector>
#include <google/protobuf/util/delimited_message_util.h>
#include "person.pb.h"
#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/strings/numbers.h"
#include "absl/strings/str_split.h"
#include "absl/strings/str_cat.h"  
#include "absl/strings/str_join.h"


ABSL_FLAG(std::string, first_name, "", "Tell the name of the character");
ABSL_FLAG(std::string, file_name, "", "Tell the name of the file");

bool read_csv_file(std::string filename, std::vector<::person_info::Person> &people) {
    std::fstream fin(filename, std::ios::in | std::ios::binary);
    if (!fin) {
        return false;
    }
    
    std::string read;
    while (fin >> read) {
        ::person_info::Person person;
        std::vector<std::string> sv = absl::StrSplit(read, ",");
        person.set_first_name(sv[0]);
        person.set_second_name(sv[1]);
        int value;
        if(!absl::SimpleAtoi(absl::string_view(sv[2]), &value))
            return false;
        person.set_age(value);
        person.set_profession(sv[3]);
        people.push_back(person);
    }
    return true;
}

std::string create_filename(std::string filename) {
    std::vector<std::string> path = absl::StrSplit(filename, absl::ByString("/"));
    std::string name = path.back();
    path.pop_back();
    std::pair<std::string, std::string> name_extension = absl::StrSplit(name, '.');
    std::string proto_file = absl::StrCat((std::string) absl::StrJoin(path, "/"), "/", name_extension.first, "_proto.", name_extension.second);
    return proto_file;
}

bool write_proto_file(std::string proto_file, std::vector<::person_info:: Person> &people) {
    std::fstream fout(proto_file, std::ios::out | std::ios::binary);
    for(auto person : people) {
        google::protobuf::util::SerializeDelimitedToOstream(person, &fout);
    }
    return true;
}

bool read_proto_file(std::string proto_file) {
    std::string character_name = absl::GetFlag(FLAGS_first_name);
    std::fstream fin(proto_file, std::ios::in | std::ios::binary);
    google::protobuf::io::IstreamInputStream istream_fin(&fin);
    bool isclean = true;
    ::person_info::Person person;

    while(google::protobuf::util::ParseDelimitedFromZeroCopyStream(&person, &istream_fin, &isclean)) {
        if(character_name != person.first_name() && character_name != "")
            continue;
        std::cout << person.first_name() << " " << person.second_name() << " " << person.age() <<  " " << person.profession() << std::endl;
    }
    return true;
}

int main(int argc, char* argv[]) {
    GOOGLE_PROTOBUF_VERIFY_VERSION;
    absl::ParseCommandLine(argc, argv);
    std::string filename = absl::GetFlag(FLAGS_file_name);

    if (filename == "") {
        std::cerr << "Usage " << argv[0] << " name of the file " << std::endl;
        return 0;
    }
    std::vector<::person_info::Person> people;

    std::string proto_file = create_filename(filename);
    bool is_readable_csv = read_csv_file(filename, people);
    if (!is_readable_csv) {
        std::cerr << "File " << filename << " not found!" << std::endl;
        return 0;
    }
    bool is_written = write_proto_file(proto_file, people);
    if (!is_written) {
        std::cerr << "Failed to write to file " << proto_file << std::endl; 
        return 0;
    }
    bool is_readable_proto = read_proto_file(proto_file);
    if (!is_readable_proto) {
        std::cerr << "File " << proto_file << " not found!" << std::endl;
        return 0;
    }
    return 0;
}
